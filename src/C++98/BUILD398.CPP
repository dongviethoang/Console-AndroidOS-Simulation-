/* build398.cpp - Turbo C++ (Borland) compatible rewrite
   Console AndroidOS simulation (classic style)
   Converted to C-style strings and fixed arrays.
   - Author: Converted for Turbo C++
*/

#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <time.h>

#include "sleepdelay.h"
#include "consoledelay.h"

using namespace std;
/* define global sleep instance (declared extern in consoledelay.h) */
SLEEP sleep;

/* Constants */
#define MAX_FILES 16
#define MAX_NAME_LEN 64
#define MAX_DESC_LEN 128
#define MAX_RECYCLE 16
#define MAX_APPS 16
#define MAX_KEYS_PER_FILE 128
#define KEY_LINE_LEN 96

/* Activation file paths (kept as requested) */
const char KEYS_PATH[] = "D:\\Console AndroidOS\\src\\keys\\keys.txt";
const char ACT_PATH[]  = "D:\\Console AndroidOS\\src\\keys\\activated.flag";

/* Helper: trim whitespace (in-place) */
void strtrim(char *s) {
    int i, j;
    /* trim leading */
    i = 0;
    while (s[i] && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i++;
    if (i > 0) {
        j = 0;
        while (s[i]) { s[j++] = s[i++]; }
        s[j] = '\0';
    }
    /* trim trailing */
    i = strlen(s) - 1;
    while (i >= 0 && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) { s[i] = '\0'; i--; }
}

/* File record */
struct FileRecord {
    char name[MAX_NAME_LEN];
    char desc[MAX_DESC_LEN];
};

/* Globals - simple arrays replacing STL containers */
FileRecord files[MAX_FILES];
int files_count = 0;

char recycle_bin[MAX_RECYCLE][MAX_NAME_LEN];
int recycle_count = 0;

char installed_apps[MAX_APPS][MAX_NAME_LEN];
int installed_apps_count = 0;

struct StoreApp {
    char name[MAX_NAME_LEN];
    char desc[MAX_DESC_LEN];
};
StoreApp app_store[MAX_APPS];
int app_store_count = 0;

/* Users (simple) */
char users[3][MAX_NAME_LEN];
char passwords_arr[2][MAX_NAME_LEN];
char current_user[MAX_NAME_LEN];
char theme[16];
int system_locked = 0;
int update_available = 0;
int activated = 0;

/* Forward declarations */
void init_data();
void login();
void task_manager();
void recycle_bin_manager();
void firewall_settings();
void file_search();
void system_benchmark();
void bluetooth_manager();
void dark_mode_toggle();
void windows_defender();
void task_scheduler();
void hibernate_mode();
void lock_screen();
void bootloader();
void devicemgr();
void restart_program();
void file_explorer();
void file_opener();
void update_system();
void activate();
void load_activation_state();
void persist_activation();

/* Utility: clear console */
void clear_console() {
    clrscr();
}

/* Initialize data */
void init_data() {
    files_count = 0;
    /* Populate files */
    strcpy(files[files_count].name, "ANDROID_OS.ISO");
    strcpy(files[files_count].desc, "System boot image"); files_count++;
    strcpy(files[files_count].name, "WARNING_KNL.EXE");
    strcpy(files[files_count].desc, "Critical system file"); files_count++;
    strcpy(files[files_count].name, "README.TXT");
    strcpy(files[files_count].desc, "System documentation"); files_count++;
    strcpy(files[files_count].name, "TASKS.LOG");
    strcpy(files[files_count].desc, "Task execution history"); files_count++;
    strcpy(files[files_count].name, "CONFIG.SYS");
    strcpy(files[files_count].desc, "System configuration file"); files_count++;

    /* installed apps */
    installed_apps_count = 0;
    strcpy(installed_apps[installed_apps_count++], "File Explorer");
    strcpy(installed_apps[installed_apps_count++], "Calculator");
    strcpy(installed_apps[installed_apps_count++], "System Info");

    /* app_store */
    app_store_count = 0;
    strcpy(app_store[app_store_count].name, "Notepad");
    strcpy(app_store[app_store_count].desc, "A simple text editor"); app_store_count++;
    strcpy(app_store[app_store_count].name, "Weather");
    strcpy(app_store[app_store_count].desc, "Shows the current weather"); app_store_count++;
    strcpy(app_store[app_store_count].name, "Media Player");
    strcpy(app_store[app_store_count].desc, "Plays audio and video files"); app_store_count++;

    /* users and passwords */
    strcpy(users[0], "Admin");
    strcpy(users[1], "Guest");
    strcpy(users[2], "Developer");
    strcpy(passwords_arr[0], "12345678");
    strcpy(passwords_arr[1], "viethoang082013");
    strcpy(current_user, "guest");
    strcpy(theme, "Light");

    /* recycle bin */
    recycle_count = 0;

    /* activation */
    activated = 0;
}

/* login */
void login() {
    char buf[128];
    cout << "Choose an user (Guest, Admin): ";
    gets(buf);
    strtrim(buf);
    if (strcmp(buf, users[1]) == 0) {
        cout << "Logging in as Guest..." << '\n';
    } else if (strcmp(buf, users[0]) == 0) {
        cout << "Enter in the password: ";
        gets(buf);
        strtrim(buf);
        if (strcmp(buf, passwords_arr[1]) == 0) {
            cout << "Logging in as Admin..." << '\n';
        } else {
            cout << "Incorrect password." << '\n';
        }
    } else if (strcmp(buf, users[2]) == 0) {
        cout << "Enter in the password: ";
        gets(buf);
        strtrim(buf);
        if (strcmp(buf, passwords_arr[1]) == 0) {
            cout << "Logging in as Developer..." << '\n';
        } else {
            cout << "Incorrect password." << '\n';
        }
    } else {
        cout << "Invalid user." << '\n';
    }
}

/* Task manager */
void task_manager() {
    cout << '\n' << "TASK MANAGER - Running Processes" << '\n';
    cout << "--------------------------------" << '\n';
    cout << "1. SYSTEM_KERNEL - Running" << '\n';
    cout << "2. FILE_MANAGER - Idle" << '\n';
    cout << "3. NETWORK_SERVICE - Running" << '\n';
    cout << "4. GRAPHICS_ENGINE - Running" << '\n';
    cout << "5. BACKGROUND_TASK - Sleeping" << '\n';
    cout << "--------------------------------" << '\n';
}

/* Recycle bin manager */
void recycle_bin_manager() {
    cout << '\n' << "RECYCLE BIN" << '\n';
    if (recycle_count == 0) {
        cout << "Recycle bin is empty." << '\n';
    } else {
        int i;
        cout << "Files in recycle bin:" << '\n';
        for (i = 0; i < recycle_count; ++i) {
            cout << "- " << recycle_bin[i] << '\n';
        }
        cout << "Type 'RESTORE' to recover all files or 'EMPTY' to delete permanently: ";
        char action[64];
        gets(action);
        strtrim(action);
        if (strcmp(action, "RESTORE") == 0) {
            for (i = 0; i < recycle_count; ++i) {
                /* restore: append to files if space */
                if (files_count < MAX_FILES) {
                    strcpy(files[files_count].name, recycle_bin[i]);
                    strcpy(files[files_count].desc, "Restored file");
                    files_count++;
                }
            }
            recycle_count = 0;
            cout << "Files restored successfully." << '\n';
        } else if (strcmp(action, "EMPTY") == 0) {
            recycle_count = 0;
            cout << "Recycle bin emptied." << '\n';
        }
    }
}

/* Firewall */
void firewall_settings() {
    cout << '\n' << "FIREWALL SETTINGS" << '\n';
    cout << "Status: ENABLED" << '\n';
    cout << "Allowed Programs: File Explorer, Calculator, Weather App" << '\n';
    cout << "Blocked Programs: Unknown Sources" << '\n';
}

/* File search */
void file_search() {
    cout << '\n' << "FILE SEARCH" << '\n';
    cout << "Enter filename to search: ";
    char filename[MAX_NAME_LEN];
    gets(filename);
    strtrim(filename);
    int i;
    int found = 0;
    for (i = 0; i < files_count; ++i) {
        if (strcmp(files[i].name, filename) == 0) {
            cout << "File found: " << files[i].name << " - " << files[i].desc << '\n';
            found = 1; break;
        }
    }
    if (!found) {
        cout << "File not found." << '\n';
    }
}

/* System benchmark */
void system_benchmark() {
    cout << '\n' << "RUNNING SYSTEM BENCHMARK..." << '\n';
    sleep.delay(2000);
    cout << "CPU Performance: 85%" << '\n';
    cout << "RAM Speed: 3200MHz" << '\n';
    cout << "Disk Speed: 500MB/s" << '\n';
}

/* Bluetooth manager */
void bluetooth_manager() {
    cout << '\n' << "BLUETOOTH MANAGER" << '\n';
    cout << "Available Devices: " << '\n';
    cout << "1. Wireless Headphones" << '\n';
    cout << "2. Smartwatch" << '\n';
    cout << "3. Bluetooth Speaker" << '\n';
    cout << "Enter device number to connect: ";
    char devbuf[16];
    gets(devbuf);
    int device = atoi(devbuf);
    cout << "Connecting..." << '\n';
    sleep.delay(1000);
    cout << "Device connected successfully." << '\n';
}

/* Dark mode toggle */
void dark_mode_toggle() {
    if (strcmp(theme, "Light") == 0) {
        strcpy(theme, "Dark");
        /* use textcolor if available */
        textcolor(LIGHTGRAY);
        cout << "Dark mode enabled." << '\n';
    } else {
        textcolor(BLACK+WHITE*16); /* best-effort; Turbo specific may vary */
        strcpy(theme, "Light");
        cout << "Light mode enabled." << '\n';
    }
}

/* Windows Defender mock */
void windows_defender() {
    cout << '\n' << "SCANNING SYSTEM FILES FOR THREATS..." << '\n';
    sleep.delay(2000);
    cout << "No threats detected." << '\n';
}

/* Task Scheduler */
void task_scheduler() {
    cout << '\n' << "TASK SCHEDULER" << '\n';
    cout << "Enter task name to schedule: ";
    char taskbuf[128];
    gets(taskbuf);
    cout << "Task '" << taskbuf << "' scheduled successfully." << '\n';
}

/* Hibernate Mode */
void hibernate_mode() {
    cout << '\n' << "System is now in Hibernate Mode. Resume to continue." << '\n';
    sleep.delay(2000);
    cout << "System Resumed!" << '\n';
}

/* Lock Screen */
void lock_screen() {
    system_locked = 1;
    cout << '\n' << "SYSTEM LOCKED. Enter password to unlock: ";
    char pw[64];
    while (1) {
        gets(pw);
        if (strcmp(pw, "password") == 0) {
            system_locked = 0;
            cout << "System Unlocked!" << '\n';
            break;
        } else {
            cout << "Incorrect password. Try again: ";
        }
    }
}

/* Bootloader */
void bootloader() {
    int i;
    cout << "Copyright Beta Corporation (c). All rights reserved" << '\n';
    sleep.delay(1000);
    cout << "Console AndroidOS" << '\n';
    sleep.delay(1000);
    cout << "Build 398 (Kernel Version 25)" << '\n';
    sleep.delay(1000);
    cout << "Launching RAM Test..." << '\n';
    sleep.delay(1000);
    cout << "RAM Test... ";
    for (i = 0; i <= 100; ++i) {
        char buf[32];
        sprintf(buf, "\rRAM Test... %3d%% ", i);
        cprintf(buf);
        sleep.delay(15);
    }
    cout << " done" << '\n';

    int failure = rand() % 100;
    if (failure > 20) {
        cout << "RAM test results: Nominal" << '\n';
    } else {
        cout << "RAM test failure. Press any key to exit" << '\n';
        while (!kbhit()) { sleep.delay(50); }
        getch();
        cout << "Exiting..." << '\n';
        sleep.delay(1000);
        exit(1);
    }
}

/* Device manager */
void devicemgr() {
    char deviceinp[64];
    cout << "Devices: " << '\n';
    cout << "USBs: " << '\n';
    cout << "USB Serial (COM2)" << '\n';
    cout << ">> ";
    gets(deviceinp);
    if (strcmp(deviceinp, "USB Serial") == 0) {
        int i;
        cout << "Drivers: ";
        for (i = 0; i < 4; ++i) {
            if (i == 0) cout << "Realtek ";
            else if (i == 1) cout << "HTC ";
            else if (i == 2) cout << "Windows ";
            else if (i == 3) cout << "AndroidOS USB Driver v1.4.1 ";
        }
        cout << '\n' << ">> ";
        char driverinp_local[128];
        gets(driverinp_local);
        if (strcmp(driverinp_local, "AndroidOS USB Driver v1.4.1") == 0) {
            cout << "Applied driver: AndroidOS USB Driver v1.4.1" << '\n';
        }
    }
}

/* Restart (simple) */
void restart_program() {
    /* Turbo environment restart is nontrivial; suggest user relaunch */
    cout << "Please restart the program manually." << '\n';
}

/* File explorer */
void file_explorer() {
    int i;
    cout << '\n' << "All files: " << '\n';
    for (i = 0; i < files_count; ++i) {
        cout << files[i].name << ": " << files[i].desc << '\n';
    }
}

/* File opener with crash simulation */
void file_opener() {
    cout << '\n' << "Choose which file to open:" << '\n';
    file_explorer();
    cout << ">> ";
    char filein[MAX_NAME_LEN];
    gets(filein);
    strtrim(filein);
    int i, found = -1;
    for (i = 0; i < files_count; ++i) {
        if (strcmp(files[i].name, filein) == 0) { found = i; break; }
    }
    if (found == -1) {
        cout << "File not found." << '\n';
        return;
    }
    if (strcmp(filein, "ANDROID_OS.ISO") == 0) {
        cout << "Loading " << filein << " ..." << '\n';
        sleep.delay(1000);
        cout << "0x02" << '\n';
        sleep.delay(1000);
        /* simulate crash */
        raise(SIGSEGV);
    } else if (strcmp(filein, "WARNING_KNL.EXE") == 0) {
        int j;
        for (j = 3; j >= 0; --j) {
            cout << "WARNING: System crash in: " << j << '\n';
            sleep.delay(1000);
            clear_console();
        }
        raise(SIGSEGV);
    } else {
        cout << "Opening " << files[found].name << " - " << files[found].desc << '\n';
    }
}

/* Update */
void update_system() {
    cout << "Updating to build: 420" << '\n';
    if (!update_available) {
        cout << "There is no updates... yet." << '\n';
    } else {
        /* try to run updater */
        system("build420.exe");
    }
}

/* Activation persistence */
void persist_activation() {
    FILE *af = fopen(ACT_PATH, "w");
    if (!af) {
        cout << "Failed to persist activation to: " << ACT_PATH << '\n';
        return;
    }
    fprintf(af, "activated=true\n");
    fclose(af);
}

/* Load activation */
void load_activation_state() {
    FILE *af = fopen(ACT_PATH, "r");
    if (!af) {
        activated = 0;
        return;
    }
    char line[128];
    activated = 0;
    while (fgets(line, sizeof(line), af)) {
        strtrim(line);
        if (strcmp(line, "activated=true") == 0 || strcmp(line, "activated=1") == 0) {
            activated = 1;
            break;
        }
    }
    fclose(af);
}

/* Activate: read keys file and compare lines */
void activate() {
    char input_key[128];
    cout << "Enter your activation key: ";
    gets(input_key);
    strtrim(input_key);
    if (strlen(input_key) == 0) {
        cout << "No key entered. Activation cancelled." << '\n';
        return;
    }

    FILE *kf = fopen(KEYS_PATH, "r");
    if (!kf) {
        cout << "Activation key store not found: " << KEYS_PATH << '\n';
        cout << "Place a file with one valid key per line and try again." << '\n';
        return;
    }
    char line[KEY_LINE_LEN];
    int found = 0;
    while (fgets(line, sizeof(line), kf)) {
        strtrim(line);
        if (strlen(line) == 0) continue;
        if (strcmp(line, input_key) == 0) { found = 1; break; }
    }
    fclose(kf);

    if (found) {
        persist_activation();
        activated = 1;
        cout << "Activation successful! Thank you for activating Console AndroidOS." << '\n';
    } else {
        cout << "Invalid activation key. Please try again." << '\n';
    }
}

/* Main */
int main(void) {
    srand((unsigned)time(NULL));
    init_data();

    clrscr();
    cprintf("Console AndroidOS (Turbo Edition)\n");

    login();
    bootloader();

    load_activation_state();

    for (;;) {
        if (system_locked) lock_screen();

        if (!activated) {
            sleep.delay(2000);
            clear_console();
            cout << "Your system is not activated. Some features may be limited." << '\n';
            cout << '\n' << "Apps:" << '\n';
            cout << "STILL IN DEVELOPMENT" << '\n';
            cout << "1. Task Manager" << '\n';
            cout << "2. Recycle Bin" << '\n';
            cout << "3. Firewall Settings" << '\n';
            cout << "4. File Search" << '\n';
            cout << "5. System Benchmark" << '\n';
            cout << "6. Bluetooth Manager" << '\n';
            cout << "7. Dark Mode Toggle" << '\n';
            cout << "8. Android Defender" << '\n';
            cout << "9. Task Scheduler" << '\n';
            cout << "10. Hibernate Mode" << '\n';
            cout << "11. Lock Screen" << '\n';
            cout << "12. Exit" << '\n';
            cout << "13. Device Manager" << '\n';
            cout << "14. Restart" << '\n';
            cout << "15. File Explorer" << '\n';
            cout << "16. File Opener" << '\n';
            cout << "17. Update" << '\n';
            cout << "18. Activate" << '\n';
            cout << "Select: ";
            char sel[16];
            gets(sel);
            strtrim(sel);

            if (strcmp(sel, "1") == 0) task_manager();
            else if (strcmp(sel, "2") == 0) recycle_bin_manager();
            else if (strcmp(sel, "3") == 0) firewall_settings();
            else if (strcmp(sel, "4") == 0) file_search();
            else if (strcmp(sel, "5") == 0) system_benchmark();
            else if (strcmp(sel, "6") == 0) bluetooth_manager();
            else if (strcmp(sel, "7") == 0) dark_mode_toggle();
            else if (strcmp(sel, "8") == 0) windows_defender();
            else if (strcmp(sel, "9") == 0) task_scheduler();
            else if (strcmp(sel, "10") == 0) hibernate_mode();
            else if (strcmp(sel, "11") == 0) lock_screen();
            else if (strcmp(sel, "12") == 0) { cout << "Shutting down..." << '\n'; break; }
            else if (strcmp(sel, "13") == 0) devicemgr();
            else if (strcmp(sel, "14") == 0) restart_program();
            else if (strcmp(sel, "15") == 0) file_explorer();
            else if (strcmp(sel, "16") == 0) file_opener();
            else if (strcmp(sel, "17") == 0) update_system();
            else if (strcmp(sel, "18") == 0) activate();
            else cout << "Invalid option." << '\n';
        } else {
            /* Activated menu - same as above but without warning */
            sleep.delay(2000);
            clear_console();
            cout << '\n' << "Apps:" << '\n';
            cout << "1. Task Manager" << '\n';
            cout << "2. Recycle Bin" << '\n';
            cout << "3. Firewall Settings" << '\n';
            cout << "4. File Search" << '\n';
            cout << "5. System Benchmark" << '\n';
            cout << "6. Bluetooth Manager" << '\n';
            cout << "7. Dark Mode Toggle" << '\n';
            cout << "8. Android Defender" << '\n';
            cout << "9. Task Scheduler" << '\n';
            cout << "10. Hibernate Mode" << '\n';
            cout << "11. Lock Screen" << '\n';
            cout << "12. Exit" << '\n';
            cout << "13. Device Manager" << '\n';
            cout << "14. Restart" << '\n';
            cout << "15. File Explorer" << '\n';
            cout << "16. File Opener" << '\n';
            cout << "17. Update" << '\n';
            cout << "18. Activate" << '\n';
            cout << "Select: ";
            char sel2[16];
            gets(sel2);
            strtrim(sel2);

            if (strcmp(sel2, "1") == 0) task_manager();
            else if (strcmp(sel2, "2") == 0) recycle_bin_manager();
            else if (strcmp(sel2, "3") == 0) firewall_settings();
            else if (strcmp(sel2, "4") == 0) file_search();
            else if (strcmp(sel2, "5") == 0) system_benchmark();
            else if (strcmp(sel2, "6") == 0) bluetooth_manager();
            else if (strcmp(sel2, "7") == 0) dark_mode_toggle();
            else if (strcmp(sel2, "8") == 0) windows_defender();
            else if (strcmp(sel2, "9") == 0) task_scheduler();
            else if (strcmp(sel2, "10") == 0) hibernate_mode();
            else if (strcmp(sel2, "11") == 0) lock_screen();
            else if (strcmp(sel2, "12") == 0) { cout << "Shutting down..." << '\n'; break; }
            else if (strcmp(sel2, "13") == 0) devicemgr();
            else if (strcmp(sel2, "14") == 0) restart_program();
            else if (strcmp(sel2, "15") == 0) file_explorer();
            else if (strcmp(sel2, "16") == 0) file_opener();
            else if (strcmp(sel2, "17") == 0) update_system();
            else if (strcmp(sel2, "18") == 0) activate();
            else cout << "Invalid option." << '\n';
        }
    }

    cout << "Shutting down..." << '\n';
    return 0;
}
