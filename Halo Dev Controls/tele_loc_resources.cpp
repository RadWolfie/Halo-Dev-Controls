/********************************************************************************
	 -- Halo Dev Controls
    Copyright © 2011 Jesus7Freak

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************************
	File:    win_tele_resources.cpp
	Project: Halo Dev Controls and HDC
	Author:  Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#include "tele_loc_resources.h"
// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <windows.h>

/*
   locations.bin layout:
   
   DWORD num_of_maps
   wchar_t 1stmap_name[MAP_STR_SIZE]
      DWORD num_of_locations
      TELEPORT_LOCATION loc_site;
   wchar_t 2ndmap_name[MAP_STR_SIZE]
      DWORD num_of_locations
      TELEPORT_LOCATION loc_site;
   .
   .
   .
*/
void WriteLocationsToFile(wchar_t *FileName, std::vector<MAPS>* locations)
{
   DWORD size_in_bytes = locations->size();
   if (size_in_bytes)
   {
      HANDLE hFile;
      if ((hFile = CreateFileW(
         FileName, 
         GENERIC_WRITE, 
         FILE_SHARE_WRITE, 
         NULL,
         CREATE_ALWAYS,
         FILE_ATTRIBUTE_NORMAL,
         NULL)) != INVALID_HANDLE_VALUE)
      {
         DWORD num_of_maps = size_in_bytes;
         size_in_bytes *= (MAP_STR_SIZE * sizeof(wchar_t));
         size_in_bytes += sizeof(DWORD);//num of maps
         
         for (DWORD i = 0; i < num_of_maps; i++)
            size_in_bytes += ((*locations)[i].teleport_locations.size() * 
                              sizeof(TELEPORT_LOCATION)) + sizeof(DWORD);

         char *buffer = new char[size_in_bytes];
         
         *(DWORD*)buffer = num_of_maps;
         DWORD buffer_loc = sizeof(DWORD);
         
         for (DWORD i = 0; i < num_of_maps; i++)
         {
            wchar_t *buffer_map_wstr = (wchar_t*)&buffer[buffer_loc];
            wchar_t *mn_str = (*locations)[i].map_name;
            for (wchar_t wc_i = 0; wc_i < MAP_STR_SIZE; wc_i++)
               buffer_map_wstr[wc_i] = mn_str[wc_i];
            
            buffer_loc += sizeof(wchar_t) * MAP_STR_SIZE;

            DWORD num_of_locations = (*locations)[i].teleport_locations.size();
            *(DWORD*)&buffer[buffer_loc] = num_of_locations;
            buffer_loc += sizeof(DWORD);
            
            for (DWORD j = 0; j < num_of_locations; j++)
            {
               char *byte_p_ts = (char*)&(*locations)[i].teleport_locations[j];
               char *byte_p_buffer = (char*)&buffer[buffer_loc];
               for (int bi = 0; bi < sizeof(TELEPORT_LOCATION); bi++)
                  byte_p_buffer[bi] = byte_p_ts[bi];

               buffer_loc += sizeof(TELEPORT_LOCATION);
            }
         }
         
         DWORD written;
         if(!WriteFile(hFile, buffer, size_in_bytes, &written, NULL))
            DWORD dosretval = GetLastError();
            
         delete[] buffer;
         CloseHandle(hFile);
      }
   }
}

void GetLocationsFromFile(wchar_t *FileName, std::vector<MAPS>* locations)
{
   HANDLE hFile;
   if ((hFile = CreateFileW(
      FileName, 
      GENERIC_READ, 
      FILE_SHARE_READ, 
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL)) != INVALID_HANDLE_VALUE)
   {
      LARGE_INTEGER size = {0}; 
      GetFileSizeEx(hFile, &size);
      char *buffer = new char[size.LowPart];
      
      DWORD bytes_read;
      if (!ReadFile(hFile, buffer, size.LowPart, &bytes_read, NULL))
         DWORD dosretval = GetLastError();
      
      DWORD num_of_maps = *(DWORD*)buffer;
      DWORD buffer_loc = sizeof(DWORD);
      
      MAPS map_site; TELEPORT_LOCATION tele_site;
      for (DWORD i = 0; i < num_of_maps; i++)
      {
         wchar_t *buffer_map_wstr = (wchar_t*)&buffer[buffer_loc];
         wchar_t *mn_str = map_site.map_name;
         for (wchar_t wc_i = 0; wc_i < MAP_STR_SIZE; wc_i++)
            mn_str[wc_i] = buffer_map_wstr[wc_i];
         
         buffer_loc += sizeof(wchar_t) * MAP_STR_SIZE;

         DWORD num_of_locations = (DWORD)buffer[buffer_loc];
         buffer_loc += sizeof(DWORD);
         
         for (DWORD j = 0; j < num_of_locations; j++)
         {
            char *byte_p_ts = (char*)&tele_site;
            char *byte_p_buffer = (char*)&buffer[buffer_loc];
            for (int bi = 0; bi < sizeof(TELEPORT_LOCATION); bi++)
               byte_p_ts[bi] = byte_p_buffer[bi];

            buffer_loc += sizeof(TELEPORT_LOCATION);

            map_site.teleport_locations.push_back(tele_site);
         }
         
         locations->push_back(map_site);
         map_site.teleport_locations.clear();
      }
      
      delete[] buffer;
      CloseHandle(hFile);
   }
}

