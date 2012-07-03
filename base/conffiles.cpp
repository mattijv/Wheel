#include "configfiles.h"

namespace wheel
{
   uint32_t readconfig(string file, string section, optionlist_t)
   {
      std::fstream filestream;
   }
}

/*
ui32 readconfig( im::string file, im::string section, im::optlist* list )
{
   std::fstream fs;

   fs.open( file.c_str(), std::ios::in );
   if ( !fs.is_open() ) {
      return IM_ERR_FILE_ACCESS;
   }
   i32 read = 0;

   im::string line, name, value;

   list->clear();

   if ( section == u8"ALL" )
      read = 2;

   while( fs.good() )
   {
      getline( fs, line );
      if (*line.c_str() == '#')
         continue;

      if ( read == 1 )
         if (*line.c_str() == '[')
            break;

      if ( read == 2 )
         if (*line.c_str() == '[')
            continue;

      if ( read != 2 )
      {
         if ((line.find(u8"[") != im::string::npos) && (line.find(u8"]") != im::string::npos))
         {
            if ( !read )
               if (line.substr(line.find(u8"[")+1, line.find(u8"]") - line.find(u8"[") - 1) == section )
               {
                  read = 1;
                  continue;
               }
         }
      }
      if ( read )
      {
         name  = line.substr( 0, line.find(u8"=") );
         value = line.substr( line.find(u8"=")+1, im::string::npos );
         if (name == "") continue;

         list->insert( im::optlist::value_type( name, value ));
      }
   }

   if ( !read )
   {
      debugmsg("readconfig::im::err_file_invalid");
      fs.close();
      return IM_ERR_FILE_INVALID;
   }
   fs.close();
   return IM_SUCCESS;
}
*/
