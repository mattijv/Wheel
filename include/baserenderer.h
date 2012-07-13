#ifndef WHEEL_BASERENDERER_HEAD
#define WHEEL_BASERENDERER_HEAD

#include "platform-gl.h"

#include <cstdint>
#include <vector>

#include "wtypes.h"

namespace wheel
{
   struct vertex_t
   {
      float    x, y;
      float    u, v;
      uint32_t rgba;
      uint32_t padding[3];
   };

   class BaseRenderer
   {
      private:
         uint32_t scrwidth, scrheight;
         std::vector<vertex_t> vertices;
      public:
         BaseRenderer();

         uint32_t flags;

         void Update();
         void Render();
         void AddRect(const rect_t& r, const rect_t& uv, float w, float h,
                      float ox = 0, float oy = 0, uint32_t c = 0xffffffff);

         void Reset();
         void SetSize(uint32_t w, uint32_t h);
   };
}

#endif
