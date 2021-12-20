#include "protect.h"

SYSTEMINFORMATION* find_system_information(SMBIOSData* bios_data)
{
    uint8_t* data = bios_data->SMBIOSTableData;
    while (data < bios_data->SMBIOSTableData + bios_data->Length)
    {
        uint8_t* next;
        SMBIOSHEADER* header = (SMBIOSHEADER*)data;
        if (header->length < 4)
            break;
        if (header->type == 0x01 && header->length >= 0x19)
        {
            return (SYSTEMINFORMATION*)header;
        }

        next = data + header->length;

        while (next < bios_data->SMBIOSTableData + bios_data->Length && (next[0] != 0 || next[1] != 0)) {
            next++;
        }
        next += 2;

        data = next;
    }
    return nullptr;
}

static void volumemountpoint(LPCWSTR volumepointpath)
{
    TCHAR volumepoint[MAX_PATH + 1] = { 0 };

    GetVolumeNameForVolumeMountPoint(volumepointpath, volumepoint, MAX_PATH);

    std::wcout << volumepointpath << " Volume mount point:  " << volumepoint << std::endl;
}
/*
static void getgpuinformation2(void*)
{
    IDXGIFactory1* pFactory1;

    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory1));

    if (FAILED(hr))
    {
        std::wcout << L"CreateDXGIFactory1 failed. " << std::endl;
        return;
    }

    for (UINT i = 0;; i++)
    {
        IDXGIAdapter1* pAdapter1 = nullptr;

        hr = pFactory1->EnumAdapters1(i, &pAdapter1);

        if (hr == DXGI_ERROR_NOT_FOUND)
        {
            // no more adapters
            break;
        }

        if (FAILED(hr))
        {
            std::wcout << L"EnumAdapters1 failed. " << std::endl;
            return;
        }

        DXGI_ADAPTER_DESC1 desc;

        hr = pAdapter1->GetDesc1(&desc);

        if (FAILED(hr))
        {
            std::wcout << L"GetDesc1 failed. " << std::endl;
            return;
        }

        std::wcout << L"Adapter: " << desc.Description << std::endl;

        for (UINT j = 0;; j++)
        {
            IDXGIOutput* pOutput = nullptr;

            HRESULT hr = pAdapter1->EnumOutputs(j, &pOutput);

            if (hr == DXGI_ERROR_NOT_FOUND)
            {
                // no more outputs
                break;
            }

            if (FAILED(hr))
            {
                std::wcout << L"EnumOutputs failed. " << std::endl;
                return;
            }

            DXGI_OUTPUT_DESC desc;

            hr = pOutput->GetDesc(&desc);

            if (FAILED(hr))
            {
                std::wcout << L"GetDesc1 failed. " << std::endl;
                return;
            }

            std::wcout << desc.

            std::wcout << L"  Output: " << desc.DeviceName <<
                L"  (" << desc.DesktopCoordinates.left << L"," << desc.DesktopCoordinates.top << L")-(" <<
                (desc.DesktopCoordinates.right - desc.DesktopCoordinates.left) << L"," <<
                (desc.DesktopCoordinates.bottom - desc.DesktopCoordinates.top) << L")" << std::endl;

        }
    }
}
*/
static void getgpuinformation(void)
{
    typedef IDirect3D9* (WINAPI* PtrDirect3DCreate9)(UINT);
    HMODULE d3d9dlllib = ::LoadLibraryA("d3d9");
    if (!d3d9dlllib)
        return;

    PtrDirect3DCreate9 direct3DCreate9 = (PtrDirect3DCreate9)GetProcAddress(d3d9dlllib, "Direct3DCreate9");
    if (!direct3DCreate9)
        return;

    IDirect3D9* direct3D9 = direct3DCreate9(D3D_SDK_VERSION);
    if (!direct3D9)
        return;

    D3DADAPTER_IDENTIFIER9 adapterIdentifier;
    const HRESULT hr = direct3D9->GetAdapterIdentifier(0, 0, &adapterIdentifier);
    direct3D9->Release();

    if (SUCCEEDED(hr)) {
        std::cout << "GPU VendorId:  " << adapterIdentifier.VendorId << std::endl;
        std::cout << "GPU DeviceId: " << adapterIdentifier.DeviceId << std::endl;
        std::cout << "GPU Driver:" << adapterIdentifier.Driver << std::endl;
        std::cout << "GPU Description: " << adapterIdentifier.Description << std::endl;
        printf("GPU Driver Version:%ld %s%p\n", adapterIdentifier.DriverVersion);
        printf("GPU Driver Version QUADPART:%ld %s%p\n", adapterIdentifier.DriverVersion.QuadPart);
        //printf("Driver Version HIGHPART:%ld %s\n", adapterIdentifier.DriverVersion.HighPart);
        std::cout << "GPU Driver Version HIGHPART: " << adapterIdentifier.DriverVersion.HighPart << std::endl;
        std::cout << "GPU Driver Version LOWPART: " << adapterIdentifier.DriverVersion.LowPart << std::endl;
        //printf("Driver Version LOWPART:%ld %s%p\n", adapterIdentifier.DriverVersion.LowPart);
        /*printf("Driver Version U:%ld %s%p\n", adapterIdentifier.DriverVersion.u);*/
        std::cout << "GPU SubSysId: " << adapterIdentifier.SubSysId << std::endl;
        std::string adamsin;
        int adamsinval;
        printf("GPU GUID : {" GUID_FORMAT "}\n", GUID_ARG(adapterIdentifier.DeviceIdentifier));
        std::cout << "GPU Revision: " << adapterIdentifier.Revision << std::endl;
    }
}

