#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    cout << "Welcome to the Console AndroidOS Disk Formatter!" << endl;
    cout << "Please enter the path of the disk you want to format: ";
    string diskPath;
    getline(cin, diskPath);
    ofstream formatDisk(diskPath, ios::out | ios::trunc);
    if (!formatDisk) {
        cerr << "Error: Could not open disk at " << diskPath << " for formatting." << endl;
        return 1;
    }
    formatDisk << "space=2048MB" << endl;
    formatDisk << "setchar=D" << endl;
    cout << "Disk formatted successfully!" << endl;
    formatDisk.close();
    return 0;
}