//requires #include <fstream>
/*void WriteLocationsToFile(wchar_t *FileName, std::vector<MAPS>* locations)
{
   if (locations->size())
   {
      //System::String^ sFileName = gcnew System::String(LocationsFileName);
      unsigned int num_of_maps = locations->size(), num_of_locations = 0;
      
      //unhide file to make changes
      //if (System::IO::File::Exists(sFileName))
      //   System::IO::File::SetAttributes(sFileName, System::IO::File::GetAttributes(sFileName) & ~System::IO::FileAttributes::Hidden);
         
      std::fstream File;//(FileName, std::ios::out | std::ios::binary);
      File.open(FileName, std::ios::out | std::ios::binary);
      //save settings changes to file
      File.write(reinterpret_cast<char*>(&num_of_maps), sizeof(unsigned int));
      
      for (unsigned int i = 0; i < num_of_maps; i++)
      {
         File.write(reinterpret_cast<char*>(&(*locations)[i].map_name), sizeof(wchar_t) * MAP_STR_SIZE);
         
         num_of_locations = (*locations)[i].teleport_locations.size();
         File.write(reinterpret_cast<char*>(&num_of_locations), sizeof(unsigned int));
         for (unsigned int j = 0; j < num_of_locations; j++)
            File.write(reinterpret_cast<char*>(&(*locations)[i].teleport_locations[j]), sizeof(TELEPORT_LOCATION));
      }
      
      File.close();
      
      //hide file
      //System::IO::File::SetAttributes(sFileName, System::IO::File::GetAttributes(sFileName) | System::IO::FileAttributes::Hidden);
      //delete sFileName;
   }
}

void GetLocationsFromFile(wchar_t *FileName, std::vector<MAPS>* locations)
{
   MAPS map_site; TELEPORT_LOCATION tele_site = {0};
   unsigned int num_of_maps = 0, num_of_locations = 0;
   
   std::fstream File;//(FileName, std::ios::in | std::ios::binary);
   File.open(FileName, std::ios::in | std::ios::binary);
   if (!File.fail())
   {
      File.read(reinterpret_cast<char*>(&num_of_maps), sizeof(unsigned int));
      
      for (unsigned int i = 0; i < num_of_maps; i++)
      {
         File.read(reinterpret_cast<char*>(&map_site.map_name), sizeof(wchar_t) * MAP_STR_SIZE);
         
         File.read(reinterpret_cast<char*>(&num_of_locations), sizeof(unsigned int));
         for (unsigned int j = 0; j < num_of_locations; j++)
         {
            File.read(reinterpret_cast<char*>(&tele_site), sizeof(TELEPORT_LOCATION));
            map_site.teleport_locations.push_back(tele_site);
         }
         
         locations->push_back(map_site);
         map_site.teleport_locations.clear();
      }
      File.close();
   }
}*/

bool FindMapIndex(std::vector<MAPS>* map_sites, char* map_name, unsigned int &map_index)
{
   bool map_found = false; 
   
   unsigned int size = map_sites->size();
   for (unsigned int map_i = 0; map_i < size && !map_found; map_i++)
   {
      wchar_t *str = (wchar_t*)((*map_sites)[map_i].map_name);
      for (int j = 0; j <= 22; j++)
      {
         if (map_name[j] != (char)(str[j]))//there shouldn't be any unicode characters anyways
            break;
                                 
         if (j == 22 || map_name[j] == 0 && (*map_sites)[map_i].map_name[j] == 0)
         {
            map_found = true;
            map_index = map_i;
            break;
         }
      }
   }
   return map_found;
}

bool FindTeleLocNameIndex(std::vector<TELEPORT_LOCATION>* tele_sites, wchar_t* TeleLocName_name, unsigned int &TeleLocName_index)
{
   bool tele_loc_found = false; 
   
   unsigned int size = tele_sites->size();
   for (unsigned int tele_i = 0; tele_i < size && !tele_loc_found; tele_i++)
   {
      wchar_t *str = (wchar_t*)((*tele_sites)[tele_i].teleport_loc_name);
      for (int j = 0; j <= 22; j++)
      {
         if (TeleLocName_name[j] != str[j])
            break;
                                 
         if (j == 22 || TeleLocName_name[j] == 0 && (*tele_sites)[tele_i].teleport_loc_name[j] == 0)
         {
            tele_loc_found = true;
            TeleLocName_index = tele_i;
            break;
         }
      }
   }
   return tele_loc_found;
}