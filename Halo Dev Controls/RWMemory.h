/********************************************************************************
	 -- Read Write Memory Tools
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
	File:    RWMemory.h
	Project: Read Write Memory Tools
	Author:  Jesus7Freak
	Date:    10/29/2011
	Version: 2.30
*********************************************************************************/
#pragma once
// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <windows.h>
#include <Tlhelp32.h>
#include <vector>

//#if defined(_WIN64)
typedef unsigned __int64 QWORD;
//#endif

//call types for CallRemoteFunction:
#define __CDECL         0
#define __STDCALL       1
#define __FASTCALL      2
#define __THISCALL      3
#define __X64FASTCALL   4

#define HEXNUM_LENGTH ((sizeof(ULONG_PTR) * 2) + 1)

//saves space from type casting
#define TO_BYTES(type, byte_array, value) (*(type*)byte_array = (type)value)


namespace ReadWriteMemoryTools
{
   static const int version = 230;

   struct ADDRESS_SIG
   {
      BYTE pattern[25];
      char mask[25];
      char offset;
   };

   bool str_cmpA(char *str1, char *str2, int length = -1);
   bool str_cmpW(wchar_t *str1, wchar_t *str2, int length = -1);

   class RWMemory
   {
   private:
      tagPROCESSENTRY32W* P;
      HANDLE hProcess;


   public:
      int LastWin32Error;

      RWMemory() { P = NULL; hProcess = NULL; LastWin32Error = 0; }
      RWMemory(wchar_t* process_name, DWORD process_to_use = 0);
      RWMemory(DWORD process_id);
      RWMemory(tagPROCESSENTRY32W process);
      ~RWMemory();

      static std::vector<tagPROCESSENTRY32> GetProcessesA();
      static std::vector<tagPROCESSENTRY32W> GetProcessesW();

      static std::vector<tagPROCESSENTRY32> GetProcessesByNameA(
         char* process_name
         );

      static std::vector<tagPROCESSENTRY32W> GetProcessesByNameW(
         wchar_t* process_name
         );

      static tagPROCESSENTRY32 GetProcessByID_A(DWORD process_ID);
      static tagPROCESSENTRY32W GetProcessByID_W(DWORD process_ID);

      HANDLE GetProcessHandle()
      {  return hProcess; }

      tagPROCESSENTRY32W GetProcessEntry()
      {  return *P; }

      std::vector<tagMODULEENTRY32> GetProcessModulesA();
      std::vector<tagMODULEENTRY32W> GetProcessModulesW();

      tagMODULEENTRY32 GetProcessModuleByNameA(char* module_name);
      tagMODULEENTRY32W GetProcessModuleByNameW(wchar_t* module_name);

      void ModuleSectionAddr_Size(
         HMODULE hModule,
         ULONG_PTR &Section_Address,
         SIZE_T &Section_Size,
         BYTE SectionName[IMAGE_SIZEOF_SHORT_NAME],
         bool x64_module
         );

      LPVOID GetRemoteProcAddress(
         HMODULE hModule,
         LPCSTR lpProcName,
         bool x64_module
         );

      ULONG_PTR FindMemPattern(
         ULONG_PTR MemoryAddress,
         SIZE_T Len,
         BYTE *Pattern,
         const char* Mask
         );

      ULONG_PTR FindMemPattern(
         ULONG_PTR MemoryAddress,
         SIZE_T Len,
         ADDRESS_SIG addr_sig)
      {
         ULONG_PTR address = FindMemPattern(
            MemoryAddress,
            Len,
            addr_sig.pattern,
            addr_sig.mask);

         //so that other non-zero checks will work
         if (address) address += addr_sig.offset;

         return address;
      }


      template <typename T> inline
      T ReadMem(LPVOID MemoryAddress)
      {
         T buffer; SIZE_T BytesRead;

         if (!::ReadProcessMemory(
                  hProcess,
                  MemoryAddress,
                  &buffer,
                  sizeof(T),
                  &BytesRead))
            LastWin32Error = ::GetLastError();

         return buffer;
      }

      template <typename T> inline
      bool ReadMemArray(LPVOID MemoryAddress, LPVOID obj, SIZE_T read_length)
      {
         bool succeded = true; SIZE_T BytesRead;

         if (!::ReadProcessMemory(
                  hProcess,
                  MemoryAddress,
                  obj,
                  sizeof(T) * read_length,
                  &BytesRead))
         {
            LastWin32Error = ::GetLastError();
            succeded = false;
         }

         return succeded;
      }

      //use this will keep reading until it hits the null terminator
      //use ReadMemArray to read a certain length
      bool ReadMemString(LPVOID MemoryAddress, char str[]);
      bool ReadMemString(LPVOID MemoryAddress, wchar_t str[]);

      template <typename T> inline
      bool WriteMem(LPVOID MemoryAddress, T obj)
      {

         bool succeeded = true; SIZE_T BytesWritten; T buffer = obj;

         if (!::WriteProcessMemory(
                  hProcess,
                  MemoryAddress,
                  &buffer,
                  sizeof(T),
                  &BytesWritten))
         {
            LastWin32Error = ::GetLastError();
            succeeded = false;
         }

         return succeeded;
      }

      template <typename T> inline
      bool WriteMemArray(LPVOID MemoryAddress, LPVOID obj, SIZE_T write_length = 1)
      {
         bool succeeded = true; SIZE_T BytesWritten;

         if (!::WriteProcessMemory(
                  hProcess,
                  MemoryAddress,
                  obj,
                  sizeof(T) * write_length,
                  &BytesWritten))
         {
            LastWin32Error = ::GetLastError();
            succeeded = false;
         }

         return succeeded;
      }

      //for null terminated strings, use WriteMemArray if no null terminator
      bool WriteMemString(LPVOID MemoryAddress, const char* str);
      bool WriteMemString(LPVOID MemoryAddress, const wchar_t* str);

      bool WriteMemJMP(
         LPVOID JMPLocMemAddress,
         LPVOID JMPToMemAddress,
         BYTE size = 5
         );

      LPVOID AllocateMemory(SIZE_T size);
      bool FreeMemory(LPVOID MemoryAddress);

      //can call functions with multiple parameter, x86 and x64
      //call types, based off of visual studio 2008 compiler
      ULONG_PTR CallRemoteFunction(
         LPTHREAD_START_ROUTINE lpStartAddress,
         DWORD call_type,
         ULONG_PTR *lpParameters = NULL,
         DWORD num_of_params = 0
         );

      //x86 apps cant inject into x64, instead make it x64
      //can return a 64-bit value
      HMODULE InjectDLL(const void* DLLPath, bool bUnicode, bool is_x64_dll);
      bool UnloadDLL(HMODULE hLibModule, bool is_x64_dll);
   };

   namespace CurrentProcess
   {
      ULONG_PTR FindMemPattern(
         ULONG_PTR MemoryAddress,
         SIZE_T Len,
         BYTE *Pattern,
         char* Mask
      );

      ULONG_PTR FindMemPattern(
         ULONG_PTR MemoryAddress,
         SIZE_T Len,
         ADDRESS_SIG addr_sig);

      LPVOID AllocateMemory(SIZE_T size);
      bool FreeMemory(LPVOID MemoryAddress);
   }
}