#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <filesystem>

#ifdef _WIN32
#include <direct.h>
#define chdir _chdir
#else
#include <unistd.h>
#endif

using namespace std;

static inline string trim(const string &s) {
    size_t a = 0;
    while (a < s.size() && isspace(static_cast<unsigned char>(s[a]))) ++a;
    size_t b = s.size();
    while (b > a && isspace(static_cast<unsigned char>(s[b-1]))) --b;
    return s.substr(a, b - a);
}

static inline string to_lower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

int main() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    string input;
    cout << "Android OS DOS Module v6.22" << endl;
    cout << "Copyright (c) November 2025 Beta7Developer. All rights reserved." << endl;
    cout << "This is under development. Do not use it for production purposes." << endl;

    const vector<string> forbidden = { "209", "conand" };
    const vector<string> dangerous = {
        "format ", "rm -rf", "rm -r", "mkfs", "dd if=", "shutdown", "poweroff",
        "reboot", "del /f", "del /s", "rmdir /s", "rd /s", ":\\", "format.com"
    };
    
    // Remove the initial definition of current_path and pathString here

    while (true) {
        // --- ADDED: Update the path string inside the loop ---
        filesystem::path current_path = filesystem::current_path();
        // Use generic_string() to format the path with forward slashes globally
        string pathString = current_path.generic_string(); 
        
        cout << pathString << ">";
        if (!getline(cin, input)) break;
        string cmd = trim(input);
        if (cmd.empty()) continue;

        string lcmd = to_lower(cmd);

        // builtins
        if (lcmd == "exit" || lcmd == "quit") {
            break;
        }
        // ... (rest of builtins: help, ver, dos? remain unchanged)
        if (lcmd == "help") {
            cout << "Builtins: cd <dir>, exit, quit, help" << endl;
            continue;
        }

        if (lcmd == "ver") {
            cout << "Android OS DOS Module v6.22" << endl;
            cout << "Copyright (c) November 2025 Beta7Developer. All rights reserved." << endl;
            cout << "Available OSes: Console AndroidOS Build 398, LTSC, 420, Security Update 2" << endl;
            continue;
        }

        if (lcmd == "dos?") {
            cout << "Yes, DOS. Windows 95/98 vibes." << endl;
            continue;
        }


        // cd handling (do not call system for cd)
        if (lcmd.rfind("cd", 0) == 0) {
            // parse argument
            string arg;
            if (cmd.size() > 2) {
                arg = trim(cmd.substr(2));
            }
            if (arg.empty()) {
                // go to HOME if available
                const char* home = getenv("HOME");
    #ifdef _WIN32
                if (!home) home = getenv("USERPROFILE");
    #endif
                if (home) {
                    if (chdir(home) == 0) {
                        cout << "Changed directory to " << home << endl;
                    } else {
                        cout << "Failed to change directory to " << home << endl;
                    }
                } else {
                    cout << "HOME not set." << endl;
                }
            } else {
                if (chdir(arg.c_str()) == 0) {
                    cout << "Changed directory to " << arg << endl;
                    // --- Note: The pathString variable will be updated at the start of the next loop iteration ---
                } else {
                    cout << "Directory not found: " << arg << endl;
                }
            }
            continue;
        }
        // ... (rest of safety checks and system call remain unchanged)
        
        // simple checks: forbid running commands that reference OS build names
        bool blocked = false;

        // helper: match only whole tokens / word-bounded occurrences
        auto contains_word = [](const string &text, const string &word) -> bool {
            size_t pos = 0;
            while ((pos = text.find(word, pos)) != string::npos) {
                bool left_ok = (pos == 0) || !isalnum(static_cast<unsigned char>(text[pos - 1]));
                size_t end = pos + word.size();
                bool right_ok = (end == text.size()) || !isalnum(static_cast<unsigned char>(text[end]));
                if (left_ok && right_ok) return true;
                pos = end;
            }
            return false;
        };

        for (const auto &f : forbidden) {
            if (contains_word(cmd, f)) {
                cout << "Error: OS does not support DOS module." << endl;
                blocked = true;
                break;
            }
        }
        if (blocked) continue;

        // block obviously dangerous commands
        for (const auto &d : dangerous) {
            if (lcmd.find(d) != string::npos) {
                cout << "Command blocked for safety: " << d << endl;
                blocked = true;
                break;
            }
        }
        if (blocked) continue;

        // NEW: Prevent running arbitrary .exe files in DOS mode except "conand"
        size_t pos_exe = lcmd.find(".exe");
        if (pos_exe != string::npos) {
            // find the token containing the .exe (handle paths and quotes)
            // locate start of token
            size_t token_start = lcmd.rfind(' ', pos_exe);
            string exe_token;
            if (token_start == string::npos) {
                exe_token = lcmd.substr(0, pos_exe + 4);
            } else {
                exe_token = lcmd.substr(token_start + 1, pos_exe - token_start + 4);
            }
            exe_token = trim(exe_token);
            // strip surrounding quotes if any
            if (!exe_token.empty() && exe_token.front() == '"') {
                if (exe_token.back() == '"') exe_token = exe_token.substr(1, exe_token.size() - 2);
                else exe_token = exe_token.substr(1);
            }
            // keep only filename (strip path)
            size_t sep = exe_token.find_last_of("\\/");
            if (sep != string::npos) exe_token = exe_token.substr(sep + 1);
            // allow only conand / conand.exe
            if (exe_token != "build398.exe" && exe_token != "build398" && exe_token != "buildltsc.exe" &&
                exe_token != "buildltsc" && exe_token != "build420.exe" && exe_token != "build420" &&
                exe_token != "secup2.exe" && exe_token != "secup2") {
                cout << "Cannot run this program in DOS mode." << endl;
                continue;
            }
        }

        // execute via system
        int rc = system(cmd.c_str());
        if (rc == -1) {
            cout << "Failed to execute command." << endl;
        }
    }

    return 0;
}
