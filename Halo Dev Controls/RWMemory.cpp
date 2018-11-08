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
    File:    RWMemory.cpp
    Project: Read Write Memory Tools
    Author:  Jesus7Freak, RadWolfie
    Date:    10/30/2016
    Version: 2.41
*********************************************************************************/
//uses both ASCII and UNICODE functions
#ifdef UNICODE
#undef UNICODE
#endif

#include "RWMemory.h"

namespace ReadWriteMemoryTools
{
   bool str_cmpA(char *str1, char *str2, int length)
   {
      bool match = true;
      if (length >= 0)
      {
         //specific length comparison
         for (int i = 0; i < length; i++)
         {
            if (str1[i] != str2[i])
            {
               match = false;
               break;
            }
         }
      }
      else
      {
         //compare until null terminator
         int i = 0;
         char _char1; char _char2;
         do//compare the null terminator as well
         {
            _char1 = str1[i];
            _char2 = str2[i];
            if (_char1 != _char2)
            {
               match = false;
               break;
            }

            i++;
         }while(_char1 && _char2);
      }
      return match;
   }

   //RadWolfie - Neccesary requirement for case insensitive files check.
   bool str_icmpA(const char *str1, const char *str2) {
       bool noMatch = false;
       //compare until null terminator
       unsigned char _char1, _char2;
       do {//compare the null terminator as well
           _char1 = *str1++;
           if ((unsigned)(_char1 - 'A') <= 'Z' - 'A')
               _char1 += 'a' - 'A';

           _char2 = *str2++;
           if ((unsigned)(_char2 - 'A') <= 'Z' - 'A')
               _char2 += 'a' - 'A';

           if (_char1 != _char2) {
               noMatch = true;
               break;
           }
       } while (_char1 && _char2);
       return !noMatch;
   }

   bool str_cmpW(wchar_t *str1, wchar_t *str2, int length)
   {
      bool match = true;
      if (length >= 0)
      {
         //specific length comparison
         for (int i = 0; i < length; i++)
         {
            if (str1[i] != str2[i])
            {
               match = false;
               break;
            }
         }
      }
      else
      {
         //compare until null terminator
         int i = 0;
         wchar_t _wchar1; wchar_t _wchar2;
         do//compare the null terminator as well
         {
            _wchar1 = str1[i];
            _wchar2 = str2[i];
            if (_wchar1 != _wchar2)
            {
               match = false;
               break;
            }

            i++;
         }while(_wchar1 && _wchar2);
      }
      return match;
   }

   #pragma region Constructors / Deconstructor
   RWMemory::RWMemory(wchar_t* process_name, DWORD process_to_use)
   {
      P = NULL;
      P = new tagPROCESSENTRY32W; LastWin32Error = 0;
      std::vector<tagPROCESSENTRY32W> process =
         GetProcessesByNameW(process_name);

      //to prevent array out of bounds error when the process is killed
      if (!process.empty())
         *P = process[process_to_use];

      hProcess = ::OpenProcess(
         PROCESS_CREATE_THREAD |
         PROCESS_QUERY_INFORMATION |
         PROCESS_VM_OPERATION |
         PROCESS_VM_WRITE |
         PROCESS_VM_READ,
         false,
         P->th32ProcessID
         );

      if (!hProcess)
      {
         LastWin32Error = ::GetLastError();
         //if this is spamming messageboxes, disable timer first
         //then re-enable after messagebox returns
         wchar_t Message[27 + HEXNUM_LENGTH] =
         {
            'O','p','e','n','P','r','o','c','e','s','s',':',' ',
            'E','r','r','o','r',' ','C','o','d','e',':',' ','0','x'
         };
         ::swprintf_s(&Message[27], HEXNUM_LENGTH, L"%lX", LastWin32Error);

         ::MessageBoxW(
            NULL,
            Message,
            L"Error",
            MB_OK | MB_ICONWARNING | MB_TASKMODAL
            );
      }
   }

   RWMemory::RWMemory(DWORD process_id)
   {
      P = NULL;
      P = new tagPROCESSENTRY32W(GetProcessByID_W(process_id));
      LastWin32Error = 0;

      hProcess = ::OpenProcess(
         PROCESS_CREATE_THREAD |
         PROCESS_QUERY_INFORMATION |
         PROCESS_VM_OPERATION |
         PROCESS_VM_WRITE |
         PROCESS_VM_READ,
         false,
         P->th32ProcessID
         );

      if (!hProcess)
      {
         LastWin32Error = ::GetLastError();
         wchar_t Message[27 + HEXNUM_LENGTH] =
         {
            'O','p','e','n','P','r','o','c','e','s','s',':',' ',
            'E','r','r','o','r',' ','C','o','d','e',':',' ','0','x'
         };
         ::swprintf_s(&Message[27], HEXNUM_LENGTH, L"%lX", LastWin32Error);

         ::MessageBoxW(
            NULL,
            Message,
            L"Error",
            MB_OK | MB_ICONWARNING | MB_TASKMODAL
            );
      }
   }

