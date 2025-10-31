import os
import sys

def show_help():
    print("--Help menu--")
    print("\n Parameters:")
    print("(non)        - Analyze current directory")
    print(" -h          - Show help")
    print(" -r          - Recursive search")
    print(" -d (number) - Set the maximum search depth")
    print("\nExample: python3 script.py /home/user -r -d 2")
    sys.exit()

def analyze_directory(directory_path, is_recursive, max_depth, current_depth=0):

    if max_depth != -1 and current_depth > max_depth:
        return 
    try:
        all_items = os.listdir(directory_path)
    except FileNotFoundError:
        print("Error, directory is not found!")
        return
    except PermissionError:
        print("Error, permision denied!")
        return

    for item_name in all_items:

        full_path = os.path.join(directory_path, item_name)

        if os.path.islink(full_path):
            print('File "{}" is a soft link'.format(item_name))

        elif os.path.isfile(full_path) and os.access(full_path, os.X_OK):
            print('File "{}" is executable'.format(item_name))

        if is_recursive and os.path.isdir(full_path):
            analyze_directory(full_path, is_recursive, max_depth, current_depth + 1)

all_arguments = sys.argv[1:]

target_directory = "."
recursive_search = False
depth_limit = -1

if "-h" in all_arguments:
    show_help()

args_to_process = list(all_arguments)
final_directory_arg = None

if "-r" in args_to_process:
    recursive_search = True
    args_to_process.remove("-r")

if "-d" in args_to_process:
    try:
        d_index = args_to_process.index("-d")
        depth_value = int(args_to_process[d_index + 1])
        depth_limit = depth_value
        args_to_process.pop(d_index)
        args_to_process.pop(d_index)
    except (ValueError, IndexError, TypeError):
        print("Error, after flag -d must be a number!")
        sys.exit()

if len(args_to_process) == 1:
    target_directory = args_to_process[0]
elif len(args_to_process) > 1:
    print("\nError, too many arguments!\n")
    show_help()

analyze_directory(target_directory, recursive_search, depth_limit)
