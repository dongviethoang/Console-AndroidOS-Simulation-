import os
import time

os.system("title Setup Console")
os.system("clear")

print("Welcome to the Setup Wizard of Console AndroidOS!")
print("1. Boot from D: drive (recommended)")
print("2. Boot from C: drive (CD-ROM)")
a = input("Choose an option: ")

option1 = "1"
option2 = "2"

directory = r"D:\\Console AndroidOS\\bin"

if a == option1:
    print("Do you want to format the D: drive before proceeding? (y/n): ")
    format_choice = input().lower()
    if format_choice == 'y':
        print("Formatting D: drive...")
        time.sleep(2)
        os.system("format")
        
    print("Launching UEFI from D: drive...")
    time.sleep(1)
    os.chdir(directory)
    os.system("launch.exe")
elif a == option2:
    print("Launching UEFI from C: drive...")
    time.sleep(1)
    os.chdir(directory)
    os.system("launch.exe")