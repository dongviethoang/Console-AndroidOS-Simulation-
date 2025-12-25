import os
import keyboard
import time

os.system("title UEFI")
os.system("clear")

f1_pressed = False  # global flag
escape_pressed = False
delete_pressed = False

def on_f1():
    global f1_pressed
    f1_pressed = True

def on_escape():
    global escape_pressed
    escape_pressed = True

def on_delete():
    global delete_pressed
    delete_pressed = True

keyboard.on_press_key("f1", lambda e: on_f1())
keyboard.on_press_key("esc", lambda e: on_escape())
keyboard.on_press_key("delete", lambda e: on_delete())

def boot_options():
    print("Boot with UEFI firmware [Y]")
    print("Boot with BIOS firmware [N]")

    a = input(">> ")
    if a == "BIOS: Y":
        os.system("echo Booting with BIOS...")
        os.system("bios.exe")
    elif a.upper() == "E":
        os.system("secup2")
    else:
        print("Invalid option, defaulting to BIOS...")
        os.system("secup2")

def boot():
    global f1_pressed
    print("Console AndroidOS UEFI")
    print("Booting......")
    print("Press F1 to enter boot options")
    print("Press Del to enter UEFI settings")
    print("Press Esc to enter the boot manager")

    # 3-second POST check
    for _ in range(30):  # 30 * 0.1s = 3 sec
        if f1_pressed:
            boot_options()
            return
        elif delete_pressed:
            print("Entering UEFI settings...")
            time.sleep(1)
            print("Date: ", time.strftime("%Y-%m-%d"))
            print("UEFI Version: 2.1.0")
            print("Type E to exit")
            a = input(">>")
            if a.upper() == "E":
                break
            return
        elif escape_pressed:
            print("Entering boot manager...")
            time.sleep(1)
            os.system("bootmgr.exe")
            return
        time.sleep(0.1)

    # Default boot
    os.system("secup2")

boot()
