#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "No file specified." << endl;
        return 1;
    }

    string file = argv[1];
    string command;

    // Open the file
    command = "open \"" + file + "\" type mpegvideo alias music";
    mciSendStringA(command.c_str(), NULL, 0, 0);

    // Play it
    mciSendStringA("play music", NULL, 0, 0);

    cout << "Playing: " << file << endl;
    cout << "Press Enter to exit..." << endl;
    cin.get();

    // Close the device
    mciSendStringA("close music", NULL, 0, 0);

    return 0;
}
