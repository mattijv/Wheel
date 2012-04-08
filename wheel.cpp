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
   struct vertexdata
   {
      float xpos, ypos, red, green, blue, s0, s1, t0, t1;
   };

   class UIRenderer
   {
      private:
         uint32_t scrwidth, scrheight;
         std::vector<uint32_t> vectices;

      public:
         std::unordered_map<cvec2d_t, uint32_t> vert;
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

   wheel::Font font, font2;

   font.load("DroidSans.ttf", 32);
   font2.cache = font.cache;
   font2.load("DroidSans.ttf", 8);

   glDisable( GL_TEXTURE_2D );

   glBegin( GL_QUADS );
      glColor4f(1.0f,0.0f,0.0f,1.0f);
      glVertex2f(-1.0f, -1.0f);
      glColor4f(1.0f,1.0f,1.0f,1.0f);
      glVertex2f( 1.0f, -1.0f);
      glColor4f(1.0f,0.0f,1.0f,1.0f);
      glVertex2f( 1.0f,  1.0f);
      glColor4f(1.0f,1.0f,1.0f,1.0f);
      glVertex2f(-1.0f,  1.0f);
   glEnd();

   glEnable( GL_TEXTURE_2D );

   glColor4f(0.0,0.,0.,0.3);

   float s1 = 0.0f;
   float t1 = 0.0f;
   float s2 = 1.0f;
   float t2 = 1.0f;

   glBegin( GL_QUADS );
      glTexCoord2f( s1, t2 );
      glVertex2f(-1.0f, -1.0f);
      glTexCoord2f( s2, t2 );
      glVertex2f( 1.0f, -1.0f);
      glTexCoord2f( s2, t1 );
      glVertex2f( 1.0f,  1.0f);
      glTexCoord2f( s1, t1 );
      glVertex2f(-1.0f,  1.0f);
   glEnd();
//*/
   font.write(U"omfg?");

   while(glfwIsWindow(win))
   {
      glfwPollEvents();
      glfwSwapBuffers();
   }
}

