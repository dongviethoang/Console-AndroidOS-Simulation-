import os
import keyboard
import time

os.system("title BIOS")
os.system("clear")

f1_pressed = False  # global flag

def on_f1():
    global f1_pressed
    f1_pressed = True

osfile = "buildltsc.exe"

keyboard.on_press_key("f1", lambda e: on_f1())

def boot_options():
    global osfile
    print("Boot with UEFI firmware [N]")
    print("Boot with BIOS firmware [Y]")

    a = input(">> ")
    if a == "UEFI: Y":
        os.system("echo Booting with UEFI...")
        os.system("uefi.exe")
    elif a.upper() == "E":
        os.system(osfile)
    else:
        print("Invalid option, defaulting to BIOS...")
        os.system(osfile)

def boot():
    global f1_pressed
    global osfile
    print("Console AndroidOS BIOS")
    print("Booting......")
    print("Press F1 to enter boot options")

    # 3-second POST check
    for _ in range(30):  # 30 * 0.1s = 3 sec
        if f1_pressed:
            boot_options()
            return
        time.sleep(0.1)

    # Default boot
    if not "398" in osfile or "2" in osfile:
        os.system(osfile)
    else:
        print("Error: Build 398 does not support BIOS booting.")

boot()
