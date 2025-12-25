import os
import psutil

def get_parent_shell_name():
    """
    Detects the parent shell process name (cmd, powershell, pwsh, etc.) on Windows.
    Returns None if the parent process cannot be determined or is not one of the shells.
    """
    if os.name != 'nt':
        # This detection is specifically for Windows shells (cmd, powershell)
        return None

    try:
        # 1. Get the current process ID
        current_pid = os.getpid()
        
        # 2. Get the current process object
        current_process = psutil.Process(current_pid)
        
        # 3. Get the parent process ID (PPID)
        parent_pid = current_process.ppid()
        
        # 4. Get the parent process object
        parent_process = psutil.Process(parent_pid)
        
        # 5. Get the parent process name (e.g., 'cmd.exe', 'powershell.exe', 'pwsh.exe')
        parent_name = parent_process.name().lower()

        # Check against common Windows shell names
        if parent_name == 'cmd.exe':
            return 1
        elif parent_name in ('powershell.exe', 'pwsh.exe'):
            return 1
        
        return parent_name

    except psutil.NoSuchProcess:
        return 4
    except Exception as e:
        # Handle other potential errors like AccessDenied
        print(f"An error occurred: {e}")
        return 3