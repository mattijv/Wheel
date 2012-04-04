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

int main( int argc, char* argv[] )
{
   GLFWwindow win;
   glfwInit();
   win=glfwOpenWindow(512,512,GLFW_WINDOWED,"title",nullptr);
   glewInit();

   glPixelStorei(GL_PACK_ALIGNMENT, 1);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glEnable( GL_TEXTURE_2D );

   wheel::Font font;

   font.load("DroidSans.ttf", 32);

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

//   wheel::rect_t r_a = font.chartable[U'A'];
/*
   float s1 = r_a.x / 512.0f;
   float t1 = r_a.y / 512.0f;
   float s2 = (r_a.x+r_a.w) / 512.0f;
   float t2 = (r_a.y+r_a.h) / 512.0f;
*/
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

   font.drawtest();

   while(glfwIsWindow(win))
   {
      glfwPollEvents();
      glfwSwapBuffers();
   }
}

