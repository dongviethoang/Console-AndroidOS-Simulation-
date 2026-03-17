/*
This is the main file of the Console AndroidOS simulation.
Don't mess with anything.
And if you want to fix it, unless you're a C++ developer, download the ZIP again. I am not gonna travel to your house.

- Hoang Viet (2025)
*/
// i like C++.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include "sleepdelay.h"
#include <csignal>
#include <fstream>
#include "consoledelay.h"
#include <conio.h>
#include "code_editor.h"
#include "text_editor.h"
#include <cstdint>
#include "example_threat.h"
#include <filesystem>
#include <tlhelp32.h>
#include <powrprof.h>
#include <ntdef.h>
#include <type_traits>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using namespace std;

#define el "\n"

ofstream keyfile;

SLEEP sleep; // externed

typedef struct _PROCESSOR_POWER_INFORMATION {
   ULONG  Number;
   ULONG  MaxMhz;
   ULONG  CurrentMhz;
   ULONG  MhzLimit;
   ULONG  MaxIdleState;
   ULONG  CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

volatile sig_atomic_t forced_shutdown = 0; 

int time_wasted_here = 1;
const int version = 420;
const int kernel_version = 30;
const string kernel_name = "x25";  
const string minor_update = "Security Update 2";
const int yeol = 2042;
const int meol = 1;
const int deol = 16;
bool security_comp = false;
bool graphics_driver = false;

MEMORYSTATUSEX getmemInfo;

atomic<bool> block_memory_check(false);

// Maps are so f[beep]ing confusing
map<string, string> files = 
{
    {"ANDROID_OS.ISO", "System boot image"}, 
    {"WARNING_KNL.EXE", "Critical system file"}, 
    {"README.TXT", "System documentation"}, 
    {"TASKS.LOG", "Task execution history"},
    {"CONFIG.SYS", "System configuration file"}
};

map<string, string> supported_formats =
{
    {".EXE", "Windows Application Format"},
    {".OUT", "UNIX Application Format"},
    {".LOG", "Log file"},
};

vector<string> recycle_bin;
vector<string> installed_apps = {"File Explorer", "Calculator", "System Info"};
// not in use for this UI
vector<string> new_ui_apps = {
    "taskmgr", "recycle", "firewall", "search_files",
    "benchmk", "bluetooth", "contrast",
    "security", "scheduler", "hibernate", "lock",
    "exit", "shutdown", "devmgr", "restart", "explorer", "fileopen",
    "update", "activate", "codeedit", "launch_ext_app",
    "textedit", "vim", "envvar",
    "playsound"
};

vector<string> new_ui_apps_dev = {
    "taskmgr", "recycle", "firewall", "search_files",
    "benchmk", "bluetooth", "contrast",
    "security", "scheduler", "hibernate", "lock",
    "exit", "shutdown", "devmgr", "restart", "explorer", "fileopen",
    "update", "activate", "codeedit", "launch_ext_app",
    "textedit", "vim", "envvar",
    "playsound", "signout"
};

// I am not gonna prevent anyone hacking into this s[beep]
// Do whatever you want, just don't mod the code
string passin;
string userin;
vector<string> users = {"Admin", "Guest", "Developer", "Tester"};
string passwords[2] = {"12345678", "viethoang082013"};
uint64_t long_pass = 2092343984202025;
string current_user = "guest";
string theme = "Light";
bool system_locked = false;
bool update_available = false;

uint64_t some_variable = 19531234;

void check_memory() {
    while (!block_memory_check) {
        getmemInfo.dwLength = sizeof(getmemInfo);
        if (!GlobalMemoryStatusEx(&getmemInfo)) {
            cerr << "Failed to get memory status." << el;
        }
        double totalRAM = static_cast<double>(getmemInfo.ullTotalPhys) / (1024 * 1024);
        double availRAM = static_cast<double>(getmemInfo.ullAvailPhys) / (1024 * 1024);
        double usedRAM = totalRAM - availRAM;
        double usagePercent = (usedRAM / totalRAM) * 100.0;

        if (availRAM < 256.0) {
            cerr << "Warning: Low available memory detected (" << availRAM << " MB available). Performance may be affected." << el;
        }
        if (usagePercent > 90.0) {
            cerr << "Warning: High memory usage detected (" << usagePercent << "% used). Consider closing some applications." << el;
        }
        this_thread::sleep_for(chrono::seconds(10)); // Check every 10 seconds
    }
} 

void detect_forced_shutdown(int signal) {
    forced_shutdown = 1;
    // Use C-style I/O in signal handlers
    FILE* flag = fopen("D:/Console AndroidOS/bin/crash.flag", "w");
    if (flag) {
        fprintf(flag, "interrupted\n");
        fclose(flag);
    }
    exit(1);
}

void check_exp()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    if (!ltm) {
        update_available = false;
        return;
    }
    
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;

    if (year > yeol) {
        update_available = true;
    } else if (year == yeol) {
        if (month > meol) {
            update_available = true;
        } else if (month == meol) {
            if (day >= deol) {
                update_available = true;
            }
        }
    }
}

// NEW: paths for activation keys and flag
const string KEYS_PATH = "D:/Console AndroidOS/src/keys/keys.txt";
const string ACT_PATH  = "D:/Console AndroidOS/src/keys/activated.flag";
const string RESET_FLAG_PATH = "D:/Console AndroidOS/src/keys/reset.flag";

// trim helper
static inline string trim(const string &s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end-1]))) --end;
    return s.substr(start, end - start);
}

uint64_t passin2;

