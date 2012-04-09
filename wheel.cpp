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

