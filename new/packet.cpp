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

template <>
struct is_string<wheel::string>
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


         posmarker_t toc;

      public:
         Packet();

         std::deque<uint8_t>  data;

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
         typename std::enable_if<is_string<T>::value, wheel::string>::type
         Get()
         {
            std::string s;

            uint32_t size  = Get<uint32_t>();
            uint32_t count = Get<uint32_t>();

            if (size != sizeof(char))
               return "";

            for (uint32_t i = 0; i < count; ++i)
            {
               s.push_back(Get<char>());
            }

            return (T) s.c_str();
         }
         template<typename T>
         typename std::enable_if<is_pointer<T>::value, void>::type
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

            Put((uint32_t)sizeof(char));
            Put((uint32_t)str.u8_str().size());

            for(char c : str.u8_str())
            {
               Put(c);
            }

            return;
         }
         template<typename T>
         typename std::enable_if<is_pointer<T>::value, void>::type
         Put (T value, uint32_t count, uint32_t esize)
         {
            Put(esize);
            Put(count);
            std::cout << count << " elements of size " << esize << "\n";
            for (uint32_t i = 0; i < count; ++i)
               Put(value[i]);

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
         typename std::enable_if<!is_pointer<T>::value, void>::type
         Pack(wheel::string key, T value)
         {
            toc.insert(posmarker_t::value_type(key, data.size()));
            Put(value);
         }

         template<typename T>
         typename std::enable_if<is_pointer<T>::value, void>::type
         Pack(wheel::string key, T value, uint32_t count, uint32_t esize)
         {
            toc.insert(posmarker_t::value_type(key, data.size()));
            Put(value, count, esize);
         }

         template<typename T>
         T Extract(wheel::string key)
         {
            T r;
            if (!toc.count(key))
               return r;

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

   Packet::WriteJSON(const char* file)
   {
      ofstream out;
      out.open(file);
      out.close();
   }
}

int main(void)
{
   wheel::Packet pkt;

   wheel::string a, b;

   pkt.Pack("name", "smith", 5, 1);

   pkt.Dump("test.dump");
   b = pkt.Extract<wheel::string>("name");

   std::cout << b.u8_str() << "\n";
}

#endif