// Clear Console
void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void login()
{
    cout << "Choose a user (";
    for (size_t i = 0; i < users.size(); ++i) {
        if (i) cout << ", ";
        cout << users[i];
    }
    cout << "): ";
    getline(cin, userin);
    
    // Trim whitespace from input
    userin = trim(userin);
    
    // Convert to lowercase for case-insensitive comparison
    for (char &c : userin) {
        c = tolower(c);
    }
    
    // Guest login (no password required)
    if (userin == "guest")
    {
        cout << "Logging in as Guest..." << endl;
        current_user = "Guest";
        return;
    }
    
    // Admin login
    if (userin == "admin")
    {
        cout << "Enter the password: ";
        getline(cin, passin);
        if (passin == passwords[0])
        {
            cout << "Logging in as Admin..." << endl;
            current_user = "Admin";
        }
        else
        {
            cerr << "Incorrect password. Access denied." << endl;
            current_user = "guest";
        }
        return;
    }
    
    // Developer login
    if (userin == "developer")
    {
        cout << "Enter the password: ";
        getline(cin, passin);
        if (passin == passwords[1])
        {
            cout << "Logging in as Developer..." << endl;
            current_user = "Developer";
        }
        else
        {
            cerr << "Incorrect password. Access denied." << endl;
            current_user = "guest";
        }
        return;
    }
    
    // Tester login (requires numeric password)
    if (userin == "tester")
    {
        cout << "Enter the password: ";
        getline(cin, passin);
        if (passin == to_string(long_pass))
        {
            cout << "Logging in as Tester..." << endl;
            current_user = "Tester";
        }
        else
        {
            cerr << "Incorrect password. Access denied." << endl;
            current_user = "guest";
        }
        return;
    }
    
    // handle any other user defined in users vector (guest-level access)
    for (const auto &u : users) {
        if (u != "Admin" && u != "Guest" && u != "Developer" && u != "Tester") {
            if (_stricmp(u.c_str(), userin.c_str()) == 0) {
                cout << "Logging in as " << u << "..." << endl;
                current_user = u;
                return;
            }
        }
    }

    // Invalid user
    cerr << "User '" << userin << "' not found. Access denied." << endl;
    current_user = "guest";
}

// Task Manager
void task_manager()
{
    cout << "\nTASK MANAGER - Running Processes" << el;
    cout << "--------------------------------" << el;
    // List simulated apps based on user
    vector<string> apps = (current_user == "Developer" || current_user == "Tester") ? new_ui_apps_dev : new_ui_apps;
    cout << "Installed Apps:" << el;
    for (size_t i = 0; i < apps.size(); ++i) {
        cout << i + 1 << ". " << apps[i] << el;
    }
    cout << "--------------------------------" << el;
    // Add memory info
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    if (GlobalMemoryStatusEx(&memInfo)) {
        double totalMB = static_cast<double>(memInfo.ullTotalPhys) / (1024 * 1024);
        double availMB = static_cast<double>(memInfo.ullAvailPhys) / (1024 * 1024);
        double usedMB = totalMB - availMB;
        double usagePercent = (usedMB / totalMB) * 100.0;
        cout << "Memory Usage: " << fixed << setprecision(1) << usedMB << " MB used / " << totalMB << " MB total (" << usagePercent << "%)\n";
    }
}

// Why recycle bin? Find the reason yourselves.
void recycle_bin_manager()
{
    cout << "\nRECYCLE BIN" << el;
    if (recycle_bin.empty())
    {
        cout << "Recycle bin is empty." << el;
    }
    else
    {
        cout << "Files in recycle bin:" << el;
        for (const auto& file : recycle_bin)
        {
            cout << "- " << file << el;
        }
        cout << "Type 'RESTORE' to recover all files or 'EMPTY' to delete permanently: ";
        string action;
        cin >> action;
        if (action == "RESTORE")
        {
            for (const auto& file : recycle_bin)
            {
                files[file] = "Restored file";
            }
            recycle_bin.clear();
            cout << "Files restored successfully." << el;
        }
        else if (action == "EMPTY")
        {
            recycle_bin.clear();
            cout << "Recycle bin emptied." << el;
        }
    }
}

// Firewall that is not a f[beep]ing firewall
void firewall_settings()
{
    cout << "\nFIREWALL SETTINGS" << el;
    cout << "Status: ENABLED" << el;
    cout << "Allowed Programs: File Explorer, Calculator, Weather App" << el;
    cout << "Blocked Programs: Unknown Sources" << el;
}

// File Search
void file_search()
{
    cout << "\nFILE SEARCH" << el;
    cout << "Enter filename to search: ";
    string filename;
    cin >> filename;
    if (files.find(filename) != files.end())
    {
        cout << "File found: " << filename << " - " << files[filename] << el;
    }
    else
    {
        cout << "File not found." << el;
    }
}

void system_benchmark()
{
    cout << "\nRUNNING SYSTEM BENCHMARK..." << el;
    this_thread::sleep_for(chrono::milliseconds(2000));
    typedef NTSTATUS (WINAPI *CallNtPowerInformation_t)(POWER_INFORMATION_LEVEL, PVOID, ULONG, PVOID, ULONG);
    HMODULE hNtDll = LoadLibraryW(L"ntdll.dll");
    if (hNtDll) {
        CallNtPowerInformation_t CallNtPowerInformation = (CallNtPowerInformation_t)GetProcAddress(hNtDll, "NtPowerInformation");
        if (CallNtPowerInformation) {
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

            delete[] ppi;
        } else {
            delay(1000);
            cout << "Failed to get CallNtPowerInformation function" << endl;
        }
        FreeLibrary(hNtDll);
    } else {
        delay(1000);
        cout << "Failed to load ntdll.dll" << endl;
    }
    // RAM Speed
    FILE* pipe = _popen("wmic memorychip get speed /value", "r");
    if (!pipe) {
        delay(1000);
        cout << "Failed to get RAM speed." << endl;
    } else {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            string line = buffer;
            if (line.find("Speed=") != string::npos) {
                size_t pos = line.find('=');
                if (pos != string::npos) {
                    string speed = line.substr(pos+1);
                    speed = trim(speed);
                    delay(1000);
                    cout << "RAM Speed: " << speed << " MHz" << endl;
                }
            }
        }
        _pclose(pipe);
    }
    // Current Memory Usage
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    if (GlobalMemoryStatusEx(&memInfo)) {
        double totalMB = static_cast<double>(memInfo.ullTotalPhys) / (1024 * 1024);
        double availMB = static_cast<double>(memInfo.ullAvailPhys) / (1024 * 1024);
        double usedMB = totalMB - availMB;
        double usagePercent = (usedMB / totalMB) * 100.0;
        delay(1000);
        cout << "Current Memory: " << fixed << setprecision(1) << availMB << " MB available / " << totalMB << " MB total (" << usagePercent << "% used)" << endl;
    }
}

