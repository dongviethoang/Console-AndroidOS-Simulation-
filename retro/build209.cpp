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
#include <conio.h>

#ifdef _WIN32
    #include <windows.h>
#endif

using namespace std;

#define el "\n"

// File System
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

vector<string> users = {"admin"};
string current_user = "guest";
string theme = "Light";
bool system_locked = false;

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

// Recycle Bin
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

// Firewall
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

// Bluetooth Manager
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

// Dark Mode Toggle
void dark_mode_toggle()
{
    if (theme == "Light")
    {
        theme = "Dark";
        cout << "Dark mode enabled." << el;
    }
    else
    {
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

// Main Menu
int main()
{
    string input;
    while (true)
    {
        if (system_locked)
        {
            lock_screen();
        }

        bool update_available = true;
        if (update_available) {
            cout << "This build is outdated." << endl;
            cout << "You can no longer use this build." << endl;
            cout << "Press any key to return to SETUP..." << endl;
            while (true) {
                if (_kbhit()) {
                    int code = _getch();
                    if (code == 0 || code == 224) {
                        code = _getch();
                    }
                    cout << "Returning to SETUP..." << endl;
                    Sleep(1000);
                    system(R"("D:\\Console AndroidOS\\bin\\setup.exe")");
                }
            }
        }

        cout << "\nANDROID OS MENU" << el;
        cout << "1. Task Manager\n2. Recycle Bin\n3. Firewall Settings\n4. File Search\n";
        cout << "5. System Benchmark\n6. Bluetooth Manager\n7. Dark Mode Toggle\n";
        cout << "8. Windows Defender\n9. Task Scheduler\n10. Hibernate Mode\n";
        cout << "11. Lock Screen\n12. Exit\nSelect: ";
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
            return 0;
        }
        else
        {
            cout << "Invalid option." << el;
        }
    }
}
