#include <iostream>
#include <string>
#include <chrono>
#include <windows.h>
#include <wbemidl.h>
#include <conio.h>
#include "sleepdelay.h" 
#include <powrprof.h>
#include <ntdef.h>
#include <thread>   // New include for multithreading
#include <iomanip>  // For std::put_time

using namespace std;

typedef struct _PROCESSOR_POWER_INFORMATION {
   ULONG  Number;
   ULONG  MaxMhz;
   ULONG  CurrentMhz;
   ULONG  MhzLimit;
   ULONG  MaxIdleState;
   ULONG  CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

// Global Mutex to protect the console output (std::cout)
std::mutex console_mutex;

/**
 * Helper function to move the console cursor to a specific X, Y coordinate.
 */
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/**
 * Helper function to retrieve a specific WMI property from the Win32_BaseBoard class.
 */
string GetMotherboardInfo(const wstring& propertyName) {
    HRESULT hres;

    // 1. Initialize COM
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) { return "COM Init Failed"; }

    // 2. Set COM security levels
    hres = CoInitializeSecurity(
        NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE, NULL
    );
    if (FAILED(hres)) { CoUninitialize(); return "CoInitializeSecurity Failed"; }

    // 3. Obtain the initial locator to WMI
    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hres)) { CoUninitialize(); return "IWbemLocator Failed"; }

    // 4. Connect to the ROOT\CIMV2 namespace
    IWbemServices* pSvc = NULL;
    // We must manually manage BSTRs now that we don't have _bstr_t
    BSTR networkResource = SysAllocString(L"ROOT\\CIMV2");
    hres = pLoc->ConnectServer(networkResource, NULL, NULL, NULL, 0, NULL, NULL, &pSvc);
    SysFreeString(networkResource); // Free the BSTR immediately after use

    if (FAILED(hres)) { pLoc->Release(); CoUninitialize(); return "ConnectServer Failed"; }

    // 5. Set security levels on the WMI connection proxy
    hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(hres)) { pSvc->Release(); pLoc->Release(); CoUninitialize(); return "CoSetProxyBlanket Failed"; }

    // 6. Use ExecQuery to query for motherboard data
    IEnumWbemClassObject* pEnumerator = NULL;
    BSTR queryLanguage = SysAllocString(L"WQL");
    BSTR query = SysAllocString(L"SELECT * FROM Win32_BaseBoard");
    
    hres = pSvc->ExecQuery(
        queryLanguage, query,
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
        NULL, &pEnumerator
    );

    SysFreeString(queryLanguage);
    SysFreeString(query);

    if (FAILED(hres)) {
        if (pSvc) pSvc->Release(); if (pLoc) pLoc->Release();
        CoUninitialize(); return "ExecQuery Failed";
    }

    // 7. Get the data from the query results
    IWbemClassObject* pclsObj = NULL; ULONG uReturn = 0; string result = "N/A";
    
    while (pEnumerator && SUCCEEDED(pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn)) && uReturn != 0) {
        VARIANT vtProp;
        // Get the specific property requested
        hres = pclsObj->Get(propertyName.c_str(), 0, &vtProp, 0, 0);

        if (SUCCEEDED(hres) && (vtProp.vt == VT_BSTR)) {
            // Convert BSTR (wide char) to std::string (narrow char) manually
            // This is a basic conversion, more robust code would handle locales better
            int requiredSize = WideCharToMultiByte(CP_UTF8, 0, vtProp.bstrVal, -1, NULL, 0, NULL, NULL);
            if (requiredSize > 0) {
                char* buffer = new char[requiredSize];
                WideCharToMultiByte(CP_UTF8, 0, vtProp.bstrVal, -1, buffer, requiredSize, NULL, NULL);
                result = string(buffer);
                delete[] buffer;
            }
            VariantClear(&vtProp); // Clear the variant holding the BSTR
        }
        pclsObj->Release(); break; 
    }

    // 8. Cleanup COM resources
    if (pSvc) pSvc->Release(); if (pLoc) pLoc->Release(); if (pEnumerator) pEnumerator->Release();
    CoUninitialize();

    return result;
}

int main() {
    auto now = chrono::system_clock::now();
    time_t time_t_now = chrono::system_clock::to_time_t(now);
    tm* local_time = localtime(&time_t_now);

    int clear_scr = system("cls");

    // Get handle to console and its dimensions
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    // int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;


    // We must use the mutex when writing standard lines too, for safety in a multithreaded context
    std::lock_guard<std::mutex> lock(console_mutex);
    cout << "\033[1;34mAndroidOS UEFI v1.5.0, In Development\033[0m" << endl;
    delay(1000); 

    cout << "\033[1;31mCopyright\033[0m (c) 2025 Beta7Developer. All rights reserved." << endl;
    delay(1000); 

    // IGNORE block for motherboad info (not using mutex here, assuming single thread so far)
    cout << endl << GetMotherboardInfo(L"Manufacturer") << " "; 
    cout << GetMotherboardInfo(L"Product") << " ";
    cout << GetMotherboardInfo(L"SerialNumber") << " "; 
    cout << GetMotherboardInfo(L"Version") << endl;

    delay(1000);

    cout << "Date: " << put_time(local_time, "%Y-%m-%d %H:%M:%S") << endl;

    // --- CPU frequency code insertion ---
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD numProcs = sysInfo.dwNumberOfProcessors;
    PROCESSOR_POWER_INFORMATION* ppi = new PROCESSOR_POWER_INFORMATION[numProcs];
    NTSTATUS status = CallNtPowerInformation(ProcessorInformation, NULL, 0, ppi, sizeof(PROCESSOR_POWER_INFORMATION) * numProcs);

    if (NT_SUCCESS(status)) {
        delay(1000);
        cout << "CPU Max Frequency: " << ppi[0].MaxMhz << " \033[1;32mMHz\033[0m" << endl;
        cout << "CPU Current Throttled Frequency: " << ppi[0].CurrentMhz << " \033[1;31mMHz\033[0m" << endl;
    } else {
        delay(1000);
        cout << "Failed to retrieve CPU frequency (NTSTATUS: " << status << ")" << endl;
    }

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    double totalPhysicalRAM_MB = static_cast<double>(statex.ullTotalPhys) / (1024 * 1024);

    string bootMsg = "Press F1 to continue, Press Del to access the boot manager";
    
    gotoxy(0, consoleHeight - 1); 
    cout << "\033[1;33m" << bootMsg << "\033[0m";

    gotoxy(0, 15); 

    for (int i = 0; i < totalPhysicalRAM_MB + 1; i++) {
        cout << "\rDetected \033[1;34mRAM\033[0m: " << i << " MB";
        cout.flush(); // Force the output buffer to update the screen immediately
    }

    const int wait = 5;
    int remaining = wait;

    Beep(1000, 2000); // POST beep

    while (remaining > 0) {
        if (_kbhit()) {
            int c = _getch();

            // Check F1
            if (c == 0 || c == 224) {
                int ext = _getch();
                if (ext == 59) { // F1 scan code
                    system("secup2");
                    break;
                }
                if (ext == 83) { // Delete key scan code
                    system("bootmgr");
                    break;
                }
            }
            // ESC alternative exit (optional)
            if (c == 27) { 
                system("bootmgr");
                break;
            }
        }
        remaining--;
    }

    gotoxy(0, 16);

    int boot = system("secup2");

    // Clean up allocated memory
    delete[] ppi;
    return 0;
}