// This is not Bluetooth, it's BluePretend
void bluetooth_manager()
{
    cout << "\nBLUETOOTH MANAGER" << el;
    cout << "Available Devices: \n1. Wireless Headphones\n2. Smartwatch\n3. Bluetooth Speaker" << el;
    cout << "Enter device number to connect: ";
    int device;
    cin >> device;
    cout << "Connecting..." << el;
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "Device connected successfully." << el;
}

// Hm, what would this do?
// AHHH IT BLINDED ME
void dark_mode_toggle()
{
    if (theme == "Light")
    {
        theme = "Dark";
        system("color F0");
        cout << "Dark mode enabled." << el;
    }
    else
    {
        system("color 0F");
        theme = "Light";
        cout << "Light mode enabled." << el;
    }
}

// this is Windows Defender, for now
void windows_defender()
{
    cout << "\nSCANNING SYSTEM FILES FOR THREATS..." << el;
    this_thread::sleep_for(chrono::milliseconds(2000));
    if (!security_comp) {
        cout << "Blocked threats: 0" << el;
    } else {
        cout << "Blocked threats: 1" << el;
    }
}

// Task Scheduler
void task_scheduler()
{
    cout << "\nTASK SCHEDULER" << el;
    cout << "Enter task name to schedule: ";
    string task;
    cin >> task;
    cout << "Task '" << task << "' scheduled successfully." << el;
}

// Hibernate Mode
void hibernate_mode()
{
    cout << "\nSystem is now in Hibernate Mode. Resume to continue." << el;
    this_thread::sleep_for(chrono::milliseconds(2000));
    cout << "System Resumed!" << el;
}

// Lock Screen
void lock_screen()
{
    system_locked = true;
    cout << "\nSYSTEM LOCKED. Enter password to unlock: ";
    string password;
    while (true)
    {
        cin >> password;
        if (password == "password")
        {
            system_locked = false;
            cout << "System Unlocked!" << el;
            break;
        }
        else
        {
            cout << "Incorrect password. Try again: ";
        }
    }
}

void bootloader()
{
    int x;
    cout << "Copyright Beta Corporation (c). All rights reserved" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Console AndroidOS" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Build 420 (Kernel Version 30)" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Current Minor Version: Security Update 2" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Launching RAM Test..." << endl;
    this_thread::sleep_for(chrono::seconds(5));

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    double totalPhysicalRAM_MB = 0;
    if (!GlobalMemoryStatusEx(&statex)) {
        totalPhysicalRAM_MB = 0;
    } else {
        totalPhysicalRAM_MB = static_cast<double>(statex.ullTotalPhys) / (1024 * 1024);
    }

    // single-line progress indicator (updates in-place)
    cout << "RAM Test... ";
    for (int i = 0; i < totalPhysicalRAM_MB + 1; i++) {
        cout << "\rDetected RAM: " << i << " MB";
        cout.flush(); // Force the output buffer to update the screen immediately
    }

    cout << " done" << el;

    srand(time(0));
    float failure = rand() % 100;
    if (failure > 4.4)
    {
        cout << "RAM test results: Nominal" << el;
    }
    else
    {
        cout << "RAM test failure. Press any key to exit";
        while (true) {
            #ifdef _WIN32
                if (_kbhit()) {
                    int code = _getch();
                    if (code == 0 || code == 224) {
                        code = _getch();
                    }
                    cout << el << "Exiting..." << el;
                    Sleep(1000);
                    exit(1);
                }
            #else
                // POSIX/UNIX implementation would go here
                #ifdef __unix__
                #include <termios.h>
                #include <unistd.h>
                struct termios oldt, newt;
                tcgetattr(STDIN_FILENO, &oldt);
                newt = oldt;
                newt.c_lflag &= ~(ICANON | ECHO);
                tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                getchar();
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                cout << el << "Exiting..." << el;
                sleep(1);
                exit(1);
                #endif
            #endif
        }
    }
}

vector<string> drivers = { "Realtek ", "HTC ", "Windows ", "AndroidOS USB Driver v1.4.1" };
string driverinp;

void devicemgr()
{
    string deviceinp;
    cout << "Devices: " << el;
    cout << "USBs: " << el;
    cout << "USB Serial (COM2)" << el;
    cout << ">> ";
    getline(cin, deviceinp);
    if (deviceinp == "USB Serial")
    {
        cout << "Drivers: ";
        for (int i = 0; i <= 3; i++)
        {
            cout << drivers[i] << " ";
        }
        cout << "\n>> ";
        getline(cin, driverinp);
        if (driverinp == drivers[3])
        {
            cout << "Applied driver: " << drivers[3] << endl;
        }
    }
}

void restart()
{
    system("secup2.exe");
}

void file_explorer()
{
    /*
    cout << "\nAll files: " << el;
    for (const auto& pair : files)
    {
        cout << pair.first << ": " << pair.second << el;
    }
    system("\n");
    */
    int launch = system("sfs.exe");
}

string error = R"(0x02)";

