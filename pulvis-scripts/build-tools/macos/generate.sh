#!/bin/bash

# Script to generate build files for macOS (ARM M2)
# Uses gmake2 as the action (gmake2 is more compatible with macOS than ninja for premake5)

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/../../.."

echo "==================================="
echo "Generating build files for macOS..."
echo "==================================="

cd "$PROJECT_ROOT"

# Check if premake5 binary exists
if [ ! -f "premake5" ]; then
    echo "Error: premake5 binary not found!"
    echo "Please run setup-dependencies.sh first"
    exit 1
fi

# Make sure premake5 is executable
chmod +x premake5

# Generate using gmake2 (GNU Make)
./premake5 gmake2

if [ $? -eq 0 ]; then
    echo ""
    echo "==================================="
    echo "Build files generated successfully!"
    echo "==================================="
    echo "Generated files are in: Makefile and *.make files"
    echo "Run './pulvis-scripts/build-tools/macos/build.sh' to compile the project"
else
    echo ""
    echo "==================================="
    echo "Error: Failed to generate build files!"
    echo "==================================="
    exit 1
fi
