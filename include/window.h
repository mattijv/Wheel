#ifndef WHEEL_WINDOW_HEAD
#define WHEEL_WINDOW_HEAD

#include "atlas.h"
#include "uirenderer.h"
#include "font.h"

#define WHEEL_GUI_REQUEST_REDRAW    0x01
#define WHEEL_TEXTURE_BLANK         0

namespace wheel
{
   class GUI;

   enum eventtype_t
   {
   };
   struct wevent_t
   {
      eventtype_t type;
   };
   struct gbox
   {
      rect_t metrics;
      rect_t texcoord;
   };

   class UIElement
   {
      protected:
         gbox                  object;
         std::list<UIElement*> children;

         GUI*  guiattachment;

      public:
         UIElement();

         uint32_t flags;
         int32_t  xpos, ypos;
         uint32_t colour;
         Atlas*   cache;

         virtual uint32_t render(UIRenderer* uir);
//         virtual  uint32_t OnEvent(const wevent_t& event) = 0;

         friend class GUI;
   };

   class Frame : public UIElement
   {
      private:
      public:
         void  setsize(uint32_t w, uint32_t h);
         Frame();
   };

   class GUI
   {
      private:
         Atlas       cache;
         UIRenderer  uir;

         std::list<UIElement*> uiobjects;
         std::unordered_map<uint32_t, gbox> textures;

      public:
         uint32_t    flags;

         std::vector<Font*> fontlist;

         void  render();
         void  reset()  {flags |= WHEEL_GUI_REQUEST_REDRAW; uir.reset();}

         int   addfont(const char*, uint32_t pt);
         int   addelement(UIElement*);

         void  puttext(uint32_t font, string text, uint32_t c = 0xffffffff, int32_t px = -1, int32_t py = -1);
   
         GUI(uint32_t w = 800, uint32_t h = 480, uint32_t csize = 512);
        ~GUI();
   };
}
   
#endif