   RWMemory::RWMemory(tagPROCESSENTRY32W process)
   {
      P = NULL;
      P = new tagPROCESSENTRY32W(process);
      LastWin32Error = 0;

      hProcess = ::OpenProcess(
         PROCESS_CREATE_THREAD |
         PROCESS_QUERY_INFORMATION |
         PROCESS_VM_OPERATION |
         PROCESS_VM_WRITE |
         PROCESS_VM_READ,
         false,
         P->th32ProcessID
         );

      if (!hProcess)
      {
         LastWin32Error = ::GetLastError();
         wchar_t Message[27 + HEXNUM_LENGTH] =
         {
            'O','p','e','n','P','r','o','c','e','s','s',':',' ',
            'E','r','r','o','r',' ','C','o','d','e',':',' ','0','x'
         };
         ::swprintf_s(&Message[27], HEXNUM_LENGTH, L"%lX", LastWin32Error);

         ::MessageBoxW(
            NULL,
            Message,
            L"Error",
            MB_OK | MB_ICONWARNING | MB_TASKMODAL
            );
      }
   }

   RWMemory::~RWMemory()
   {
      if (P)
      {
         delete P;
         P = NULL;
      }

      if (hProcess)
      {
         if (! ::CloseHandle(hProcess))
            LastWin32Error = ::GetLastError();
         hProcess = NULL;
      }
   }
   #pragma endregion

   std::vector<tagPROCESSENTRY32> RWMemory::GetProcessesA()
   {
      tagPROCESSENTRY32 pe32;
      std::vector<tagPROCESSENTRY32> pe32_list;
      //DWORD lpBinaryType = 0;

      try
      {
         // Take a snapshot of all processes in the system.
         HANDLE hProcessSnap =
            ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
         if (hProcessSnap != INVALID_HANDLE_VALUE)
         {
            // Set the size of the structure before using it.
            pe32.dwSize = sizeof(tagPROCESSENTRY32);

            if (::Process32First(hProcessSnap, &pe32))
            {
               do
                  {
                     //::GetBinaryType(pe32.szExeFile, lpBinaryType);
                     //if (sizeof(ULONG_PTR) ==
                     pe32_list.push_back(pe32);
                  }
                  while (::Process32Next(hProcessSnap, &pe32));
            }
         }

         ::CloseHandle(hProcessSnap);
      }
      catch(...) {}


      return pe32_list;
   }

   std::vector<PROCESSENTRY32W> RWMemory::GetProcessesW()
   {
      tagPROCESSENTRY32W pe32;
      std::vector<tagPROCESSENTRY32W> pe32_list;
      //DWORD lpBinaryType = 0;

      try
      {
         // Take a snapshot of all processes in the system.
         HANDLE hProcessSnap =
            ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
         if (hProcessSnap != INVALID_HANDLE_VALUE)
         {
            // Set the size of the structure before using it.
            pe32.dwSize = sizeof(tagPROCESSENTRY32W);

            if (::Process32FirstW(hProcessSnap, &pe32))
            {
               do
                  {
                     //::GetBinaryType(pe32.szExeFile, lpBinaryType);
                     //if (sizeof(ULONG_PTR) ==
                     pe32_list.push_back(pe32);
                  }
                  while (::Process32NextW(hProcessSnap, &pe32));
            }
         }

         ::CloseHandle(hProcessSnap);
      }
      catch(...) {}


      return pe32_list;
   }

   std::vector<tagPROCESSENTRY32> RWMemory::GetProcessesByNameA(
      char* process_name)
   {
      std::vector<tagPROCESSENTRY32> pe32_list = GetProcessesA(), processes;

      DWORD pe32_list_size = pe32_list.size();
      for (DWORD pe32_i = 0; pe32_i < pe32_list_size; pe32_i++)
      {
         if (str_cmpA(process_name, pe32_list[pe32_i].szExeFile))
            processes.push_back(pe32_list[pe32_i]);
      }

      return processes;
   }

   std::vector<tagPROCESSENTRY32W> RWMemory::GetProcessesByNameW(
      wchar_t* process_name)
   {
      std::vector<tagPROCESSENTRY32W> pe32_list = GetProcessesW(), processes;

      DWORD pe32_list_size = pe32_list.size();
      for (DWORD pe32_i = 0; pe32_i < pe32_list_size; pe32_i++)
      {
         if (str_cmpW(process_name, pe32_list[pe32_i].szExeFile))
            processes.push_back(pe32_list[pe32_i]);
      }

      return processes;
   }

   tagPROCESSENTRY32 RWMemory::GetProcessByID_A(DWORD process_ID)
   {
      std::vector<tagPROCESSENTRY32> pe32_list = GetProcessesA();
      tagPROCESSENTRY32 process;

      process.dwSize = 0;//test if this is still zero before using
      DWORD pe32_list_size = pe32_list.size();
      for (unsigned int pe32_i = 0; pe32_i < pe32_list_size; pe32_i++)
      {
         if (process_ID == pe32_list[pe32_i].th32ProcessID)
         {
            process = pe32_list[pe32_i];
            break;
         }
      }

      return process;
   }

