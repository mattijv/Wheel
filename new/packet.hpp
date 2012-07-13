#ifndef WHEEL_PACKET_HEADER
#define WHEEL_PACKET_HEADER

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <deque>
#include <fstream>

#include "wtypes.h"
#include "common.h"

using std::is_array;
using std::is_arithmetic;
using std::is_pointer;

template <typename T>
struct is_string
{
   enum {value = false};
};

template <typename charT, typename traits, typename Alloc>
struct is_string<std::basic_string<charT, traits, Alloc>>
{
   enum {value = true};
};

namespace wheel
{
   typedef std::unordered_map<wheel::string, size_t> posmarker_t;

   inline void endian_swap(uint8_t* value, size_t valuesize)
   {
      std::reverse(value, value+valuesize);
   }

   class Packet
   {
      private:
         static uint32_t      cflags;
         size_t               packetptr;

         std::deque<uint8_t>  data;

         posmarker_t toc;

      public:
         Packet();

         uint32_t WriteXML(const char* file);
         uint32_t WriteJSON(const char* file);
         uint32_t WriteWDF(const char* file);

         template<typename T>
         typename std::enable_if<is_arithmetic<T>::value, T>::type
         Get()
         {
            if (packetptr + sizeof(T) > data.size())
               return 0;
            T* ptr = (T*)&data[packetptr];
            packetptr += sizeof(T);

            T retval = *ptr;

            if (cflags & WHEEL_LITTLE_ENDIAN)
               endian_swap((uint8_t*)&retval, sizeof(retval));

            return retval;
         }
         template<typename T>
         typename std::enable_if<is_string<T>::value, void>::type
         Get()
         {
            std::cout << "string!" << "\n";
            return "";
         }
         template<typename T>
         typename std::enable_if<is_array<T>::value, void>::type
         Get()
         {
            std::cout << "array!" << "\n";
            return nullptr;
         }

         template<typename T>
         typename std::enable_if<is_arithmetic<T>::value, void>::type
         Put(T value)
         {
            if (cflags & WHEEL_LITTLE_ENDIAN)
               endian_swap((uint8_t*)&value, sizeof(value));

            for (size_t i = 0; i < sizeof(value); ++i)
            {
               uint8_t* valueptr = (uint8_t*)&value;
               data.push_back(*(valueptr+i));
            }
            return;
         }
         template<typename T>
         typename std::enable_if<is_string<T>::value, void>::type
         Put(T value)
         {
            wheel::string str = value;
            std::cout << str.u8_str() << " length: " << str.size() << "\n";
            return;
         }
         template<typename T>
         typename std::enable_if<is_array<T>::value || is_pointer<T>::value, void>::type
         Put (T value)
         {
            std::cout << "array : " << std::extent<T>::value << "\n";
            return;
         }

         void Dump(const char* file)
         {
            std::ofstream of;
            of.open(file, std::ios::binary );
            for(uint8_t ui8 : data)
               of.write((const char*)&ui8, sizeof(uint8_t));
            of.close();
         }
         bool Seek(size_t pos)
         {
            if (data.size() < pos) return false;
            packetptr = pos;
            return true;
         }

         template<typename T>
         void Pack(wheel::string key, T value)
         {
            toc.insert(posmarker_t::value_type(key, data.size()));
            Put(value);
         }
         template<typename T>
         T Extract(wheel::string key)
         {
            if (!toc.count(key))
               return 0;
            this->Seek(toc[key]);
            return this->Get<T>();
         }
   };
   uint32_t Packet::cflags = 0;

   Packet::Packet()
   {
      packetptr = 0;

      if (!(cflags & WHEEL_INITIALISED))
      {
         uint16_t test = 0xbe1e;
         if ( (uint8_t)*(&test) == 0x1e )
            cflags |= WHEEL_LITTLE_ENDIAN;
         cflags |= WHEEL_INITIALISED;
      }
   }
}

int main(void)
{
   wheel::Packet pkt;

   std::string str = "omfg!";
   int a[3];
   pkt.Pack<int[3]>("lol", a);
}

#endif
