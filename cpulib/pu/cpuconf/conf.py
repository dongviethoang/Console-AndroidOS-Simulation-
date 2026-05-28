import sys

def integer(i):
    print(f"Setting integer to {i}")

def setmax(i):
    return i

def os_d(value):
    if bool(value):
        return True
    else:
        return False
    
def setcores(p, e):
    with open("cpu_config.txt", "w") as f:
        f.write(f"Performance cores: {p}\n")
        f.write(f"Efficiency cores: {e}\n")
        
def exitf(code):
    print(f"Exiting with code {code}")
    sys.exit(code)