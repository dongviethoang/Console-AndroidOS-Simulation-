/*
This is the main file of the Console AndroidOS simulation.
Don't mess with anything.
And if you want to fix it, unless you're a C++ developer, download the ZIP again. I am not gonna travel to your house.

- Hoang Viet (2025)
*/
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
#include <sleepdelay.h>
#include <csignal>
#include <fstream>
#include <consoledelay.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using namespace std;

#define el "\n"

int time_wasted_here = 1;
int version = 302;

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
vector<string> users = {"Admin", "Guest", "Developer"};
string passwords[2] = {"12345678", "viethoang082013"};
string current_user = "guest";
string theme = "Light";
bool system_locked = false;
bool update_available = false;

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

// Windows Defender
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
    cout << "Copyright Beta Corporation (c). All rights reserved" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Console AndroidOS" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Build 302 (Kernel Version 20)" << endl;
    this_thread::sleep_for(chrono::seconds(5));
    cout << "Launching RAM Test..." << endl;
    for (int i = 1; i <= 65535; i++)
    {
        cout << "RAM Test: " << i << endl;
    }
    int failure = rand() % 100;
    if (failure > 20)
    {
        cout << "RAM test results: Nominal" << el;
    }
    else
    {
        cout << "RAM test failure. Press [ENTER] to exit";
        string key;
        getline(cin, key);
        if (key == "\n")
        {
            exit(1);
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
    system("conand.exe");
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
        raise(SIGSEGV); // simulated crash
    }
    else if (filein == "WARNING_KNL.EXE")
    {
        for (int i = 3; i >= 0; i--)
        {
            cerr << "WARNING: System crash in: " << i << el;
            delay(1000);
            system("clear");
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
    cout << "Updating to build: 234 LTSC" << endl;
    sleep.delay(1000);
    system("\"D:\\Console AndroidOS\\bin\\setup.exe\"");
}

void dos_mode()
{
    cout << "Restarting in DOS mode..." << endl;
    sleep.delay(1000);

#ifdef _WIN32
    // spawn a known dos binary instead of system("dos")
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    LPCSTR exePath = "D:\\Console AndroidOS\\bin\\dos.exe"; // ensure this exists
    if (!CreateProcessA(exePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        cerr << "Failed to start DOS mode: error " << GetLastError() << endl;
    } else {
        // optionally wait for the DOS session to end:
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
#else
    // POSIX: exec a DOS emulator (e.g. dosbox) or an executable named "dos" in PATH
    pid_t pid = fork();
    if (pid == 0) {
        // child
        execlp("dos", NULL); // replace with your emulator/binary
        _exit(127); // exec failed
    } else if (pid > 0) {
        // parent waits
        int status = 0;
        waitpid(pid, &status, 0);
    } else {
        cerr << "Failed to fork for DOS mode." << endl;
    }
#endif
}

void check_command_line() 
{
    #ifdef _WIN32
    std::wstring cmdW = L"python src\\command_line_check.py";
    std::vector<wchar_t> cmdBuf(cmdW.begin(), cmdW.end());
    cmdBuf.push_back(0);

    // create pipe for child's stdout/stderr
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE hOutRead = NULL, hOutWrite = NULL;
    if (!CreatePipe(&hOutRead, &hOutWrite, &sa, 0)) {
        cerr << "Error: failed to create pipe." << el;
        return;
    }
    // parent should not inherit the read handle
    SetHandleInformation(hOutRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hOutWrite;
    si.hStdError  = hOutWrite;
    si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);

    ZeroMemory(&pi, sizeof(pi));

    BOOL ok = CreateProcessW(
        NULL,
        cmdBuf.data(),   
        NULL,
        NULL,
        TRUE,              // inherit handles so child uses pipe write handle
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    // Close the writer in parent; child has a copy
    CloseHandle(hOutWrite);

    if (!ok) {
        cerr << "Error: CLICP failed (" << GetLastError() << ")." << el;
        CloseHandle(hOutRead);
        return;
    }

    // Read child's output and forward to this process' stdout
    CHAR buf[4096];
    DWORD readBytes = 0;
    while (true) {
        BOOL readOk = ReadFile(hOutRead, buf, sizeof(buf) - 1, &readBytes, NULL);
        if (readOk && readBytes > 0) {
            buf[readBytes] = '\0';
            cout << buf << flush;
            continue;
        }
        // If no data, check if process exited
        DWORD wait = WaitForSingleObject(pi.hProcess, 50);
        if (wait == WAIT_OBJECT_0) {
            // process ended; drain remaining pipe data
            while (ReadFile(hOutRead, buf, sizeof(buf) - 1, &readBytes, NULL) && readBytes > 0) {
                buf[readBytes] = '\0';
                cout << buf << flush;
            }
            break;
        }
        // otherwise loop and try reading again
    }

    // get exit code
    DWORD exitCode = 0;
    if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
        cerr << "Error 0xC0: GetExitCodeProcess failed (" << GetLastError() << ")." << el;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(hOutRead);
        exit(1);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hOutRead);

    if (exitCode != 0) {
        cerr << "Error 0x01: Cannot run Build 302 in an external CLI. (exit=" << exitCode << ")" << el;
        exit(1);
    }
#else
    // POSIX fallback: run and forward output (simple)
    int rc = system("python3 src/command_line_check.py");
    if (rc != 0) {
        cerr << "Error 0x01: Cannot run Build 302 in an external CLI. (exit=" << rc << ")" << el;
        exit(1);
    }
#endif
}

// Main Menu
int main()
{
    system("clear");
    system("color 0F");

    check_command_line();

    string input;
    login();
    bootloader();
    while (true)
    {
        if (system_locked)
        {
            lock_screen();
        }

        cout << "\nANDROID OS MENU" << el;
        cout << "1. Task Manager\n2. Recycle Bin\n3. Firewall Settings\n4. File Search\n";
        cout << "5. System Benchmark\n6. Bluetooth Manager\n7. Dark Mode Toggle\n";
        cout << "8. Android Defender\n9. Task Scheduler\n10. Hibernate Mode\n";
        cout << "11. Lock Screen\n12. Exit\n13. Device Manager\n14. Restart\n15. File Explorer\n16. File Opener\n17. Update\n18. Restart in DOS mode\nSelect: ";
        getline(cin, input);

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
            system(R"(taskkill /IM "conand.exe" /F > $null 2>&1)");
            system(R"(del /f "D:\Console AndroidOS\bin\$null")");
            system("clear");
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
            dos_mode();
        }
        else
        {
            cout << "Invalid option." << el;
        }
    }
}