   tagPROCESSENTRY32W RWMemory::GetProcessByID_W(DWORD process_ID)
   {
      std::vector<tagPROCESSENTRY32W> pe32_list = GetProcessesW();
      tagPROCESSENTRY32W process;

      process.dwSize = 0;//test if this is still zero before using
      DWORD pe32_list_size = pe32_list.size();
      for (unsigned int pe32_i = 0; pe32_i < pe32_list_size; pe32_i++)
      {
         if (process_ID == pe32_list[pe32_i].th32ProcessID)
         {
            process = pe32_list[pe32_i];
            break;
         }
      }

      return process;
   }

   std::vector<tagMODULEENTRY32> RWMemory::GetProcessModulesA()
   {
      tagMODULEENTRY32 me32;
      std::vector<tagMODULEENTRY32> me32_list;

      try
      {
         // Take a snapshot of all modules in the process.
         HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(
            TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
            P->th32ProcessID
            );
         if (hModuleSnap != INVALID_HANDLE_VALUE)
         {
            // Set the size of the structure before using it.
            me32.dwSize = sizeof(tagMODULEENTRY32);

            if (::Module32First(hModuleSnap, &me32))
            {
               do
                  {
                     me32_list.push_back(me32);
                  }
                  while (::Module32Next(hModuleSnap, &me32));
            }
         }
         else
            LastWin32Error = ::GetLastError();

         if (!::CloseHandle(hModuleSnap))
            LastWin32Error = ::GetLastError();
      }
      catch(...) {}

      return me32_list;
   }

   std::vector<tagMODULEENTRY32W> RWMemory::GetProcessModulesW()
   {
      tagMODULEENTRY32W me32;
      std::vector<tagMODULEENTRY32W> me32_list;

      try
      {
         // Take a snapshot of all modules in the process.
         HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(
            TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
            P->th32ProcessID
            );
         if (hModuleSnap != INVALID_HANDLE_VALUE)
         {
            // Set the size of the structure before using it.
            me32.dwSize = sizeof(tagMODULEENTRY32W);

            if (::Module32FirstW(hModuleSnap, &me32))
            {
               do
                  {
                     me32_list.push_back(me32);
                  }
                  while (::Module32NextW(hModuleSnap, &me32));
            }
         }
         else
            LastWin32Error = ::GetLastError();

         if (!::CloseHandle(hModuleSnap))
            LastWin32Error = ::GetLastError();
      }
      catch(...) {}

      return me32_list;
   }

   tagMODULEENTRY32 RWMemory::GetProcessModuleByNameA(
      char* module_name)
   {
      std::vector<tagMODULEENTRY32> me32_list = GetProcessModulesA();
      tagMODULEENTRY32 module;

      module.dwSize = 0;//test if this is still zero before using
      DWORD me32_list_size = me32_list.size();
      for (unsigned int me32_i = 0; me32_i < me32_list_size; me32_i++)
      {
         if (str_icmpA(module_name, me32_list[me32_i].szModule))
         {
            module = me32_list[me32_i];
            break;
         }
      }

      return module;
   }

   tagMODULEENTRY32W RWMemory::GetProcessModuleByNameW(
      wchar_t* module_name)
   {
      std::vector<tagMODULEENTRY32W> me32_list = GetProcessModulesW();
      tagMODULEENTRY32W module;

      module.dwSize = 0;//test if this is still zero before using
      DWORD me32_list_size = me32_list.size();
      for (unsigned int me32_i = 0; me32_i < me32_list_size; me32_i++)
      {
         if (str_cmpW(module_name, me32_list[me32_i].szModule))
         {
            module = me32_list[me32_i];
            break;
         }
      }

      return module;
   }

