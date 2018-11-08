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
    File:    tele_loc_resources.h
    Project: Halo Dev Controls and HDC
    Author:  Jesus7Freak
    Date:    11/22/2011
    Game:    Halo and Halo Custom Edition
    Version: all
*********************************************************************************/
#pragma once
#include <vector>

#define MAP_STR_SIZE 24
#define TELE_LOC_NAME_SIZE 12

struct TELEPORT_LOCATION
{
   wchar_t teleport_loc_name[TELE_LOC_NAME_SIZE];
   float coordinates[3];
};

struct MAPS
{
   wchar_t map_name[MAP_STR_SIZE];
   std::vector<TELEPORT_LOCATION> teleport_locations;
};

void WriteLocationsToFile(wchar_t *FileName, std::vector<MAPS>* locations);
void GetLocationsFromFile(wchar_t *FileName, std::vector<MAPS>* locations);
bool FindMapIndex(std::vector<MAPS>* map_sites, char* map_name, unsigned int &map_index);
bool FindTeleLocNameIndex(std::vector<TELEPORT_LOCATION>* tele_sites, wchar_t* TeleLocName_name, unsigned int &TeleLocName_index);