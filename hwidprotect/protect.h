#pragma once
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment (lib, "d3d9.lib")
#define GUID_FORMAT "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX"
#define GUID_ARG(guid) (guid).Data1, (guid).Data2, (guid).Data3, (guid).Data4[0], (guid).Data4[1], (guid).Data4[2], (guid).Data4[3], (guid).Data4[4], (guid).Data4[5], (guid).Data4[6], (guid).Data4[7]
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#include <dxgi.h>
#include <math.h>
#include <d3d9.h>
#include <tchar.h>
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <conio.h>
#include <intrin.h>
#include <algorithm>
#include <cctype>
#include <vector>
#include <windows.h>
#include <sysinfoapi.h>
#include <cstring>
#include <shlwapi.h>
#include <string.h>
#include <ctime>
#include <wininet.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>
#include <errno.h>
#include <PortableDevice.h>
#include <iphlpapi.h>

struct SMBIOSHEADER
{
    uint8_t type;
    uint8_t length;
    uint16_t handle;
};
struct SMBIOSData {
    uint8_t  Used20CallingMethod;
    uint8_t  SMBIOSMajorVersion;
    uint8_t  SMBIOSMinorVersion;
    uint8_t  DmiRevision;
    uint32_t  Length;
    uint8_t  SMBIOSTableData[1];
};
struct SYSTEMINFORMATION {
    SMBIOSHEADER Header;
    uint8_t Manufacturer;
    uint8_t Version;
    uint8_t ProductName;
    uint8_t SerialNumber;
    uint8_t WakeUpType;
    uint8_t UUID[16];
    uint8_t SKUNumber;
    uint8_t Family;
};

std::string encrypt(std::string data)
{
    int i, x;
    for (i = 0; (i < 1024 && data[i] != '\0'); i++)
    {
        data[i] = data[i] + 2;
    }
    return data;
}

std::string decrypt(std::string data)
{
    int i, x;
    for (i = 0; (i < 1024 && data[i] != '\0'); i++)
    {
        data[i] = data[i] - 2;
    }
    return data;
}

const char* convert_string_with_header_byte(const SMBIOSHEADER* dm, BYTE s);
static void getgpuinformation();
static void macaddress(static void*);
static void system_uuid(const BYTE* p, short ver);
static void bios();
static void profileguid();
static void diskserialnumber();
static void cpuserial();