   void RWMemory::ModuleSectionAddr_Size(
         HMODULE hModule,
         ULONG_PTR &Section_Address,
         SIZE_T &Section_Size,
         BYTE SectionName[IMAGE_SIZEOF_SHORT_NAME],
         bool x64_module)
   {
      //Offset to PE sig
      DWORD PE_sig_offset = ReadMem<DWORD>((LPVOID)(
         (ULONG_PTR)hModule + FIELD_OFFSET(IMAGE_DOS_HEADER, e_lfanew)));

      WORD num_of_sections = ReadMem<WORD>((LPVOID)(
         //offset to NumberOfSections
         (ULONG_PTR)hModule + PE_sig_offset +
            FIELD_OFFSET(IMAGE_NT_HEADERS32, FileHeader) +
            FIELD_OFFSET(IMAGE_FILE_HEADER, NumberOfSections)));

      //add the rest of the PE header size and base of module
      ULONG_PTR sections = (ULONG_PTR)hModule + PE_sig_offset ;

      if (x64_module)
         sections += sizeof(IMAGE_NT_HEADERS64);
      else
         sections += sizeof(IMAGE_NT_HEADERS32);

      for (int i = 0; i < num_of_sections; i++)
      {
         int sect_index = i * sizeof(IMAGE_SECTION_HEADER);

         BYTE buffer[IMAGE_SIZEOF_SHORT_NAME] = {0};
         ReadMemArray<BYTE>(
            (LPVOID)(sections + sect_index),
            buffer,
            IMAGE_SIZEOF_SHORT_NAME);

         if (str_cmpA((char*)SectionName,(char*)buffer, IMAGE_SIZEOF_SHORT_NAME))
         {
            Section_Address = (ULONG_PTR)hModule + ReadMem<DWORD>((LPVOID)(
               sections + sect_index +
                  FIELD_OFFSET(IMAGE_SECTION_HEADER, VirtualAddress)));

            Section_Size = ReadMem<DWORD>((LPVOID)(
               sections + sect_index +
                  RTL_FIELD_SIZE(IMAGE_SECTION_HEADER, Name)));
            break;
         }
      }
   }

   LPVOID RWMemory::GetRemoteProcAddress(HMODULE hModule, LPCSTR lpProcName, bool x64_module)
   {
      char func_name[64]; WORD func_index = 0; ULONG_PTR func_address = NULL;

      /*using defined data structs method to get Export Table address offset
      IMAGE_DOS_HEADER *IDH = (IMAGE_DOS_HEADER*)hModule;
      IMAGE_NT_HEADERS *INH = (IMAGE_NT_HEADERS*)(hModule + IDH->e_lfanew);
      IMAGE_EXPORT_DIRECTORY *IED = (IMAGE_EXPORT_DIRECTORY*)(
         hModule + INH->OptionalHeader.DataDirectory[0].VirtualAddress);
      */

      DWORD PE_sig_offset = ReadMem<DWORD>(//IMAGE_DOS_HEADER->e_lfanew
         (LPVOID)((ULONG_PTR)hModule + FIELD_OFFSET(IMAGE_DOS_HEADER, e_lfanew))
         );

      //IMAGE_NT_HEADERS->OptionalHeader.DataDirectory[0].VirtualAddress
      DWORD offset_to_ET = RTL_FIELD_SIZE(IMAGE_NT_HEADERS, Signature) +
         RTL_FIELD_SIZE(IMAGE_NT_HEADERS, FileHeader);

      if (x64_module)
         offset_to_ET += FIELD_OFFSET(IMAGE_OPTIONAL_HEADER64, DataDirectory);
      else
         offset_to_ET += FIELD_OFFSET(IMAGE_OPTIONAL_HEADER32, DataDirectory);

      DWORD Export_Table_offset = ReadMem<DWORD>(
         (LPVOID)((ULONG_PTR)hModule + (ULONG_PTR)PE_sig_offset + offset_to_ET)
         );

     ::IMAGE_EXPORT_DIRECTORY IED = ReadMem< ::IMAGE_EXPORT_DIRECTORY>(
         (LPVOID)((ULONG_PTR)hModule + Export_Table_offset)
         );

      bool found = false;
      if ((DWORD)lpProcName >> 16 != 0)
      {
         for (DWORD name_i = 0; name_i < IED.NumberOfNames && !found; name_i++)
         {
            DWORD str_offset = ReadMem<DWORD>(
               (LPVOID)((ULONG_PTR)hModule + IED.AddressOfNames
                  + (name_i * sizeof(DWORD)))
               );

            ReadMemString((LPVOID)((ULONG_PTR)hModule + str_offset), func_name);

            if (str_cmpA((char*)lpProcName, (char*)func_name))
            {
               func_index = ReadMem<WORD>((LPVOID)((ULONG_PTR)hModule
               + IED.AddressOfNameOrdinals + (name_i * sizeof(WORD))));
               found = true;
            }
         }
      }
      else
      {
         func_index = (WORD)((DWORD)lpProcName - IED.Base);
         found = true;
      }

      if (found)
         func_address = (ULONG_PTR)hModule + ReadMem<DWORD>((LPVOID)(
            (ULONG_PTR)hModule + IED.AddressOfFunctions
            + (func_index * sizeof(DWORD))));

      return (LPVOID)func_address;
   }

   ULONG_PTR RWMemory::FindMemPattern(
      ULONG_PTR MemoryAddress,
      SIZE_T Len,
      BYTE *Pattern,
      const char* Mask)
   {
      ULONG_PTR PatterAddress = 0;
      BYTE* buffer = new BYTE[Len]();
      ReadMemArray<BYTE>((LPVOID)MemoryAddress, buffer, Len);

      for(ULONG_PTR i = 0; i < Len && !PatterAddress; i++)
      {
         //while Mask[i2] isnt the null terminator
         for(ULONG_PTR i2 = 0; Mask[i2]; i2++)
         {
            //make sure its not at the end of the mask before continuing
            if (Mask[i2] == '?' && Mask[i2 + 1])
               continue;
            //if the bytes don't match exit loop
            if (Mask[i2] == 'x' && buffer[i + i2] != Pattern[i2])
               break;
            //when it reaches the end, it must be the address we're looking for
            if (!Mask[i2 + 1])
               PatterAddress = MemoryAddress + i;
         }
      }

      delete[] buffer;
      return PatterAddress;
   }

