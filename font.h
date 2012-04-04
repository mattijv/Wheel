#ifndef WHEEL_FONT_HEAD
#define WHEEL_FONT_HEAD

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

#include "atlas.h"
#include "wtypes.h"

namespace wheel
{
   class Font
   {
      private:
         static int fcount;
         static FT_Library ftlib;

         uint32_t BaseInit();

         FT_Face face;
         uint32_t ptsize;

         Atlas* cache;

      public:
         uint32_t flags;

         std::unordered_map<char32_t, rect_t> chartable;

         Font();
        ~Font();

         int load(const char* file, unsigned int pt);
         int drawtest();
         int precache(string chars);
         
   };
}

#endif

