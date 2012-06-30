#include "font.h"

inline void die( const char* err = nullptr)
{
   if (err != nullptr)
      std::cout << err << "\n";
   exit(1);
}

inline void warn( const char* err = nullptr)
{
   if (err != nullptr)
      std::cout << err << "\n";
}

namespace wheel
{
   float scrwidth = 1024.0f;
   float scrheight = 640.0f;
}

namespace wheel
{
   Font::Font()
   {
      BaseInit();
      cache = nullptr;
//      vertices = nullptr;

      pen_x = 0;
      pen_y = 0;
      colour = 0xffffffff;

      flags = 0;
   }
   Font::~Font()
   {
/*
      if ( flags & WHEEL_CLEAN_VERTICE_BUFFER )
      {
         delete vertices;
         vertices = nullptr;
      }
*/
      if ( face != nullptr )
         FT_Done_Face( face );

      fcount--;
      if ( fcount == 0 )
      {
         FT_Done_FreeType(ftlib);
      }
   }

   uint32_t Font::BaseInit()
   {
      face = nullptr;
      if ( fcount == 0 )
      {
         if ( FT_Init_FreeType(&ftlib) )
         {
            die("Font system failed");
         }
      }
      fcount++;

      return fcount;
   }
   int Font::load(const char* file, unsigned int pt)
   {
      int err = 0;
      if ((err = FT_New_Face(ftlib, file, 0, &face)))
         return err;

      if ((err = FT_Select_Charmap(face, FT_ENCODING_UNICODE)))
         return err;

      if ((err = FT_Set_Char_Size(face, pt<<6, pt<<6, 96, 96)))
         return err;

      precache(U"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZzÄäÖö01234567890().:,-+!*?\" \u2020\u00a9");

      return 0;
   }
   int32_t Font::AddGlyph(char32_t charcode)
   {
      FT_UInt glyph_index;
      int fterror;

      glyph_index = FT_Get_Char_Index(face, charcode);

      if ((fterror = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)))
      {
         warn("character not found");
         return 0;
      }
      if ((fterror = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)))
      {
         warn("character not found");
         return 0;
      }

      int32_t rv = cache->Add(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
      if ( (rv < 0) && (charcode != U' ') )
      {
         warn("texture full");
         return WHEEL_ATLAS_FULL;
      }
      else
      {
         glyph_t nglyph;
         nglyph.coord = cache->mapped[rv];
         flags |= WHEEL_REQUEST_KERNING_UPDATE;

         nglyph.metrics.x = face->glyph->bitmap_left;
         nglyph.metrics.y = face->glyph->bitmap_top;
         nglyph.metrics.w = face->glyph->bitmap.width;
         nglyph.metrics.h = face->glyph->bitmap.rows;

         nglyph.advance = face->glyph->advance.x >> 6;
/*
         if (vertices == nullptr)
         {
            vertices = new std::unordered_map<cvec2d_t, uint32_t>();
            flags |= WHEEL_CLEAN_VERTICE_BUFFER;
            cvec2d_t t;
         }
*/
         chartable.insert( std::pair<char32_t, glyph_t>(charcode, nglyph) );
      }
      return 0;
   }

   int Font::precache(string chars)
   {
      if (cache == nullptr)
      {
         cache = new Atlas;
         cache->Init();
         flags |= WHEEL_CLEAN_AT_EXIT;
         cache->SetFormat(WHEEL_ATLAS_FMT_ALPHA);
         cache->Reset();
      }

      for (char32_t charcode : chars)
      {
         int32_t rval = AddGlyph(charcode);
         if ( rval != 0 )
            return rval;
      }
      return 0;
   }
   void Font::batchr(const string& text, UIRenderer* uir)
   {
      if (uir == nullptr) return;

      glyph_t g;
      for (char32_t charcode : text)
      {
         if (chartable.count(charcode) == 0)
         {
            continue;
         }
         g = chartable[charcode];

         uir->addrect(g.metrics, g.coord, cache->width, cache->height, pen_x, pen_y, colour);
         pen_x += g.advance;
      }
   }
   void Font::move(uint32_t x, uint32_t y)
   {
      pen_x = x;
      pen_y = y;
   }
   void Font::setcolour(uint32_t c)
   {
      colour = c;
   }
   void Font::write(const string& text)
   {
   }
   // Static member definitions
   FT_Library Font::ftlib = nullptr;
   int Font::fcount = 0;
}