   bool RWMemory::ReadMemString(LPVOID MemoryAddress, char str[])
   {
      bool succeded = true; SIZE_T BytesRead;
      long i = 0;

      do
      {
         if (!::ReadProcessMemory(
            hProcess,
            (LPVOID)((ULONG_PTR)MemoryAddress + (i * sizeof(char))),
            &str[i],
            sizeof(char),
            &BytesRead))
         {
            LastWin32Error = ::GetLastError();
            succeded = false;
         }

      }while(str[i++] != 0);

      return succeded;
   }

   bool RWMemory::ReadMemString(LPVOID MemoryAddress, wchar_t str[])
   {
      bool succeded = true; SIZE_T BytesRead;
      long i = 0;

      do
      {
         if (!::ReadProcessMemory(
            hProcess,
            (LPVOID)((ULONG_PTR)MemoryAddress + (i * sizeof(wchar_t))),
            &str[i], sizeof(wchar_t),
            &BytesRead))
         {
            LastWin32Error = ::GetLastError();
            succeded = false;
         }

      }while(str[i++] != 0);

      return succeded;
   }

   bool RWMemory::WriteMemString(LPVOID MemoryAddress, const char* str)
   {
      bool succeeded = true; SIZE_T BytesWritten;

      //c string length
      int length = 0; while(str[length++]);

      if (!::WriteProcessMemory(
         hProcess,
         MemoryAddress,
         (void*)str, length * sizeof(char),
         &BytesWritten))
      {
         LastWin32Error = ::GetLastError();
         succeeded = false;
      }

      return succeeded;
   }

   bool RWMemory::WriteMemString(LPVOID MemoryAddress, const wchar_t* str)
   {
      bool succeeded = true; SIZE_T BytesWritten;

      //c string length
      int length = 0; while(str[length++]);

      if (!::WriteProcessMemory(
         hProcess,
         MemoryAddress,
         (void*)str, length * sizeof(wchar_t),
         &BytesWritten))
      {
         LastWin32Error = ::GetLastError();
         succeeded = false;
      }

      return succeeded;
   }

   bool RWMemory::WriteMemJMP(
      LPVOID JMPLocMemAddress,
      LPVOID JMPToMemAddress,
      BYTE size)
   {
      bool succeeded = false;
      BYTE* bytes = new BYTE[size]();
      ULONG_PTR JMP_to_code = (ULONG_PTR)JMPToMemAddress
         - ((ULONG_PTR)JMPLocMemAddress + 5);

      bytes[0] = 0xE9;//JMP
      TO_BYTES(UINT_PTR, &bytes[1], JMP_to_code);

      for (BYTE i = 5; i < size; i++)
         bytes[i] = 0x90;//NOP

      if (WriteMemArray<BYTE>(JMPLocMemAddress, bytes, size))
         succeeded = true;

      delete[] bytes;
      return succeeded;
   }

   LPVOID RWMemory::AllocateMemory(SIZE_T size)
   {
      LPVOID address_of_alloc = ::VirtualAllocEx(
         hProcess,
         NULL,
         size,
         MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
         );

      if (!address_of_alloc)
         LastWin32Error = ::GetLastError();

      return address_of_alloc;
   }

   bool RWMemory::FreeMemory(LPVOID MemoryAddress)
   {
      bool succeded = false;
      if (!::VirtualFreeEx(hProcess, MemoryAddress, NULL, MEM_RELEASE))
         LastWin32Error = GetLastError();
      else
         succeded = true;

      return succeded;
   }

