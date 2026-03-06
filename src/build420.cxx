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
#include <cstdint>
#include "example_threat.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using namespace std;

#define el "\n"

ofstream keyfile;

SLEEP sleep; // externed

int time_wasted_here = 1;
const int version = 420;
const int kernel_version = 30;
const string kernel_name = "x25";  
const int yeol = 2042;
const int meol = 1;
const int deol = 16;

// Maps are so f[beep]ing confusing
map<string, string> files = 
{
    {"ANDROID_OS.ISO", "System boot image"}, 
    {"WARNING_KNL.EXE", "Critical system file"}, 
    {"README.TXT", "System documentation"}, 
    {"TASKS.LOG", "Task execution history"},
    {"CONFIG.SYS", "System configuration file"}
};

vector<string> recycle_bin;
vector<string> installed_apps = {"File Explorer", "Calculator", "System Info"};
map<string, string> app_store = 
{
    {"Notepad", "A simple text editor"},
    {"Weather", "Shows the current weather"},
    {"Media Player", "Plays audio and video files"}
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
bool update_available = true;
bool reached_eol = false;

uint64_t some_variable = 19531234;

void check_exp()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;

    if (year > yeol) {
        reached_eol = true;
    } else if (year == yeol) {
        if (month > meol) {
            reached_eol = true;
        } else if (month == meol) {
            if (day >= deol) {
                reached_eol = true;
            }
        }
    }
}

// NEW: paths for activation keys and flag
const string KEYS_PATH = "D:\\Console AndroidOS\\src\\keys\\keys.txt";
const string ACT_PATH  = "D:\\Console AndroidOS\\src\\keys\\activated.flag";

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
    cout << "Choose an user (Guest, Admin): ";
    getline(cin, userin);
    if (userin == users[1])
    {
        cout << "Logging in as Guest..." << endl;
    }
    else if (userin == users[0])
    {
        cout << "Enter in the password: ";
        getline(cin, passin);
        if (passin == passwords[1])
        {
            cout << "Logging in as Admin..." << endl;
        }
        else
        {
            cerr << "Incorrect password." << endl;
        }
    }
    else if (userin == users[2])
    {
        cout << "Enter in the password: ";
        getline(cin, passin);
        if (passin == passwords[1])
        {
            cout << "Logging in as Developer..." << el;
        }
        else
        {
            cerr << "Incorrect password." << el;
        }
    }
    else if (userin == users[3])
    {
        cout << "Enter in the password: ";
        cin >> passin2;
        if (passin2 == long_pass)
        {
            cout << "Logging in as Tester..." << el;
        }
        else
        {
            cerr << "Incorrect password." << el;
        }
    }
    else
    {
        cerr << "Invalid user." << el;
    }
}

// Task Manager
void task_manager()
{
    cout << "\nTASK MANAGER - Running Processes" << el;
    cout << "--------------------------------" << el;
    cout << "1. SYSTEM_KERNEL - Running" << el;
    cout << "2. FILE_MANAGER - Idle" << el;
    cout << "3. NETWORK_SERVICE - Running" << el;
    cout << "4. GRAPHICS_ENGINE - Running" << el;
    cout << "5. BACKGROUND_TASK - Sleeping" << el;
    cout << "--------------------------------" << el;
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

// System Benchmark
void system_benchmark()
{
    cout << "\nRUNNING SYSTEM BENCHMARK..." << el;
    this_thread::sleep_for(chrono::milliseconds(2000));
    cout << "CPU Performance: 85%\nRAM Speed: 3200MHz\nDisk Speed: 500MB/s" << el;
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
    cout << "No threats detected." << el;
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
    cout << "Launching RAM Test..." << endl;
    this_thread::sleep_for(chrono::seconds(5));

    // single-line progress indicator (updates in-place)
    cout << "RAM Test... ";
    cout.flush();
    for (int pct = 0; pct <= 164; ++pct) { // rounded megabytes count for 16384 KB
        cout << "\rRAM Test... " << setw(3) << pct << "MB " << flush;
        this_thread::sleep_for(chrono::milliseconds(15));
    }
    cout << " done" << el;

    int failure = rand() % 100;
    if (failure > 20)
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
                    cout << "Exiting..." << el;
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
                cout << "Exiting..." << el;
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
    system("build420.exe");
}

void file_explorer()
{
    cout << "\nAll files: " << el;
    for (const auto& pair : files)
    {
        cout << pair.first << ": " << pair.second << el;
    }
    system("\n");
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
    cout << "Updating to build: 420 Security Update 2" << endl;
    if (update_available == false)
    {
        cerr << "There is no updates... yet." << el;
    }
    else
    {
        system("uefi.exe");
    }
}

bool activated = false;

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

// Main Menu
int main()
{
    check_exp();
    system("title main");
    clear_console();
    system("color 0F");
    string input;
    login();
    if (reached_eol)
    {
        cout << "Build 420 has reached it's EOL. Please update to build 420 to get the lastest features." << el;
        cout << "Stay in B420 at your own risk." << el;
    }
    bootloader();

    // load persisted activation state (do NOT create keys here)
    load_activation_state();

    while (true)
    {
        if (system_locked)
        {
            lock_screen();
        }
        if (!activated) {
            sleep.delay(2000);
            clear_console();
            cout << "Your system is not activated. Some features may be limited." << el;
            cout << "\nApps:" << el;
            cout << "STILL IN DEVELOPMENT" << el;
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
            delay(1000);
            return 0;
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
                launch_external_app();
            } else {
                cout << "Launch cancelled." << el;
            }
        }
        else
        {
            cout << "Invalid option." << el;
        }
    }
}

/*
How do I even compile this?

g++ -std=c++23 build420.cxx -o "D:\Console AndroidOS\bin\build420.exe"

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
// A: Yes, build 420 is the next one.
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