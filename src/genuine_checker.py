import os
import sys

os.system("title check")

def open_file():
    if not os.path.exists("genuine.txt"):
        print("genuine.txt not found. Returning code 0x13...")
        sys.exit(1)
    else:
        with open("genuine.txt", 'r') as file:
            for line in file:
                if "GENUINE=TRUE" in line:
                    return True
                else:
                    print("This copy is not genuine. Returning code 0x12...")
                    sys.exit(1) 

open_file()