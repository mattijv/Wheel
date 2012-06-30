/*
   g++ -std=c++0x `freetype-config --cflags --libs` *.cpp -lGL -lglfw -lGLEW
*/
#include <iostream>
#include <map>

#include "atlas.h"
#include "wtypes.h"
#include "font.h"
#include "window.h"

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
   win=glfwOpenWindow(1024,640,GLFW_WINDOWED,"title",nullptr);
   glewInit();

   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
   glEnable( GL_TEXTURE_2D );

   glMatrixMode(GL_PROJECTION);
   glOrtho(0,1024,0,640,-1,1);
   glMatrixMode(GL_MODELVIEW);

   //-----------------------------------------------------------------------

   wheel::GUI gui(1024,640,512);

   int droidsans = gui.addfont("DroidSans.ttf", 16);

   double timer;
   uint32_t frames = 0;
   wheel::string fpsmsg;

   wheel::Frame window;

   window.setsize(20, 20);

   int mainwin = gui.addelement(&window);

   //-----------------------------------------------------------------------

   while(glfwIsWindow(win))
   {
      glClear( GL_COLOR_BUFFER_BIT );

      gui.puttext(droidsans, fpsmsg, 0xffffffff, 10, 10);
      gui.render();

      if (glfwGetTime() > timer + 1)
      {
         timer = glfwGetTime();
         char fpsmsgc[32];
         sprintf(fpsmsgc, "fps: %d", frames);
         fpsmsg = fpsmsgc;
         gui.reset();
         frames = 0;
      }
      glfwPollEvents();
      glfwSwapBuffers();
      frames++;
   }
}

