import os
import time
import shutil

# Configuration
monitor_dir = "/Users/limou/Downloads"  # Directory where decompressed folders appear
target_filename = "fig1_10.c"  # The file to check inside new folders
replacement_file = "/Users/limou/Documents/fig1_10.c"  # The replacement file

def touch_r(source, target):
    """Set the modification time of 'target' to match 'source' (touch -r equivalent)."""
    if os.path.exists(source) and os.path.exists(target):
        times = os.stat(source).st_atime, os.stat(source).st_mtime
        os.utime(target, times)
        print(f"Updated timestamp of {target} to match {source}")

def monitor_folder():
    print(f"Monitoring {monitor_dir} for new folders...")
    known_folders = set(os.listdir(monitor_dir))

    while True:
        time.sleep(5)  # Check every 5 seconds
        current_folders = set(os.listdir(monitor_dir))
        new_folders = current_folders - known_folders  # Detect new folders

        for folder in new_folders:
            folder_path = os.path.join(monitor_dir, folder)
            if os.path.isdir(folder_path):  # Ensure it's a folder
                target_path = os.path.join(folder_path, target_filename)
                
                if os.path.exists(target_path):
                    print(f"Found {target_filename} in {folder_path}, replacing it...")
                    shutil.copy(replacement_file, target_path)
                    touch_r(replacement_file, target_path)  # Apply timestamp sync
                    print(f"Replaced {target_filename} in {folder_path} and synced timestamps.")

        known_folders = current_folders  # Update known folders

if __name__ == "__main__":
    monitor_folder()