void file_opener()
{
    cout << "\nChoose which file to open:" << el;
    file_explorer(); // shows all files
    cout << ">> ";

    string filein;
    getline(cin, filein);

    // check if file exists in map
    auto it = files.find(filein);
    if (it == files.end())
    {
        cout << "File not found." << el;
        return;
    }

    // special crash cases
    if (filein == "ANDROID_OS.ISO")
    {
        cout << "Loading " << filein << " ..." << el;
        delay(1000);
        cout << error;
        sleep.delay(1000);
        raise(SIGSEGV); // simulated crash
    }
    else if (filein == "WARNING_KNL.EXE")
    {
        for (int i = 3; i >= 0; i--)
        {
            cerr << "WARNING: System crash in: " << i << el;
            delay(1000);
            clear_console();
        }
        raise(SIGSEGV);
    }
    else
    {
        // normal open
        cout << "Opening " << filein << " - " << it->second << el;
    }
}
// if anyone doesn't know what 'SIGSEGV' is, it's calling a segfault

void update()
{
    cout << "Updating to build: 508" << endl;
    if (update_available == false)
    {
        cerr << "There is no updates... yet." << el;
    }
    else
    {
        system("build508.exe");
    }
}

bool activated = false;
bool genuine_flag = false;

// read/write genuine marker and record runtime info
static bool read_genuine_state(const string &path = "D:/Console AndroidOS/bin/genuine.txt") {
    ifstream gf(path);
    if (!gf.is_open()) return false;
    string line;
    while (getline(gf, line)) {
        if (trim(line) == "genuine=true") {
            gf.close();
            return true;
        }
    }
    gf.close();
    return false;
}

// Replace/implement activate() - do NOT create key files; read keys file and write flag via ofstream
void activate() {
    cout << "Enter your activation key: ";
    string input_key;
    getline(cin, input_key);
    input_key = trim(input_key);

    if (input_key.empty()) {
        cout << "No key entered. Activation cancelled." << el;
        return;
    }

    // open keys file (do NOT create it)
    ifstream kf(KEYS_PATH);
    if (!kf.is_open()) {
        cerr << "Activation key store not found: " << KEYS_PATH << el;
        cerr << "Place a file with one valid key per line and try again." << el;
        return;
    }

    string line;
    bool found = false;
    while (getline(kf, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (line == input_key) { found = true; break; }
    }
    kf.close();

    if (found) {
        // persist activation using ofstream
        ofstream af(ACT_PATH, ios::out | ios::trunc);
        if (!af.is_open()) {
            cerr << "Failed to persist activation to: " << ACT_PATH << el;
            cerr << "Check permissions." << el;
            return;
        }
        af << "activated=true" << el;
        af.close();

        activated = true;
        cout << "Activation successful! Thank you for activating Console AndroidOS." << el;
    } else {
        cout << "Invalid activation key. Please try again." << el;
        system("python src/sounddriver.py D:/Downloads/windows_xp_error.ogg");
    }
}

// NEW: load activation state at startup (reads flag file)
void load_activation_state() {
    ifstream af(ACT_PATH);
    if (!af.is_open()) {
        activated = false;
        return;
    }
    string line;
    while (getline(af, line)) {
        line = trim(line);
        if (line == "activated=true" || line == "activated=1") {
            activated = true;
            break;
        }
    }
    af.close();
} // activation uses... flags.

void code_editor()
{
    get_input();
}

void launch_external_app() {
    threat();
}

void env_var() {
    cout << el << "Environment Variables" << el;
    cout << "VARIABLES:" << el;
    cout << "SUPPORTED_FORMATS: ";
    for (const auto& pair : supported_formats) {
        cout << "\n " << pair.first << " " << pair.second << "\n";
    } 
    delay(1000);
}

void play_ext_sound() {
    string soundinp;
    cout << "Enter sound file path: ";
    getline(cin, soundinp);
    soundinp = trim(soundinp);

    if (soundinp.empty()) {
        cout << "No file specified." << el;
        return;
    }

    // basic dangerous-char check to reduce command injection risk
    if (soundinp.find_first_of("&;|<>") != string::npos) {
        cout << "Invalid file path." << el;
        return;
    }

    cout << "Playing external sound..." << el;

    // quote the script and the argument
    string command = "python src\\sounddriver.py \"" + soundinp + "\"";

    // try executing
    int rc = system(command.c_str());
    if (rc == -1) {
        cout << "Failed to run player." << el;
    }
}

void calculator() { // soon to be scientific_calculator()
    char op;
    int num1, num2;
    int result;
    cout << "Enter operation (+, -, *, /): ";
    cin >> op;
    cout << "Enter #1: ";
    cin >> num1;
    cout << "Enter #2: ";
    cin >> num2;

    switch (op) {
        case '+':
            result = num1 + num2;
            cout << "Result: " << result << el;
            break;
        case '-':
            result = num1 - num2;
            cout << "Result: " << result << el;
            break;
        case '*':
            result = num1 * num2;
            cout << "Result: " << result << el;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
                cout << "Result: " << result << el;
            } else {
                cout << "Error: Division by zero." << el;
            }
            break;
        default:
            cout << "Invalid operation." << el;
            break;
    }
}

void powerop() {
    cout << "Power options: \n1. Restart\n2. Shutdown\n3. Sign out\n>> ";
    string powerinp;
    getline(cin, powerinp);
    if (powerinp == "Restart" || powerinp == "restart") {
        cout << "Restarting..." << el;
        restart();
    } else if (powerinp == "Shutdown" || powerinp == "shutdown") {
        cout << "Shutting down..." << el;
        // system(R"(taskkill /IM "build398.exe" /F > $null 2>&1)");
        system(R"(taskkill /IM "build398.exe" /F 2>&1)");
        // system(R"(del /f "D:\Console AndroidOS\bin\$null")");
        clear_console();
        exit(0);
    } else if (powerinp == "Sign out" || powerinp == "sign out" || powerinp == "signout") {
        login();
    } else {
        cout << "Invalid option." << el;
    }
}

