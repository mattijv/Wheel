/*
   gcc -std=c++0x -c atlas.cpp
*/
#include "atlas.h"
#include <iostream>

namespace wheel
{

   bool rect_overlap(const rect_t& r1, const rect_t& r2)
   {
      int b1,b2,rg1,rg2;

      b1 = r1.y + r1.h;
      b2 = r2.y + r2.h;

      rg1 = r1.x + r1.w;
      rg2 = r2.x + r2.w;

      if (( b1 > r2.y ) && ( r1.y < b2 ) && ( rg1 > r2.x ) && ( rg2 > r1.x ))
         return true;

      return false;
   }
   bool rect_iscontained(const rect_t& r1, const rect_t& r2)
   {
      if ((r1.x >= r2.x) && (r1.x + r1.w <= r2.x + r2.w) && (r1.y >= r2.y) && (r1.y + r1.h <= r2.y + r2.h))
         return true;

      return false;
   }

   Atlas::Atlas()
   {
      texid = 0;
      offset_x = 0; offset_y = 0; width = 0; height = 0;
      flags = 0;
      minsize = 4;
      usrflags = 0;
      internalformat = WHEEL_ATLAS_FMT_BGRA;
   };

   void Atlas::SetFormat( atlas_fmt_t fmt )
   {
      internalformat = fmt;
   }

   void Atlas::Split( const rect_t& r )
   {
      rect_t temp;

      for( auto r2 : unused )
      {
         if (rect_overlap( r, r2 ))
         {
            if (r.x < r2.x + r2.w && r.x + r.w > r2.x)
            {
               if (r.y > r2.y && r.y < r2.y + r2.h)
               {
                  temp = r2;
                  temp.h = r.y - r2.y;
                  if (temp.h > minsize)
                     unused.push_front(temp);
               }
               if (r.y + r.h < r2.y + r2.h)
               {
                  temp = r2;
                  temp.y = r.y + r.h;
                  temp.h = r2.y + r2.h - (r.y + r.h);
                  if (temp.h > minsize)
                     unused.push_front(temp);
               }
            }
            if (r.y < r2.y + r2.h && r.y + r.h > r2.y)
            {
               if (r.x > r2.x && r.x < r2.x + r2.w)
               {
                  temp = r2;
                  temp.w = r.x - r2.x;
                  if (temp.w > minsize)
                     unused.push_front(temp);
               }
               if (r.x + r.w < r2.x + r2.w)
               {
                  temp = r2;
                  temp.x = r.x + r.w;
                  temp.w = r2.x + r2.w - (r.x + r.w);
                  if (temp.w > minsize)
                     unused.push_front(temp);
               }
            }
         }
      }
   }

   rect_t Atlas::Fit( uint32_t w, uint32_t h )
   {
      rect_t rval;
      rval.w = 0;
      rval.h = 0;

      unused.sort();

      for (auto it : unused)
      {
         if (( (int32_t)w < it.w ) && ( (int32_t)h < it.h ))
         {
            rval.x = it.x;
            rval.y = it.y;
            rval.w = w;
            rval.h = h;

            Split(rval);

            return rval;
         }
      }
      return rval;
   }

   void Atlas::Reset()
   {
      unused.clear();
      rect_t r;
      r.x = 0;
      r.y = 0;
      r.w = width;
      r.h = height;
      usrflags = 0;

      unused.push_back(r);

      glBindTexture( GL_TEXTURE_2D, texid );

      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      glTexImage2D( GL_TEXTURE_2D, 0, internalformat, tex_w, tex_h, 0, internalformat, GL_UNSIGNED_BYTE, 0 );
   }

   void Atlas::Prune( const rect_t& r )
   {
      for (auto it = unused.begin(); it!=unused.end(); it++ )
      {
         if (rect_overlap(r, *it))
         {
            it = unused.erase(it);
            it--;
         }
      }
      for (auto i = unused.begin(); i!=unused.end(); ++i )
         for (auto j = i; j != unused.end(); ++j)
         {
            if ( i == j ) continue;
            if (rect_iscontained(*i, *j))
            {
               i = unused.erase(i);
               i--;
               break;
            }
            if (rect_iscontained(*j, *i))
            {
               j = unused.erase(j);
               j--;
            }
         }
   }

   /*
      These are OpenGL-dependant
   */
   Atlas::~Atlas()
   {
      if ( flags & WHEEL_CLEAN_AT_EXIT )
         glDeleteBuffers( 1, &texid );
   }

   uint32_t Atlas::Init( uint32_t w, uint32_t h )
   {
      assert(glGenBuffers && "Init GLEW or use gl3.h");
      glGenBuffers( 1, &texid );

      flags |= WHEEL_CLEAN_AT_EXIT;
      SetFormat(WHEEL_ATLAS_FMT_BGRA);

      width = w; height = h;

      tex_w = width;
      tex_h = height;

      Reset();
      return 0;
   }

   uint32_t Atlas::Init( uint32_t id, uint32_t x, uint32_t y, uint32_t w, uint32_t h, float rw, float rh)
   {
      texid = id;
      offset_x = x;
      offset_y = y;
      width = w;
      height = h;

      tex_w = width;
      tex_h = height;

      Reset();
      return 0;
   }

   int32_t Atlas::Add(uint32_t w, uint32_t h, const uint8_t* data)
   {
      if (texid == 0) return WHEEL_ATLAS_TEXTURE_NOT_INITIALISED;

      rect_t dest = Fit(w, h);

      // This test doesn't work for non-existant characters (' ')
      if ((dest.w == 0) || (dest.h == 0))
      {
         return WHEEL_ATLAS_FULL;
      }

      Prune(dest);

      glPixelStorei(GL_PACK_ALIGNMENT, 1);

      glBindTexture( GL_TEXTURE_2D, texid );
      glTexSubImage2D(GL_TEXTURE_2D, 0, dest.x, dest.y, w, h, internalformat, GL_UNSIGNED_BYTE, data);

      mapped.push_back(dest);

      return mapped.size()-1;
   }

   rectf_t Atlas::GetTexCoord(int32_t w)
   {
      rectf_t rval;

      rval.tl = (float)mapped[w].x / tex_w;
      rval.tr = (float)(mapped[w].x + mapped[w].w) / tex_w;
      rval.bl = (float)mapped[w].y / tex_h;
      rval.br = (float)(mapped[w].y + mapped[w].h) / tex_h;

      return rval;
   }
}