static void volumeinformation(LPWSTR volumepath)
{
    WCHAR volumename[MAX_PATH + 1];
    WCHAR filesystemname[MAX_PATH + 1] = { 0 };
    DWORD serialnumber = 0;
    DWORD maxcompenentlenght = 0;
    DWORD filesystemflagsbuffer = 0;
    //HATA NERDE AMK
    if (GetVolumeInformation(volumepath, (LPWSTR)volumename, sizeof(volumename), &serialnumber, &maxcompenentlenght, &filesystemflagsbuffer, (LPWSTR)filesystemname, sizeof(filesystemname)) == TRUE)
    {
        std::stringstream stringss;
        stringss << std::hex << serialnumber;
        std::string volumeserialnumber(stringss.str());
        std::wstring wstring(volumepath);
        std::string volumepathondisk = std::string(wstring.begin(), wstring.end());
        std::transform(volumeserialnumber.begin(), volumeserialnumber.end(), volumeserialnumber.begin(), ::toupper);
        std::cout << volumepathondisk << " " << "Serial Number = " << volumeserialnumber;
        std::cout << "\n";
    }
}
static void macaddress(static void*)
{
    IP_ADAPTER_INFO AdapterInfo[32];
    DWORD dwBufLen = sizeof(AdapterInfo);
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) != ERROR_SUCCESS)
    {
        printf("hata");
    }

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    char szBuffer[512];
    while (pAdapterInfo)
    {
        if (pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)
        {
            sprintf_s(szBuffer, sizeof(szBuffer), "%.2X%.2X%.2X%.2X%.2X%.2X"
                , pAdapterInfo->Address[0]
                , pAdapterInfo->Address[1]
                , pAdapterInfo->Address[2]
                , pAdapterInfo->Address[3]
                , pAdapterInfo->Address[4]
                , pAdapterInfo->Address[5]
            );
            printf("Mac Address = %s", szBuffer);
        }
        pAdapterInfo = pAdapterInfo->Next;
    }
    printf("\n");
}

