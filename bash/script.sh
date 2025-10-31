#!/bin/bash

MAX_DEPTH=1
DIRECTORY_PATH="."


# This function just prints a help message.
show_help() {
    echo "How to use this script:"
    echo "  ./script.sh [options] [directory_name]"
    echo
    echo "Options:"
    echo "  -h          Show this help message"
    echo "  -r          Search in all subfolders"
    echo "  -d DEPTH    Search in subfolders up to a specific depth"
    echo
    echo "If no directory is specified, the current one is used."
}


# This function checks files in one specific directory.
analyze_directory() {
    local target_dir="$1"
    local current_depth="$2"

    if [ "$current_depth" -gt "$MAX_DEPTH" ]; then
        return
    fi

    for item in "$target_dir"/*; do
    	if [ -L "$item" ]; then
            echo "File '$(basename "$item")' is a soft link"

        elif [ -f "$item" ] && [ -x "$item" ]; then
            echo "File '$(basename "$item")' is executable"
  
        elif [ -d "$item" ]; then
            analyze_directory "$item" $((current_depth + 1))
        fi
    done
}


# This loop checks all the arguments the user provided.
while [ "$#" -gt 0 ]; do
    case "$1" in
        -h)
            show_help
            exit 0
            ;;
        -r)
            MAX_DEPTH=99
            shift
            ;;
        -d)
            if [ -z "$2" ] || ! [[ "$2" =~ ^[0-9]+$ ]]; then
                echo "Error: -d option requires a number after it."
                exit 1
            fi
            MAX_DEPTH="$2"
            shift 2
            ;;
        *)
            DIRECTORY_PATH="$1"
            shift
            ;;
    esac
done

if [ ! -d "$DIRECTORY_PATH" ]; then
    echo "Error: Cannot find directory '$DIRECTORY_PATH'"
    exit 1
fi
analyze_directory "$DIRECTORY_PATH" 1

echo "Done!"
