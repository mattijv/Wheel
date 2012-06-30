#include "uirenderer.h"

namespace wheel
{
   UIRenderer::UIRenderer()
   {
      scrwidth = 1024;
      scrheight = 640;
   }
   void UIRenderer::render()
   {
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glEnableClientState(GL_COLOR_ARRAY);

      glVertexPointer(2, GL_FLOAT, sizeof(vertexdata), &vertices[0].x);
      glTexCoordPointer(2, GL_FLOAT, sizeof(vertexdata), &vertices[0].u);
      glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertexdata), &vertices[0].rgba);
      glDrawArrays(GL_TRIANGLES, 0, vertices.size());
   }
   void UIRenderer::addrect(const rect_t& r, const rect_t& uv, const float& uiw, const float& uih, int off_x, int off_y, uint32_t colour)
   {
      vertexdata vd1,vd2,vd3,vd4;

      vd1.x = (r.x + r.w) + off_x;
      vd1.y = r.y + off_y;
      vd1.u = (uv.x + uv.w) / uiw;
      vd1.v = uv.y / uih;
      vd1.rgba = colour;

      vd2.x = r.x + off_x;
      vd2.y = r.y + off_y;
      vd2.u = uv.x / uiw;
      vd2.v = uv.y / uih;
      vd2.rgba = colour;

      vd3.x = r.x + off_x;
      vd3.y = (r.y - r.h) + off_y;
      vd3.u = uv.x / uiw;
      vd3.v = (uv.y + uv.h) / uih;
      vd3.rgba = colour;

      vd4.x = (r.x + r.w) + off_x;
      vd4.y = (r.y - r.h) + off_y;
      vd4.u = (uv.x + uv.w) / uiw;
      vd4.v = (uv.y + uv.h) / uih;
      vd4.rgba = colour;

      vertices.push_back( vd1 );
      vertices.push_back( vd2 );
      vertices.push_back( vd3 );
      vertices.push_back( vd1 );
      vertices.push_back( vd3 );
      vertices.push_back( vd4 );
   }
   void UIRenderer::reset()
   {
      vertices.clear();
   }
   void UIRenderer::setsize(uint32_t w, uint32_t h)
   {
      scrwidth = w;
      scrheight = h;
   }
}

