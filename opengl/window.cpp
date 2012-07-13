#include "window.h"

namespace wheel
{
   void Frame::setsize(uint32_t w, uint32_t h)
   {
   }
   Frame::Frame()
   {
      
   }


   UIElement::UIElement()
   {
      guiattachment = nullptr;
   }
   uint32_t UIElement::render(UIRenderer* uir)
   {
      uir->addrect(object.metrics, object.texcoord, cache->width, cache->height, xpos, ypos, colour);
      for (UIElement* e : children)
         e->render(uir);
      return 0;
   }

   GUI::GUI(uint32_t w, uint32_t h, uint32_t csize)
   {
      uir.setsize(w,h);
      cache.Init(csize, csize);
      cache.SetFormat(WHEEL_ATLAS_FMT_ALPHA);
      cache.Reset();

      flags = WHEEL_GUI_REQUEST_REDRAW;

      uint8_t block[8][8];
      for (uint32_t i=0;i<8;++i) for (uint32_t j=0;j<8;++j)
         block[i][j] = 0xff;

      int rv = cache.Add(8,8, (uint8_t*)block);
      if (rv < 0)
         exit(1);
      else
      {
         gbox nbox;
         nbox.texcoord = cache.mapped[rv];
         nbox.metrics.x = 0;
         nbox.metrics.y = 0;
         nbox.metrics.w = 8;
         nbox.metrics.h = 8;

         textures.insert(std::pair<uint32_t, gbox>(WHEEL_TEXTURE_BLANK, nbox));
      }
   }
   GUI::~GUI()
   {
      for ( Font* f : fontlist )
         delete f;
   }
   void GUI::render()
   {
      if (flags & WHEEL_GUI_REQUEST_REDRAW)
      {
         for (auto elem : uiobjects)
         {
            elem->render(&uir);
         }
      }
      flags &=  ~WHEEL_GUI_REQUEST_REDRAW;
      uir.render();
   }
   int GUI::addfont(const char* file, uint32_t pt)
   {
      wheel::Font* font = new Font;
      font->cache = &cache;
      font->Load(file, pt);
      fontlist.push_back(font);
      return fontlist.size()-1;
   }
   int GUI::addelement(UIElement* uie)
   {
      uie->guiattachment = this;
      uie->cache = &cache;
      uiobjects.push_back(uie);
      return uiobjects.size()-1;
   }
   void GUI::puttext(uint32_t font, string text, uint32_t c, int32_t px, int32_t py)
   {
      if (!( flags & WHEEL_GUI_REQUEST_REDRAW ))
         return;

      if ((px > 0) && (py > 0)) fontlist[font]->Move(px, py);
      fontlist[font]->SetColour(c);
      fontlist[font]->Batchr(text, &uir);
   }
}

