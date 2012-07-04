#include "configfiles.h"
#include "werror.h"

namespace wheel
{
   uint32_t readconfig(string file, string section, optionlist_t* list)
   {
      std::fstream filestream;

      filestream.open(filestream.c_str(), std::ios::in);
      if (!filestream.is_open())
      {
         return WHEEL_CONFIG_FILE_ACCESS;
      }

      uint32_t read = 0;
      string line, name, value;

      list->clear();

      if (section == u8"ALL")
         read = 2;

      while(filestream.good())
      {
         getline(filestream, line);

         if ((*line.c_str() == '#') || (*line.c_str() == ';'))
            continue;

         if (read == 1)
         {
            if (*line.c_str() == '[')
               break;
         }
         else if (read == 2)
         {
            if (*line.c_str() == '[')
               continue;
         }
         if (read != 2)
         {
            if ((line.find(u8"[") != string::npos) && (line.find(u8"]") != string::npos))
            {
               if (!read)
                  if (line.substr(line.find(u8"[")+1, line.find(u8"]") - line.find(u8"[") - 1) == section)
                  {
                     read = 1;
                     continue;
                  }
            }
         }
         if (read)
         {
            name  = line.substr(0, line.find(u8"="));
            value = line.substr(line.find(u8"="+1, string::npos);
            if (name.empty()) continue;

            list->insert(optionlist_t::value_type(name,value));
         }
      }
      if (!read)
      {
         filestream.close();
         return WHEEL_CONFIG_INVALID_FILE;
      }
   }
   filestream.close();
   return WHEEL_OK;
}