   ULONG_PTR RWMemory::CallRemoteFunction(
      LPTHREAD_START_ROUTINE lpStartAddress,
      DWORD call_type,
      ULONG_PTR *lpParameters,
      DWORD num_of_params
      )
   {
      int size; BYTE *asmfunc; ULONG_PTR asmfunc_loc = NULL;

      //x86 calls
      if (call_type != __X64FASTCALL)
      {
         size = 11 + (5 * num_of_params);
         if (call_type == __CDECL)
            size += 3;

         //allocate memory for the calling function and a DWORD var
         asmfunc_loc = (ULONG_PTR)AllocateMemory(size + sizeof(DWORD));
         //try once more
         if (!asmfunc_loc)
         {
            Sleep(100);
            asmfunc_loc = (ULONG_PTR)AllocateMemory(size + sizeof(DWORD));
            //failed again
            if (!asmfunc_loc) return NULL;
         }

         asmfunc = new BYTE[size]();

         /* asmfunc x86 template
           ?68 xxxxxxxx    //push xxxxxxxx      parameters?
            ...
           ?BA xxxxxxxx    //mov  edx,xxxxxxxx  fastcall param2
           ?B9 xxxxxxxx    //mov  ecx,xxxxxxxx  fastcall param1 or thiscall ptr
            E8 xxxxxxxx    //call xxxxxxxx
           ?83 C4 xx       //add  esp,xx        cdecl stack clean up
            A3 xxxxxxxx    //mov  [xxxxxxxx],eax
            C3             //ret
         */

         int byte_loc = 0;
         int pushed_params = num_of_params;
         if (call_type == __FASTCALL)
            pushed_params -= 2;
         else if (call_type == __THISCALL)
            pushed_params--;

         //write the parameters in the function
         for (int param_i = 0; param_i < pushed_params; param_i++)
         {
            asmfunc[byte_loc++] = 0x68;         //push param@i
            TO_BYTES(
               DWORD,
               &asmfunc[byte_loc],
               lpParameters[num_of_params - 1 - param_i]);

            byte_loc += sizeof(DWORD);
         }

         if (call_type == __FASTCALL && num_of_params > 1)
         {
            asmfunc[byte_loc++] = 0xBA;         //mov edx,xxxxxxxx
            TO_BYTES(DWORD, &asmfunc[byte_loc], lpParameters[1]);

            byte_loc += sizeof(DWORD);
         }

         if (call_type == __FASTCALL && num_of_params > 0
            || call_type == __THISCALL)
         {
            asmfunc[byte_loc++] = 0xB9;         //mov ecx,xxxxxxxx
            TO_BYTES(DWORD, &asmfunc[byte_loc], lpParameters[0]);
            byte_loc += sizeof(DWORD);
         }

         //calculate destination function offset
         asmfunc[byte_loc++] = 0xE8;            //call <function address>
         TO_BYTES(
            DWORD,
            &asmfunc[byte_loc],
            (DWORD)lpStartAddress - ((DWORD)asmfunc_loc + byte_loc + 4));

         byte_loc += sizeof(DWORD);

         if (call_type == __CDECL)
         {
            asmfunc[byte_loc++] = 0x83; //add  esp,num_of_params * sizeof(DWORD)
            asmfunc[byte_loc++] = 0xC4;
            asmfunc[byte_loc++] = (BYTE)(num_of_params * sizeof(DWORD));
         }

         //store the return val at the end the the func
         //could also use 0x89 0x05 ?
         asmfunc[byte_loc++] = 0xA3;     //mov [ret_val_address],eax
         TO_BYTES(DWORD, &asmfunc[byte_loc], (DWORD)asmfunc_loc + size);
         byte_loc += sizeof(DWORD);

         asmfunc[byte_loc++] = 0xC3;      //ret
      }
      //x64 call
      else
      {
         size = 22 + sizeof(ULONG_PTR);
         for (DWORD i = 0; i < num_of_params; i++)
         {
            if (i < 4)
               size += 10;
            else
               size += 15;
         }

         //allocate memory for the calling function and a QWORD var
         asmfunc_loc = (ULONG_PTR)AllocateMemory(size + sizeof(QWORD));
         //try once more
         if (!asmfunc_loc)
         {
            Sleep(100);
            asmfunc_loc = (ULONG_PTR)AllocateMemory(size + sizeof(QWORD));
            //failed again
            if (!asmfunc_loc) return NULL;
         }

         asmfunc = new BYTE[size]();

         /* asmfunc x64 template
            48 83 EC xx             //sub  rsp,xx
            ...
            48 B8 xxxxxxxxxxxxxxxx  //mov  rax,xxxxxxxxxxxxxxxx
            48 89 44 24 xx          //mov  [rsp+xx],rax
            49 B9 xxxxxxxxxxxxxxxx  //mov  r9,xxxxxxxxxxxxxxxx
            49 B8 xxxxxxxxxxxxxxxx  //mov  r8,xxxxxxxxxxxxxxxx
            48 BA xxxxxxxxxxxxxxxx  //mov  rdx,xxxxxxxxxxxxxxxx
            48 89 xxxxxxxxxxxxxxxx  //mov  rcx,xxxxxxxxxxxxxxxx
            FF 15 xxxxxxxx          //call qword ptr [xxxxxxxx]
            48 89 05 xxxxxxxx       //mov  [xxxxxxxx],rax
            48 83 C4 xx             //add  rsp,xx
            C3                      //ret
         */

         int byte_loc = 0;
         BYTE stack_alloc_size = 0;
         //8(the return address) + 32(reserved space for 4 arguments)
         if (num_of_params > 4)
            stack_alloc_size += ((BYTE)num_of_params - 4) * 8;
         stack_alloc_size += 8 + 32;
         //stack pointer RSP must be aligned on a 16-byte boundary
         //before a next function call.
         stack_alloc_size += stack_alloc_size % 16;

         asmfunc[byte_loc++] = 0x48;            //sub  rsp,xx
         asmfunc[byte_loc++] = 0x83;
         asmfunc[byte_loc++] = 0xEC;
         asmfunc[byte_loc++] = stack_alloc_size;

         //write the parameters in the function
         for (int i = 0; num_of_params - i > 4; i++)
         {
            asmfunc[byte_loc++] = 0x48;  ////mov  rax,xxxxxxxxxxxxxxxx
            asmfunc[byte_loc++] = 0xB8;
            TO_BYTES(QWORD, &asmfunc[byte_loc], lpParameters[num_of_params - 1 - i]);
            byte_loc += sizeof(QWORD);

            asmfunc[byte_loc++] = 0x48;
            asmfunc[byte_loc++] = 0x89;
            asmfunc[byte_loc++] = 0x44;
            asmfunc[byte_loc++] = 0x24;
            asmfunc[byte_loc++] = (BYTE)(((num_of_params - 1) * sizeof(QWORD)) -
                                         (i * sizeof(QWORD)));
         }

         if (num_of_params >= 4)
         {
            asmfunc[byte_loc++] = 0x49;       //mov  r9,param4
            asmfunc[byte_loc++] = 0xB9;
            TO_BYTES(QWORD, &asmfunc[byte_loc], lpParameters[3]);
            byte_loc += sizeof(QWORD);
         }
         if (num_of_params >= 3)
         {
            asmfunc[byte_loc++] = 0x49;       //mov  r8,param3
            asmfunc[byte_loc++] = 0xB8;
            TO_BYTES(QWORD, &asmfunc[byte_loc], lpParameters[2]);
            byte_loc += sizeof(QWORD);
         }
         if (num_of_params >= 2)
         {
            asmfunc[byte_loc++] = 0x48;       //mov  rdx,param2
            asmfunc[byte_loc++] = 0xBA;
            TO_BYTES(QWORD, &asmfunc[byte_loc], lpParameters[1]);
            byte_loc += sizeof(QWORD);
         }
         if (num_of_params >= 1)
         {
            asmfunc[byte_loc++] = 0x48;       //mov  rcx,param1
            asmfunc[byte_loc++] = 0xB9;
            TO_BYTES(QWORD, &asmfunc[byte_loc], lpParameters[0]);
            byte_loc += sizeof(QWORD);
         }

         //calculate destination function offset
         asmfunc[byte_loc++] = 0xFF;    //call qword ptr [xxxxxxxx]
         asmfunc[byte_loc++] = 0x15;
         asmfunc[byte_loc++] = 0x0C;
         byte_loc += 3;
         //next 3 are already zero

         //store the return val at the end the the func
         asmfunc[byte_loc++] = 0x48;     //mov [ret_val_address],rax
         asmfunc[byte_loc++] = 0x89;
         asmfunc[byte_loc++] = 0x05;
         asmfunc[byte_loc++] = 0x0D;
         byte_loc += 3;
         //next 3 are already zero

         asmfunc[byte_loc++] = 0x48;       //add  rsp,xx
         asmfunc[byte_loc++] = 0x83;
         asmfunc[byte_loc++] = 0xC4;
         asmfunc[byte_loc++] = stack_alloc_size;

         asmfunc[byte_loc++] = 0xC3;       //ret

         TO_BYTES(QWORD, &asmfunc[byte_loc], lpStartAddress);
      }

      //write the calling function to allocated space
      WriteMemArray<BYTE>((LPVOID)asmfunc_loc, asmfunc, size);

      //call the injected function
      HANDLE NewThreadhnd = ::CreateRemoteThread(
         hProcess,
         NULL,
         0,
         (LPTHREAD_START_ROUTINE)asmfunc_loc,
         NULL,
         0,
         NULL
         );

      if (NewThreadhnd)
      {
         ::WaitForSingleObject(NewThreadhnd, 1000);
         ::CloseHandle(NewThreadhnd);
      }
      else
         LastWin32Error = ::GetLastError();

      //get the return val
      ULONG_PTR return_val = ReadMem<ULONG_PTR>((LPVOID)(asmfunc_loc + size));

      //DWORD pointer return, doesn't work with x64 values
      //::GetExitCodeThread(NewThreadhnd, &return_val);

      //free allocated memory for the calling function and a UINT_PTR var
      FreeMemory((LPVOID)asmfunc_loc);
      delete[] asmfunc;

      return return_val;
   }

