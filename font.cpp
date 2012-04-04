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
   Font::Font()
   {
      BaseInit();
      cache = nullptr;
      flags = 0;
   }
   Font::~Font()
   {
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

      precache(U"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvXxYyZzÄäÖö01234567890().,-+!*?\"\u2020\u00a9");

      return 0;
   }
   int Font::precache(string chars)
   {
      FT_UInt glyph_index;
      int fterror;

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
         glyph_index = FT_Get_Char_Index(face, charcode);

         if ((fterror = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)))
         {
            warn("character not found");
            continue;
         }
         if ((fterror = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)))
         {
            warn("character not found");
            continue;
         }

         int32_t rv = cache->Add(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
         if ( rv < 0 )
         {
            warn("texture full");
            return -1;
         }
         else
         {
            chartable.insert( std::pair<char32_t, rect_t>(charcode, cache->mapped[rv]) );
         }
      }
      return 0;
   }
   int Font::drawtest()
   {
      return 0;
   }
   // Static member definitions
   FT_Library Font::ftlib = nullptr;
   int Font::fcount = 0;
}