void newui() {
    if (activated) {
        cout << "Launching 2025-12-25 UI..." << el;
        sleep.delay(2000);
        #ifdef _WIN32
        int clearoldui = system("cls");
        #else
        int clearolduiunix = system("clear");
        #endif
        if (!activated) {
            cout << "Your system is not activated. Some features may be limited." << el;
            cout << "Welcome to the new UI!" << el;
        }
        else if (current_user == "Developer" || current_user == "Tester" && activated) {
            cout << "Welcome to the new UI, Developer/Tester!" << el;
        }
        // --- ADDED: Update the path string inside the loop ---
        while (true) {
            // filesystem::path current_path = filesystem::current_path();
            // Use generic_string() to format the path with forward slashes globally
            // string pathString = current_path.generic_string(); 
            MEMORYSTATUSEX memInfo;
            memInfo.dwLength = sizeof(memInfo);
            double availMB = 0.0;
            if (GlobalMemoryStatusEx(&memInfo)) {
                availMB = static_cast<double>(memInfo.ullAvailPhys) / (1024 * 1024);
            }
            cout << "\n[RAM: " << fixed << setprecision(1) << availMB << " MB free]\n";
            cout << "\nD:/Console AndroidOS> ";
            string newuiinput;
            getline(cin, newuiinput);
            if ((newuiinput == "apps") && (current_user == "Developer" || current_user == "Tester")) {
                cout << "\nApps (Developer/Tester Mode):" << el;
                cout << "Task Manager\n";
                cout << "Recycle Bin\n";
                cout << "Firewall Settings\n";
                cout << "File Search\n";
                cout << "System Benchmark\n";
                cout << "Bluetooth Manager\n";
                cout << "Dark Mode Toggle\n";
                cout << "Android Defender\n";
                cout << "Task Scheduler\n";
                cout << "Hibernate Mode\n";
                cout << "Lock Screen\n";
                // cout << "Exit\n";
                cout << "Device Manager\n";
                // cout << "Restart\n";
                cout << "File Explorer\n";
                cout << "File Opener\n";
                cout << "Update\n";
                cout << "Activate\n";
                cout << "Code Editor\n";
                cout << "Launch External App\n";
                cout << "Text Editor\n";
                cout << "Launch Vim Editor\n";
                cout << "Environment Variables\n";
                cout << "Play External Sound\n";
                // cout << "23. New UI mode\n";
                cout << "(Developer/Tester) Power\n";
            }
            else if (newuiinput == "apps") {
                cout << "\nApps:" << el;
                cout << "Task Manager\n";
                cout << "Recycle Bin\n";
                cout << "Firewall Settings\n";
                cout << "File Search\n";
                cout << "System Benchmark\n";
                cout << "Bluetooth Manager\n";
                cout << "Dark Mode Toggle\n";
                cout << "Android Defender\n";
                cout << "Task Scheduler\n";
                cout << "Hibernate Mode\n";
                cout << "Lock Screen\n";
                cout << "Exit\n";
                cout << "Device Manager\n";
                cout << "Restart\n";
                cout << "File Explorer\n";
                cout << "File Opener\n";
                cout << "Update\n";
                cout << "Activate\n";
                cout << "Code Editor\n";
                cout << "Launch External App\n";
                cout << "Text Editor\n";
                cout << "Environment Variables\n";
                cout << "Play External Sound\n";
                cout << "Sign out\n";
            }
            else if (newuiinput == "calculator") {
                calculator();
            }
            else {
                // if constexpr (is_same_v<decltype(newuiinput), string>) {
                //    cout << el;
                // }
                // else {
                //    cerr << "Syntax error: Cannot find command '" << newuiinput << "'" << el;
                // }
                if (!newuiinput.empty() && newuiinput != "apps" && newuiinput != "calculator") {
                    vector<string>& valid_apps = (current_user == "Developer" || current_user == "Tester") ? new_ui_apps_dev : new_ui_apps;
                    if (find(valid_apps.begin(), valid_apps.end(), newuiinput) == valid_apps.end()) {
                        cerr << "Syntax error: Cannot find command '" << newuiinput << "'" << el;
                    }
                }
            }

            if (newuiinput == "taskmgr")
            {
                task_manager();
            }
            else if (newuiinput == "recycle")
            {
                recycle_bin_manager();
            }
            else if (newuiinput == "firewall")
            {
                firewall_settings();
            }
            else if (newuiinput == "search_files")
            {
                file_search();
            }
            else if (newuiinput == "benchmk")
            {
                system_benchmark();
            }
            else if (newuiinput == "bluetooth")
            {
                bluetooth_manager();
            }
            else if (newuiinput == "contrast")
            {
                dark_mode_toggle();
            }
            else if (newuiinput == "security")
            {
                windows_defender();
            }
            else if (newuiinput == "scheduler")
            {
                task_scheduler();
            }
            else if (newuiinput == "hibernate")
            {
                hibernate_mode();
            }
            else if (newuiinput == "lock")
            {
                lock_screen();
            }
            else if (newuiinput == "shutdown" || newuiinput == "exit")
            {
                cout << "Shutting down..." << el;
                system(R"(taskkill /IM "build398.exe" /F > $null 2>&1)");
                system(R"(del /f "D:\Console AndroidOS\bin\$null")");
                clear_console();
                Beep(1000, 2000);
                cout << "It is now safe to turn off your computer." << el;
                delay(1000);
                exit(0);
            }
            else if (newuiinput == "devmgr")
            {
                devicemgr();
            }
            else if (newuiinput == "restart")
            {
                cout << "Restarting...";
                restart();
            }
            else if (newuiinput == "power" && (current_user == "Developer" || current_user == "Tester") && activated) {
                powerop();
            }
            else if (newuiinput == "explorer")
            {
                file_explorer();
            }
            else if (newuiinput == "fileopen")
            {
                file_opener();
            }
            else if (newuiinput == "update")
            {
                update();
            }
            else if (newuiinput == "activate")
            {
                activate();
            }
            else if (newuiinput == "codeedit")
            {
                code_editor();
            }
            else if (newuiinput == "launch_ext_app")
            {
                cout << "Are you sure you want to launch the external app? (Y/N): ";
                char conf;
                cin >> conf;
                if (conf == 'Y' || conf == 'y') {
                    security_comp = true;
                    if (security_comp) {
                        cout << "Security compromised. Android Defender has stopped the threat." << el;
                    }
                } else {
                    cout << "Cancelled launching external app." << el;
                }
            }
            else if (newuiinput == "textedit")
            {
                get_input2();
            }
            else if (newuiinput == "vim")
            {
                launch_vim();
            }
            else if (newuiinput == "envvar")
            {
                env_var();
            }
            else if (newuiinput == "playsound")
            {
                play_ext_sound();
            }
            /*
            else if (newuiinput == "exit")
            {
                break; // exit the new UI
            }
            */
            else if (newuiinput == "signout")
            {
                cout << "Signing out from Developer/Tester mode..." << el;
                sleep.delay(1000);
                login();
            }
            else
            {
                cout << "Invalid option." << el;
            }
        }
    }
}

