#!/bin/bash

# Run make to compile the project using the Makefile in the root directory

SCRIPT_DIR="$(dirname "$0")"

source "$SCRIPT_DIR/../.env"

echo
echo "Running make to compile the project..."
make -f "$PROJECT_DIR/Makefile"

# Check if make was successful
if [ $? -eq 0 ]; then
    echo "Make completed successfully."

    # Define the path to the specific executable to run
    EXECUTABLE="$PROJECT_DIR/bin/listFiles"

    # Check if the executable exists and is executable
    if [ -f "$EXECUTABLE" ] && [ -x "$EXECUTABLE" ]; then
        echo "Running executable: $EXECUTABLE"
        echo

        # Get optional parameters
        MAX_DEPTH=${1:--1}
        MAX_FILES=${2:--1}
        MAX_SIZE=${3:--1}

        # Execute the specific executable with parameters
        "$EXECUTABLE" "$MAX_DEPTH" "$MAX_FILES" "$MAX_SIZE"
    else
        echo "Error: Executable $EXECUTABLE not found or not executable."
        exit 1
    fi
else
    echo "Error: Make failed."
    exit 1
fi
