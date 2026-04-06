#!/bin/bash

# Script to run the compiled executable on macOS

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/../../.."

# Default configuration is Debug, but can be overridden
CONFIG="${1:-Debug}"

echo "==================================="
echo "Running pulvis-game (Configuration: $CONFIG)..."
echo "==================================="

cd "$PROJECT_ROOT"

EXECUTABLE="build/$CONFIG/pulvis-game"

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable not found at $EXECUTABLE"
    echo "Please build the project first using build.sh"
    exit 1
fi

# Make sure the executable has execute permissions
chmod +x "$EXECUTABLE"

# Run the executable
./"$EXECUTABLE"

