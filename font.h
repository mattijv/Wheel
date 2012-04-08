#ifndef WHEEL_FONT_HEAD
#define WHEEL_FONT_HEAD

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <tuple>
#include <unordered_map>

#include "atlas.h"
#include "wtypes.h"

#define WHEEL_REQUEST_KERNING_UPDATE   0x02
#define WHEEL_CLEAN_VERTICE_BUFFER     0x04

namespace wheel
{
   struct glyph_t
   {
      std::unordered_map<char32_t, float> kerning;
      rect_t metrics;
      rect_t coord;

      float advance;
      uint32_t vao;
   };

   class Font
   {
      private:
         static int fcount;
         static FT_Library ftlib;

         uint32_t BaseInit();
         int32_t AddGlyph(char32_t);

         FT_Face face;
         uint32_t ptsize;

//         std::unordered_map<cvec2d_t, uint32_t>* vertices;

      public:
         Atlas* cache;

         uint32_t flags;

         std::unordered_map<char32_t, glyph_t> chartable;

         Font();
        ~Font();

         int load(const char* file, unsigned int pt);
         int precache(string chars);

         void batch(const string& text);
         void write(const string& text);
         void render();
         
   };
}

#endif