   //code help from http://www.codeproject.com/KB/threads/winspy.aspx
   HMODULE RWMemory::InjectDLL(const void* DLLPath, bool bUnicode, bool is_x64_dll)
   {
      HMODULE hLibModule = NULL;
      int mem_size;
      char *tLoadLibrary;

      if (!bUnicode)
      {
         //c string length
         char *DLLPathA = (char*)DLLPath;
         while(*DLLPathA++);

         mem_size = (DWORD)DLLPathA - (DWORD)DLLPath;
         tLoadLibrary = "LoadLibraryA";
      }
      else
      {
         //c string length
         wchar_t *DLLPathW = (wchar_t*)DLLPath;
         while(*DLLPathW++);

         mem_size = (DWORD)DLLPathW - (DWORD)DLLPath;
         tLoadLibrary = "LoadLibraryW";
      }

      tagMODULEENTRY32 hKernel32 = GetProcessModuleByNameA("kernel32.dll");

      if (hKernel32.dwSize)
      {
         LPTHREAD_START_ROUTINE LoadLibrary_address =
            (LPTHREAD_START_ROUTINE)GetRemoteProcAddress(
               hKernel32.hModule,
               tLoadLibrary,
               is_x64_dll
            );

         if (LoadLibrary_address)
         {
            //Allocate then write DLLPath to memory
            LPVOID pLibRemote = AllocateMemory(mem_size);

            SIZE_T BytesWritten;
            if (!::WriteProcessMemory(
                     hProcess,
                     pLibRemote,
                     DLLPath,
                     mem_size,
                     &BytesWritten)
               )
               LastWin32Error = ::GetLastError();

            DWORD call_type;
            if (is_x64_dll)
               call_type = __X64FASTCALL;
            else
               call_type = __STDCALL;

            //Load DLL into remote process, return base of loaded module
            ULONG_PTR parameters[] = { (ULONG_PTR)pLibRemote };
            hLibModule = (HMODULE)CallRemoteFunction(
               LoadLibrary_address,
               call_type,
               parameters,
               1
               );

            FreeMemory(pLibRemote);
         }
      }
      return hLibModule;
   }

