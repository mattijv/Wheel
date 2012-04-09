#ifndef WHEEL_TYPES_HEAD
#define WHEEL_TYPES_HEAD

#include "include/utf8.h"

namespace wheel
{
   class string: public std::basic_string<char32_t>
   {
      public:
         string()
         {
            this->clear();
         }
         string( const string& s )
         {
            this->assign(s);
         }
         string( const char32_t* s )
         {
            this->assign(s);
         }
         string( const std::string& s )
         {
            string us32;
            utf8::utf8to32(s.begin(), s.end(), std::back_inserter(us32));
            this->assign(us32);
         }
         std::string u8_str() const
         {
            std::string u8l;
            utf8::utf32to8(this->begin(), this->end(), std::back_inserter(u8l));
            return u8l;
         }
   };
   struct rect_t
   {
      int32_t w, h, x, y;

      // Required for sorting
      bool operator<(const rect_t& r)
      {
         return w*h < r.w * r.h;
      }
      bool operator==(const rect_t& r)
      {
         return ((w == r.w) && (h == r.h) && (x == r.x) && (y == r.y));
      }
   };
   struct rectf_t
   {
      float tl, tr, bl, br;
   };
}
#endif