static void system_uuid(const BYTE* p, short ver)
{
    int only0xFF = 1, only0x00 = 1;
    int i;
    for (i = 0; i < 16 && (only0x00 || only0xFF); i++)
    {
        if (p[i] != 0x00) only0x00 = 0;
        if (p[i] != 0xFF) only0xFF = 0;
    }
    if (only0xFF)
    {
        printf("CANNOT FOUND");
        return;
    }
    if (only0x00)
    {
        printf("CANT CONVERTABLE");
        return;
    }

    if (ver >= 0x0206)
    {
        std::string uuid_;
        static const int max_uuid_size{ 50 };
        char uuid[max_uuid_size] = {};
        _snprintf_s(uuid, max_uuid_size, max_uuid_size - 1, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
            p[3], p[2], p[1], p[0], p[5], p[4], p[7], p[6],
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
        uuid_ = uuid;
        /*
        printf("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
            p[3], p[2], p[1], p[0], p[5], p[4], p[7], p[6],
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
        */
        printf(uuid_.c_str());
    }
    else
    {
        std::string uuid_;
        static const int max_uuid_size{ 50 };
        char uuid[max_uuid_size] = {};
        _snprintf_s(uuid, max_uuid_size, max_uuid_size - 1, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
            p[3], p[2], p[1], p[0], p[5], p[4], p[7], p[6],
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);

        uuid_ = uuid;
        printf(uuid_.c_str());
    }
    /*
        printf("-%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15])*/
}

const char* get_string_by_index(const char* str, int index, const char* null_string_text = "")
{
    if (0 == index || 0 == *str) {
        return null_string_text;
    }

    while (--index) {
        str += strlen(str) + 1;
    }
    return str;
}

const char* convert_string_with_header_byte(const SMBIOSHEADER* dm, BYTE s)
{
    char* bp = (char*)dm;
    size_t i, len;
    if (s == 0)
    {
        return "ASYMPTOMATIC";
    }

    bp += dm->length;

    while (s > 1 && *bp)
    {
        bp += strlen(bp);
        bp++;
        s--;
    }
    if (!*bp)
    { 
        return "INDEX_ERROR hatasi nullptr yuksek ihtimal";
    }
    len = strlen(bp);
    for (i = 0; i < len; i++)
        if (bp[i] < 32 || bp[i] == 127)
            bp[i] = '.';
    return bp;
}

static void bios(void)
{
    std::string serialnumber_;
    DWORD bufsize = 0;
    BYTE buf[65536] = { 0 };
    int ret = 0;
    SMBIOSData* Smbios;
    SMBIOSHEADER* header = NULL;
    int flag = 1;

    ret = GetSystemFirmwareTable('RSMB', 0, 0, 0);
    if (!ret)
    {
        printf("FIRST SYSTEMFRIMWARETABLE FUNCTION FAILED!\n");
    }
    bufsize = ret;
    ret = GetSystemFirmwareTable('RSMB', 0, buf, bufsize);
    if (!ret)
    {
        printf("SECOND SYSTEMFRIMWARETABLE FUNCTION FAILED!\n");
    }
    Smbios = (SMBIOSData*)buf;
    BYTE* p = Smbios->SMBIOSTableData;
    if (Smbios->Length != bufsize - 8)
    {
        printf("Smbios length error\n");
    }

    for (int s = 0; s < Smbios->Length; s++)
    {
        header = (SMBIOSHEADER*)p;
        if (header->type == 0 && flag)
        {
            printf("BIOS Version = %s\n", convert_string_with_header_byte(header, p[0x5]));
            printf("BIOS Release Date = %s\n", convert_string_with_header_byte(header, p[0x8]));
            flag = 0;
        }
        else if (header->type == 1)
        {
            printf("Version = %s\n", convert_string_with_header_byte(header, p[0x6]));
            printf("Serial Number = %s\n", convert_string_with_header_byte(header, p[0x7]));
            printf("UUID = "); system_uuid(p + 0x8, Smbios->SMBIOSMajorVersion * 0x100 + Smbios->SMBIOSMinorVersion);
            printf("\n");
            printf("SKU Number = %s\n", convert_string_with_header_byte(header, p[0x19]));
        }
        p += header->length;
        while ((*(WORD*)p) != 0) p++;
        p += 2;
    }
    SYSTEMINFORMATION* sysinfo = find_system_information(Smbios);
    const char* str = (const char*)sysinfo + sysinfo->Header.length;
    if (sysinfo)
    {
        serialnumber_ = get_string_by_index(str, sysinfo->SerialNumber);
        printf("Serial Number2 = %s", serialnumber_.c_str());
        printf("\n");
    }
}

static void profileguid(void)
{
    HW_PROFILE_INFO   HwProfInfo;
    if (!GetCurrentHwProfile(&HwProfInfo))
    {
        _tprintf(TEXT("ProfileGuid ERROR.%s\n"), GetLastError());
    }
    _tprintf(TEXT("ProfileGuid = %s\n"), HwProfInfo.szHwProfileGuid);
}

static void diskserialnumber()
{
        HANDLE device = CreateFileW(L"//./PhysicalDrive0", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
        if (device == INVALID_HANDLE_VALUE)
        {
            printf("HARDDISK DRIVE CANNOT FOUND");
        }
        STORAGE_PROPERTY_QUERY query{};
        query.PropertyId = StorageDeviceProperty;
        query.QueryType = PropertyStandardQuery;

        STORAGE_DESCRIPTOR_HEADER storgeDescriptorHeader = { 0 };
        DWORD dwbytesreturned;

        if (!DeviceIoControl(device, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(STORAGE_PROPERTY_QUERY), &storgeDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER), &dwbytesreturned, NULL))
        {
            printf("DEVICEIOCONTROL CANNOT ACCESS");
        }

        const DWORD dwbuffersize = storgeDescriptorHeader.Size;
        std::vector<BYTE> pOutBuffer(dwbuffersize, 0);
        if (!DeviceIoControl(device, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(STORAGE_PROPERTY_QUERY), pOutBuffer.data(), dwbuffersize, &dwbytesreturned, NULL))
        {
            printf("DEVICEIOCONTROL CANNOT ACCESS 2");
        }
        STORAGE_DEVICE_DESCRIPTOR* devicedescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer.data();
        DWORD serialnumberoffset = devicedescriptor->SerialNumberOffset;
        if (serialnumberoffset != 0)
        {
            std::string serialnumber;
            serialnumber = (char*)(pOutBuffer.data() + serialnumberoffset);
            std::string serialnumberwithoutblank;
            for (int i = 0; i < serialnumber.length(); ) {
                if (serialnumber[i] == ' ') {
                    if (i == 0 || i == serialnumber.length() - 1) {
                        i++;
                        continue;
                    }
                    while (serialnumber[i + 1] == ' ')
                        i++;
                }
                serialnumberwithoutblank += serialnumber[i++];
            }
            std::cout << "Harddisk Serial Number =" << serialnumberwithoutblank << std::endl;
            CloseHandle(device);
        }
}

static void cpuserial(void)
{
    std::array<int, 4> cpuInfo;
    __cpuid(cpuInfo.data(), 1);
    std::stringstream buffer;
    buffer << std::hex << std::setfill('0') << std::setw(8) << cpuInfo.at(3) << std::setw(8) << cpuInfo.at(0);
    std::string newbuffer;
    newbuffer = buffer.str();
    std::transform(newbuffer.begin(), newbuffer.end(), newbuffer.begin(), ::toupper);
    std::cout << "Processor ID : " << newbuffer << std::endl;
}


int __cdecl wmain(int argc, WCHAR* argv[])
{
    profileguid();
    diskserialnumber();
    cpuserial();
    bios();
    macaddress(NULL);
    getgpuinformation();
    const wchar_t* volumepath = L"C:\\";
    const wchar_t* volumepath1 = L"D:\\";
    volumeinformation((LPWSTR)volumepath);
    volumeinformation((LPWSTR)volumepath1);
    volumemountpoint(volumepath);
    volumemountpoint(volumepath1);
}