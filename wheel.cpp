/*
   g++ -std=c++0x `freetype-config --cflags --libs` *.cpp -lGL -lglfw -lGLEW
*/
#include <iostream>
#include <map>

#include "atlas.h"
#include "wtypes.h"
#include "font.h"

#include <GL/glfw3.h>

namespace wheel
{
   int isp2 (unsigned int x)
   {
      return ((x != 0) && ((x & (~x + 1)) == x));
   }
}

namespace wheel
{
   struct cvec2d_t
   {
      uint32_t x, y;
   };
}

namespace std
{
   template<>
   struct hash<typename wheel::cvec2d_t>// : public unary_function<wheel::cvec2d_t, size_t>
   {
      public:
         size_t operator()(const wheel::cvec2d_t& x) const
         {
            size_t l = std::hash<uint32_t>()(x.x);
            size_t r = std::hash<uint32_t>()(x.y);

            return l ^ ~r;
         }
   };
}

namespace wheel
{
/*
   struct vertexdata
   {
      float x, y,
            u, v;
   };

   class UIRenderer
   {
      private:
         uint32_t scrwidth, scrheight;
         std::vector<vertexdata> vertices;

      public:
         UIRenderer();

         void update();
         void render();
         void addrect(const rect_t& r, const rect_t& uv, const float& w, const float& h);
         void reset();
   };
*/
}

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
}
int main( int argc, char* argv[] )
{
   GLFWwindow win;
   glfwInit();
   win=glfwOpenWindow(1024,640,GLFW_WINDOWED,"title",nullptr);
   glewInit();

   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glEnable( GL_TEXTURE_2D );

   glMatrixMode(GL_PROJECTION);
   glOrtho(0,1024,0,640,-1,1);
   glMatrixMode(GL_MODELVIEW);

   wheel::Font font, font2;

   font.load("DroidSans.ttf", 32);
   font2.cache = font.cache;
   font2.load("DroidSans.ttf", 8);

   wheel::UIRenderer uir;

   font.batchr(U"The quick brown", &uir);
   font.setcolour(0xff00ffff);
   font.batchr(U" fox", &uir);
   font.setcolour(0xffffffff);
   font.batchr(U" jumped", &uir);

   uir.render();

   while(glfwIsWindow(win))
   {
      glfwPollEvents();
      glfwSwapBuffers();
   }
}

