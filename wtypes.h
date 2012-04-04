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
}
#endif