bool isProcessRunning(const std::wstring& processName) {
    PROCESSENTRY32 info;
    info.dwSize = sizeof(info);
    // Take a snapshot of all processes
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE) {
        return false; // Error creating snapshot
    }

    // Retrieve information about the first process
    if (Process32First(snapshot, &info)) {
        do {
            // Compare the executable file name with the target name
            std::string exeStr(info.szExeFile);
            std::wstring exeWstr(exeStr.begin(), exeStr.end());
            if (processName == exeWstr) {
                CloseHandle(snapshot);
                return true; // Found the process
            }
        } while (Process32Next(snapshot, &info)); // Move to the next process
    }

    CloseHandle(snapshot);
    return false; // Did not find the process
}

void oldui() {
    if (!activated) {
        sleep.delay(2000);
        clear_console();
        cout << "Your system is not activated. Some features may be limited." << el;
        cout << "\nApps:" << el;
        cout << "STILL IN DEVELOPMENT" << el;
        cout << "1. Task Manager\n"; // 0x01
        cout << "2. Recycle Bin\n"; // 0x02
        cout << "3. Firewall Settings\n"; // 0x03
        cout << "4. File Search\n"; // 0x04
        cout << "5. System Benchmark\n"; // 0x05
        cout << "6. Bluetooth Manager\n"; // 0x06
        cout << "7. Dark Mode Toggle\n"; // 0x07
        cout << "8. Android Defender\n"; // 0x08
        cout << "9. Task Scheduler\n"; // 0x09 
        cout << "10. Hibernate Mode\n"; // 0x0A
        cout << "11. Lock Screen\n"; // 0x0B
        cout << "12. Exit\n"; // 0x0C
        cout << "13. Device Manager\n"; // 0x0D
        cout << "14. Restart\n"; // 0x0E
        cout << "15. File Explorer\n"; // 0x0F
        cout << "16. File Opener\n"; // 0x10
        cout << "17. Update\n"; // 0x11
        cout << "18. Activate\n"; // 0x12
        cout << "19. Code Editor\n"; // 0x13
        cout << "19.1 Launch External App\n"; // 0x14
        cout << "20. Text Editor\n"; // 0x15
        cout << "21. Environment Variables\n"; // 0x16
        cout << "22. Play External Sound\n"; // 0x17
        cout << "Select: ";
        getline(cin, input);
    }
    else if (userin == "Developer" || userin == "Tester" && activated) {
        sleep.delay(2000);
        clear_console();
        cout << "\nApps (Developer/Tester Mode):" << el;
        cout << "1. Task Manager\n"; // 0x01
        cout << "2. Recycle Bin\n"; // 0x02
        cout << "3. Firewall Settings\n"; // 0x03
        cout << "4. File Search\n"; // 0x04
        cout << "5. System Benchmark\n"; // 0x05
        cout << "6. Bluetooth Manager\n"; // 0x06
        cout << "7. Dark Mode Toggle\n"; // 0x07
        cout << "8. Android Defender\n"; // 0x08
        cout << "9. Task Scheduler\n"; // 0x09
        cout << "10. Hibernate Mode\n"; // 0x0A
        cout << "11. Lock Screen\n"; // 0x0B
        cout << "12. Exit\n"; // 0x0C
        cout << "13. Device Manager\n"; // 0x0D
        cout << "14. Restart\n"; // 0x0E
        cout << "15. File Explorer\n"; // 0x0F
        cout << "16. File Opener\n"; // 0x10
        cout << "17. Update\n"; // 0x11
        cout << "18. Activate\n"; // 0x12
        cout << "19. Code Editor\n"; // 0x13
        cout << "19.1 Launch External App\n"; // 0x14
        cout << "20. Text Editor\n"; // 0x15
        cout << "20+vim. Launch Vim Editor\n"; // 0x15
        cout << "21. Environment Variables\n"; // 0x16
        cout << "22. Play External Sound\n"; // 0x17
        cout << "23. New UI mode\n"; // 0x18
        cout << "Select: ";
        getline(cin, input);
    }
    else {
        sleep.delay(2000);
        clear_console();
        cout << "\nApps:" << el;
        cout << "1. Task Manager\n";
        cout << "2. Recycle Bin\n";
        cout << "3. Firewall Settings\n";
        cout << "4. File Search\n";
        cout << "5. System Benchmark\n";
        cout << "6. Bluetooth Manager\n";
        cout << "7. Dark Mode Toggle\n";
        cout << "8. Android Defender\n";
        cout << "9. Task Scheduler\n";
        cout << "10. Hibernate Mode\n";
        cout << "11. Lock Screen\n";
        cout << "12. Exit\n";
        cout << "13. Device Manager\n";
        cout << "14. Restart\n";
        cout << "15. File Explorer\n";
        cout << "16. File Opener\n";
        cout << "17. Update\n";
        cout << "18. Activate\n";
        cout << "19. Code Editor\n";
        cout << "19.1 Launch External App\n";
        cout << "20. Text Editor\n";
        cout << "21. Environment Variables\n";
        cout << "22. Play External Sound\n";
        cout << "Select: ";
        getline(cin, input);
    }

    if (input == "1")
    {
        task_manager();
    }
    else if (input == "2")
    {
        recycle_bin_manager();
    }
    else if (input == "3")
    {
        firewall_settings();
    }
    else if (input == "4")
    {
        file_search();
    }
    else if (input == "5")
    {
        system_benchmark();
    }
    else if (input == "6")
    {
        bluetooth_manager();
    }
    else if (input == "7")
    {
        dark_mode_toggle();
    }
    else if (input == "8")
    {
        windows_defender();
    }
    else if (input == "9")
    {
        task_scheduler();
    }
    else if (input == "10")
    {
        hibernate_mode();
    }
    else if (input == "11")
    {
        lock_screen();
    }
    else if (input == "12")
    {
        cout << "Shutting down..." << el;
        system(R"(taskkill /IM "build398.exe" /F > $null 2>&1)");
        system(R"(del /f "D:\Console AndroidOS\bin\$null")");
        clear_console();
        Beep(1000, 2000);
        cout << "It is now safe to turn off your computer." << el;
        delay(1000);
        exit(0);
    }
    else if (input == "13")
    {
        devicemgr();
    }
    else if (input == "14")
    {
        cout << "Restarting...";
        restart();
    }
    else if (input == "15")
    {
        file_explorer();
    }
    else if (input == "16")
    {
        file_opener();
    }
    else if (input == "17")
    {
        update();
    }
    else if (input == "18")
    {
        activate();
    }
    else if (input == "19")
    {
        code_editor();
    }
    else if (input == "19.1")
    {
        cout << "Are you sure you want to launch the external app? (Y/N): ";
        char conf;
        cin >> conf;
        if (conf == 'Y' || conf == 'y') {
            security_comp = true;
            if (security_comp) {
                cout << "Security compromised. Android Defender has stopped the threat." << el;
            }
        } else {
            cout << "Cancelled launching external app." << el;
        }
    }
    else if (input == "20")
    {
        get_input2();
    }
    else if (input == "20+vim")
    {
        launch_vim();
    }
    else if (input == "21")
    {
        env_var();
    }
    else if (input == "22")
    {
        play_ext_sound();
    }
    else if (input == "23" && (userin == "Developer" || userin == "Tester") && activated)
    {
        newui();
    }
    else
    {
        cout << "Invalid option." << el;
    }
}

