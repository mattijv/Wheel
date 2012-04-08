#ifndef WHEEL_ATLAS_HEAD
#define WHEEL_ATLAS_HEAD

#include <GL/glew.h>

#include <cstdint>
#include <list>
#include <vector>
#include <cassert>

#define WHEEL_CLEAN_AT_EXIT                  0x01

#define WHEEL_ATLAS_TEXTURE_NOT_INITIALISED  -1
#define WHEEL_ATLAS_FULL                     -2

namespace wheel
{
   enum atlas_fmt_t
   {
      WHEEL_ATLAS_FMT_BGRA = GL_BGRA,
      WHEEL_ATLAS_FMT_BGR  = GL_BGR,
      WHEEL_ATLAS_FMT_RGBA = GL_RGBA,
      WHEEL_ATLAS_FMT_RGB  = GL_RGB,
      WHEEL_ATLAS_FMT_LUM  = GL_LUMINANCE,
      WHEEL_ATLAS_FMT_ALPHA= GL_ALPHA
   };

   struct rect_t
   {
      int32_t w, h, x, y;

      // Required for sorting
      bool operator<(const rect_t& r)
      {
         return w*h < r.w * r.h;
      }
      bool operator==(const rect_t& r)
      {
         return ((w == r.w) && (h == r.h) && (x == r.x) && (y == r.y));
      }
   };
   struct rectf_t
   {
      float tl, tr, bl, br;
   };

   class Atlas
   {
      private:
         uint32_t       texid;
         atlas_fmt_t    internalformat;

         float          tex_w, tex_h;

         uint32_t       flags;

         int32_t        minsize;

         rect_t         Fit( uint32_t w, uint32_t h );

         void           Split( const rect_t& r );
         void           Prune( const rect_t& r );

         std::list<rect_t> unused;

      public:
         Atlas();
        ~Atlas();

         uint32_t       offset_x, offset_y, width, height;

         std::vector<rect_t> mapped;

         uint32_t       usrflags;

         uint32_t       Init( uint32_t w = 512, uint32_t h = 512);
         uint32_t       Init( uint32_t texid, uint32_t x, uint32_t y, uint32_t w, uint32_t h, float rw, float rh );
         int32_t        Add(uint32_t w, uint32_t h, const uint8_t* data);
         void           SetFormat( atlas_fmt_t fmt );

         rectf_t        GetTexCoord(int32_t which);

         void           Reset();
   };
}

#endif

