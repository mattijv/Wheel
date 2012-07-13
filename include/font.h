#ifndef WHEEL_FONT_HEAD
#define WHEEL_FONT_HEAD

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <tuple>
#include <unordered_map>

#include "atlas.h"
#include "wtypes.h"
#include "baserenderer.h"

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

         uint32_t pen_x, pen_y, colour;

//         std::unordered_map<cvec2d_t, uint32_t>* vertices;

      public:
         Atlas* cache;

         uint32_t flags;

         std::unordered_map<char32_t, glyph_t> chartable;

         Font();
        ~Font();

         int Load(const char* file, unsigned int pt);
         int Precache(string chars);

//         void batch(const string& text);
         void Batch(const string& text, BaseRenderer* rndr);
         void Write(const string& text);

         void Move(uint32_t x, uint32_t y);
         void SetColour(uint32_t c);
   };
}

#endif