void setdefault() {
    cout << "Resetting newui() to default state..." << el;
    
    // Read default UI from default.h
    ifstream default_file("src\\default.h");
    if (!default_file.is_open()) {
        cerr << "Failed to open default.h" << el;
        return;
    }
    
    vector<string> default_content;
    string line;
    bool in_function = false;
    int brace_count = 0;
    
    // Extract the newui() function from default.h
    while (getline(default_file, line)) {
        if (line.find("newui()") != string::npos) {
            in_function = true;
            brace_count = 0;
        }
        
        if (in_function) {
            default_content.push_back(line);
            
            for (char c : line) {
                if (c == '{') brace_count++;
                if (c == '}') brace_count--;
            }
            
            if (brace_count == 0 && in_function && line.find("}") != string::npos) {
                in_function = false;
                break;
            }
        }
    }
    default_file.close();
    
    // Read the current secup2.cxx file
    ifstream source_file("src\\secup2.cxx");
    vector<string> source_content;
    vector<string> current_newui;
    int func_start = -1;
    int func_end = -1;
    brace_count = 0;
    int line_count = 0;
    bool found_start = false;
    
    while (getline(source_file, line)) {
        source_content.push_back(line);
        
        if (line.find("newui()") != string::npos && func_start == -1) {
            func_start = line_count;
            brace_count = 0;
            found_start = true;
        }
        
        if (found_start && func_end == -1) {
            current_newui.push_back(line);
            for (char c : line) {
                if (c == '{') brace_count++;
                if (c == '}') brace_count--;
            }
            
            if (brace_count == 0 && line.find("}") != string::npos && found_start) {
                func_end = line_count;
                found_start = false;
            }
        }
        
        line_count++;
    }
    source_file.close();
    
    // Check if current UI matches default
    if (func_start != -1 && func_end != -1 && current_newui.size() == default_content.size()) {
        bool is_default = true;
        for (size_t i = 0; i < current_newui.size(); i++) {
            if (current_newui[i] != default_content[i]) {
                is_default = false;
                break;
            }
        }
        
        if (is_default) {
            cout << "newui() is already in default state. No action needed." << el;
            return;
        }
    }
    
    // Replace the function
    if (func_start != -1 && func_end != -1 && func_end > func_start) {
        source_content.erase(source_content.begin() + func_start, source_content.begin() + func_end + 1);
        source_content.insert(source_content.begin() + func_start, default_content.begin(), default_content.end());
        
        // Write back to file
        ofstream output_file("src\\secup2.cxx");
        if (output_file.is_open()) {
            for (const string& content_line : source_content) {
                output_file << content_line << "\n";
            }
            output_file.close();
            cout << "newui() has been reset to default state successfully." << el;
            cout << "Changes saved. Please recompile and restart the OS." << el;
            cout << "Exiting..." << el;
            sleep.delay(1000);
            system("oscompiler");
        }
        else {
            cerr << "Failed to write to secup2.cxx" << el;
        }
    }
    else {
        cerr << "Could not find complete newui() function in secup2.cxx" << el;
    }
}

void read_driver() {
    ifstream driver_file("driver.aos");
    if (!driver_file.is_open()) {
        cerr << "Failed to open graphics driver" << el;
        return;
    }
    string line;
    vector<string> graphics = {"DLSS", "Direct", "NVIDIA"};
    while (getline(driver_file, line)) {
        line = trim(line);
        for (int i = 0; i < graphics.size(); i++) {
            if (line.find(graphics[i]) == 0) {
                graphics_driver = true;
                cout << "Graphics driver loaded: " << graphics[i] << el;
                break;
            }
        }
    }
}

