#ifndef WHEEL_TYPES_HEAD
#define WHEEL_TYPES_HEAD

#include "utf8.h"

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
/*
         string( std::string&& s )
         {
         }
         string( string&& s )
         {
         }
*/
         string( const char* s )
         {
            std::string s2(s);
            string s3(s2);
            this->assign(s3);
         }
         std::string u8_str() const
         {
            std::string u8l;
            utf8::utf32to8(this->begin(), this->end(), std::back_inserter(u8l));
            return u8l;
         }
         uint32_t to_uint32() const
         {
            return strtoul(this->u8_str().c_str(), nullptr, 0);
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

#endif