   /*concept based off of Joachim Bauch's MemoryModule
   HMODULE RWMemory::InjectMemoryModule(const LPVOID DLLData)
   {
      PIMAGE_DOS_HEADER dos_header;
      PIMAGE_NT_HEADERS nt_header;
      BYTE *code, *headers;
      SIZE_T locationDelta;
      DllEntryProc DllEntry;
      BOOL successfull;

      still working on...
      return hLibModule;
   }*/

   bool RWMemory::UnloadDLL(HMODULE hLibModule, bool is_x64_dll)
   {
      bool succeded = false;
      tagMODULEENTRY32 hKernel32 = GetProcessModuleByNameA("kernel32.dll");

      LPTHREAD_START_ROUTINE FreeLibrary_address =
         (LPTHREAD_START_ROUTINE)GetRemoteProcAddress(
            hKernel32.hModule,
            "FreeLibrary",
            is_x64_dll
            );

      DWORD call_type;
      if (is_x64_dll)
         call_type = __X64FASTCALL;
      else
         call_type = __STDCALL;

      //Unload DLL from remote process
      ULONG_PTR parameters[] = { (ULONG_PTR)hLibModule };
      succeded = CallRemoteFunction(
         FreeLibrary_address,
         call_type,
         parameters,
         1
         ) != 0;

      return succeded;
   }

   namespace CurrentProcess
   {
      ULONG_PTR FindMemPattern(
         ULONG_PTR MemoryAddress,
         SIZE_T Len,
         BYTE *Pattern,
         char* Mask)
      {
         ULONG_PTR PatterAddress = 0;
         BYTE* data = (BYTE*)MemoryAddress;

         for(ULONG_PTR i = 0; i < Len && !PatterAddress; i++)
         {
            //while Mask[i2] isnt the null terminator
            for(ULONG_PTR i2 = 0; Mask[i2]; i2++)
            {
               //make sure its not at the end of the mask before continuing
               if (Mask[i2] == '?' && Mask[i2 + 1])
                  continue;
               //if the bytes don't match exit loop
               if (Mask[i2] == 'x' && data[i + i2] != Pattern[i2])
                  break;
               //if it reaches the end, it must be the address your looking for
               if (!Mask[i2 + 1])
                  PatterAddress = MemoryAddress + i;
            }
         }

         return PatterAddress;
      }

      ULONG_PTR FindMemPattern(
         ULONG_PTR MemoryAddress,
         SIZE_T Len,
         ADDRESS_SIG addr_sig)
      {
         return FindMemPattern(
                  MemoryAddress,
                  Len,
                  addr_sig.pattern,
                  addr_sig.mask) + addr_sig.offset;
      }

      LPVOID AllocateMemory(SIZE_T size)
      {
         int LastWin32Error = 0;
         LPVOID address_of_alloc = ::VirtualAlloc(NULL, size,
            MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

         if (!address_of_alloc)
            LastWin32Error = ::GetLastError();

         return address_of_alloc;
      }

      bool FreeMemory(LPVOID MemoryAddress)
      {
         int LastWin32Error = 0;
         bool succeded = false;
         if (!::VirtualFree(MemoryAddress, NULL, MEM_RELEASE))
            LastWin32Error = GetLastError();
         else
            succeded = true;

         return succeded;
      }
   }
}