// Main Menu
int main()
{
    thread t(check_memory);
    clear_console();
    check_exp();
    system("title main");
    // previously: ofstream genuine("genuine.txt", ios::app | ios::in);
    if (isProcessRunning(L"uefi.exe")) {
        cout << ":)" << endl;
    } else {
        return 0;
    }

    ifstream userFile("users.txt");
    ifstream allowFile("allow.flag");
    string flagWord;
    string allowWord;
    bool allow_flag = false;

    cout << "Reading users.txt..." << endl;
    if (userFile.is_open()) {
        string line;
        while (getline(userFile, line)) {
            line = trim(line);
            if (line.empty())
                continue; // skip blank lines
            // ignore obvious headers or comments
            if (line.front() == '#' || line.front() == ';')
                continue;

            string detectedUser;
            // look for lines starting with "username=" (case-sensitive)
            if (line.rfind("username=", 0) == 0) {
                detectedUser = trim(line.substr(strlen("username=")));
                // credentials may follow the actual username separated by commas;
                // keep only the first field as the username
                size_t comma = detectedUser.find(',');
                if (comma != string::npos) {
                    string raw = detectedUser.substr(0, comma);
                    detectedUser = trim(raw);
                }
            } else if (line.find(' ') == string::npos && line.find('=') == string::npos) {
                // fallback: a single word with no spaces or '=' is treated as username
                detectedUser = line;
            } else {
                // if the line isn't a plain username, skip it
                continue;
            }

            if (!detectedUser.empty()) {
                // avoid duplicates (case-insensitive)
                bool exists = false;
                for (const auto &u : users) {
                    if (_stricmp(u.c_str(), detectedUser.c_str()) == 0) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    users.push_back(detectedUser);
                    cout << "Added user from file: " << detectedUser << endl;
                }
            }
        }
    } else {
        cerr << "Unable to open users.txt (0x03)" << endl;
    }
    // debug: show all users we currently have
    cout << "Final user list:";
    for (const auto &u : users) {
        cout << " " << u;
    }
    cout << endl;

    sleep.delay(1000);
    cout << "Reading allow.flag..." << endl;
    while (allowFile >> allowWord) {
        if (allowWord == "allow=true") {
            allow_flag = true;
            break;
        }
    }
    sleep.delay(1000);
    
    signal(SIGINT, detect_forced_shutdown);

    system("color 0F");
    string input;
    login();
    if (update_available)
    {
        cout << "Build 420 has reached it's EOL. Please update to build 420 to get the lastest features." << el;
        cout << "Stay in B420 at your own risk." << el;
    }
    bootloader();

    // load persisted activation state (do NOT create keys here)
    load_activation_state();

    // load/read genuine state (do NOT overwrite)
    genuine_flag = read_genuine_state();

    // Check for reset flag and reset to default if present
    ifstream reset_check(RESET_FLAG_PATH);
    if (reset_check.is_open()) {
        reset_check.close();
        cout << "Reset flag detected. Do you want to reset to default UI? (Y/N): ";
        char confirm;
        cin >> confirm;
        if (confirm == 'Y' || confirm == 'y') {
            setdefault();
            // Delete the reset flag
            remove(RESET_FLAG_PATH.c_str());
        } else {
            cout << "Reset cancelled. Continuing with current UI." << el;
        }
    }

    // if already activated but no genuine record exists, create one

    // If system is not genuine, show message and exit with code 0x12
    if (!genuine_flag) {
        cout << "\nSystem not genuine. Returning code 0x0C" << el;
        return 1;
    }

    read_driver();

    if (graphics_driver) {
        while (true)
        {
            if (system_locked)
            {
                lock_screen();
            }
            
            // Non-blocking key check
            if (_kbhit()) {
                int key = _getch();
                if (key == 'd' || key == 'D') {
                    setdefault();
                }
            }
            
            newui();
        }
    }
    else {
        return 0;
    }
}

/*
How do I even compile this?

g++ -std=c++23 secup2.cxx -o "D:\Console AndroidOS\bin\secup2.exe"

Yes, Build 420 uses C++23 (i might use C++26 in the future)

AND YES, I HAVE TO HARDTYPE THE PATH, BECAUSE OF PATH ISSUES.
*/

// Answering questions:
// Q: WHY USE BOTH DELAYS???
// A: consoledelay.h is the cross-platform one, sleepdelay.h is the same, i just like both.
// Q: Why C++?
// A: BECAUSE IT'S FAST.
// Q: Why not C?
// A: I suck at C. WHAT THE HELL IS char* EVEN???
// Q: Why not Python/JavaScript/Java/C#?
// A: Python I like, but it's slow. JS IS FOR WEB. AND I HATE JAVA/C#, WHAT THE F[BEEP] IS THE JVM, WHAT LANGUAGE NEEDS A VM????
// Q: Why simulate an OS in console?
// A: Because I like it.
// Q: Why is this code so messy?
// A: IT'S NOT MESSY, IT'S COMPLEX.
// Q: Are there any bugs?
// A: There are... quite a few. Every software has bugs. Windows has bugs, macOS has bugs, Linux has bugs, even AndroidOS has bugs. Deal with it.
// Q: Will there be more builds?
// A: Yes, build 508 is the next one.
// Q: Will the source code be open-sourced?
// A: It's on GitHub.
// Q: Can I contribute?
// A: Sure, fork it and make a pull request.
// Q: Why flags?
// A: Because I need persistence.
// Q: Why not use a database?
// A: BECAUSE I DON'T KNOW HOW TO USE DATABASES IN C++.
// Q: Why not use JSON/XML/YAML for config files?
// A: that's for web.
// Q: Why use system("command")?
// A: BECAUSE THAT IS THE ONLY WAY TO DO IT IN C++ WITHOUT EXTERNAL LIBRARIES.
// Q: Will there be a GUI version?
// A: nope.
// Q: Why GNU?
// A: BECAUSE IT'S FREE SOFTWARE.
