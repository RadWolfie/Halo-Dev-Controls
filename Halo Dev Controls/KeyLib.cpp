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
	File:    KeyLib.cpp
	Project: Halo Dev Controls
	Author:  Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#include "KeyLib.h"

#define KEY_INFO_LIST_SIZE 223

namespace KeyLib
{
   wchar_t *szUknownKey = L"Uknown";
   
   KEY_INFO key_info_list[NUM_OF_KEYS] = 
   {
      {0,L"none"},
      {1,L"Left Mouse Button"},
      {2,L"Right Mouse Button"},
      {3,L"Cancel"},
      {4,L"Middle Mouse Button"},
      {5,L"X1 Mouse Button"},
      {6,L"X2 Mouse Button"},
      {8,L"BackSpace"},
      {9,L"Tab"},
      {12,L"Clear"},
      {13,L"Enter"},
      {16,L"Shift"},
      {17,L"Control"},
      {18,L"Alt"},
      {19,L"Pause"},
      {20,L"Caps Lock"},
      {27,L"Escape"},
      {32,L"Space Bar"},
      {33,L"Page Up"},
      {34,L"Page Down"},
      {35,L"End"},
      {36,L"Home"},
      {37,L"Left Arrow"},
      {38,L"Up Arrow"},
      {39,L"Right Arrow"},
      {40,L"Down Arrow"},
      {42,L"Print"},
      {44,L"Print Screen"},
      {45,L"Insert"},
      {46,L"Delete"},
      {47,L"Help"},
      {48,L"0"},
      {49,L"1"},
      {50,L"2"},
      {51,L"3"},
      {52,L"4"},
      {53,L"5"},
      {54,L"6"},
      {55,L"7"},
      {56,L"8"},
      {57,L"9"},
      {65,L"A"},
      {66,L"B"},
      {67,L"C"},
      {68,L"D"},
      {69,L"E"},
      {70,L"F"},
      {71,L"G"},
      {72,L"H"},
      {73,L"I"},
      {74,L"J"},
      {75,L"K"},
      {76,L"L"},
      {77,L"M"},
      {78,L"N"},
      {79,L"O"},
      {80,L"P"},
      {81,L"Q"},
      {82,L"R"},
      {83,L"S"},
      {84,L"T"},
      {85,L"U"},
      {86,L"V"},
      {87,L"W"},
      {88,L"X"},
      {89,L"Y"},
      {90,L"Z"},
      {96,L"NumPad 0"},
      {97,L"NumPad 1"},
      {98,L"NumPad 2"},
      {99,L"NumPad 3"},
      {100,L"NumPad 4"},
      {101,L"NumPad 5"},
      {102,L"NumPad 6"},
      {103,L"NumPad 7"},
      {104,L"NumPad 8"},
      {105,L"NumPad 9"},
      {106,L"NumPad *"},
      {107,L"NumPad +"},
      {109,L"NumPad -"},
      {110,L"NumPad ."},
      {111,L"NumPad /"},
      {112,L"F1"},
      {113,L"F2"},
      {114,L"F3"},
      {115,L"F4"},
      {116,L"F5"},
      {117,L"F6"},
      {118,L"F7"},
      {119,L"F8"},
      {120,L"F9"},
      {121,L"F10"},
      {122,L"F11"},
      {123,L"F12"},
      {124,L"F13"},
      {125,L"F14"},
      {126,L"F15"},
      {127,L"F16"},
      {128,L"F17"},
      {129,L"F18"},
      {130,L"F19"},
      {131,L"F20"},
      {132,L"F21"},
      {133,L"F22"},
      {134,L"F23"},
      {135,L"F24"},
      {144,L"Num Lock"},
      {145,L"Scroll Lock"},
      {186,L"OEM ;"},
      {187,L"OEM ="},
      {188,L"OEM ,"},
      {189,L"OEM -"},
      {190,L"OEM ."},
      {191,L"OEM /"},
      {192,L"OEM `"},
      {219,L"OEM ["},
      {220,L"OEM \\"},
      {221,L"OEM ]"},
      {222,L"OEM '"}
      //{-1,L"none"}
   };

   wchar_t* Get_Key_Name(unsigned int keycode)
   {
      wchar_t *name = szUknownKey;
      for (int i = 0; i < KEY_INFO_LIST_SIZE; i++)
      {
         if (key_info_list[i].key_code == keycode)
         {
            name = key_info_list[i].key_name;
            break;
         }
      }
      return name;
   }
}