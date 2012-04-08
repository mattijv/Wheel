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

      precache(U"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvXxYyZzÄäÖö01234567890().,-+!*?\"\u2020\u00a9");

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
      if ( rv < 0 )
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
   void Font::render()
   {
   }
   void Font::batch(const string& text)
   {
   }
   void Font::write(const string& text)
   {
      //TODO: VBO/VAO, this is just quick test to see everything works
      glyph_t g;

      float s1 ,x;
      float t1 ,y;
      float s2 ,w;
      float t2 ,h;

      glMatrixMode(GL_MODELVIEW);

      for (char32_t charcode : text)
      {
         if ( chartable.count(charcode) == 0 )
         {
            //TODO: Load the character from fly
            std::cout << "no charcode\n";
            continue;
         }
         g = chartable[charcode];

         s1 = g.coord.x / (float)cache->width;
         t1 = g.coord.y / (float)cache->height;
         s2 = (g.coord.x+g.coord.w) / (float)cache->width;
         t2 = (g.coord.y+g.coord.h) / (float)cache->height;

         x = g.metrics.x / (float)(scrwidth) * 2.0f;
         y = g.metrics.y / (float)(scrheight) * 2.0f;
         w = (g.metrics.w) / (scrwidth) * 2.0f;
         h = (g.metrics.h) / (scrheight) * 2.0f;
  
         glTranslatef(x,y-h,0);
         glBegin( GL_TRIANGLE_STRIP );
            glTexCoord2f( s1, t1 );
            glVertex2f( 0.0f, h );
            glTexCoord2f( s2, t1 );
            glVertex2f( w, h );
            glTexCoord2f( s1, t2 );
            glVertex2f( 0.0f, 0.0f );
            glTexCoord2f( s2, t2 );
            glVertex2f( w,  0.0f );
         glEnd();
         glTranslatef(-x,h-y,0);

         glTranslatef(g.advance / scrwidth * 2.0f, 0, 0);
      }
      glLoadIdentity();

   }
   // Static member definitions
   FT_Library Font::ftlib = nullptr;
   int Font::fcount = 0;
}

