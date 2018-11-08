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
    File:    KeyLib.h
    Project: Halo Dev Controls
    Author:  Jesus7Freak
    Date:    11/22/2011
    Game:    Halo and Halo Custom Edition
    Version: all
*********************************************************************************/
#pragma once

#define NUM_OF_KEYS 119

namespace KeyLib
{
   struct KEY_INFO
   {
      short key_code;
      wchar_t* key_name;
   };

   extern wchar_t *szUknownKey;
   extern KEY_INFO key_info_list[NUM_OF_KEYS];

   wchar_t* Get_Key_Name(unsigned int keycode);
}