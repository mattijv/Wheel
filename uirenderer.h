#ifndef WHEEL_UIRENDERER_HEAD
#define WHEEL_UIRENDERER_HEAD

#include <GL/glew.h>
#include <cstdint>
#include <vector>

#include "wtypes.h"

namespace wheel
{
   struct vertexdata
   {
      float    x, y,
               u, v;
      uint32_t rgba;
      uint32_t padding[3];
   };

   class UIRenderer
   {
      private:
         uint32_t scrwidth, scrheight;
         std::vector<vertexdata> vertices;

      public:
         UIRenderer();

         uint32_t flags;

         void update();
         void render();
         void addrect(const rect_t& r, const rect_t& uv, const float& w, const float& h, int off_x = 0, int off_y = 0, uint32_t colour = 0xffffffff);
         void reset();
   };
}

#endif

