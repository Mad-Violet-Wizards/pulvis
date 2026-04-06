#!/bin/bash

# Script to clean generated build files on macOS

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/../../.."

echo "==================================="
echo "Cleaning generated build files..."
echo "==================================="

cd "$PROJECT_ROOT"

# Remove build directory
if [ -d "build" ]; then
    echo "Removing build/ directory..."
    rm -rf build/
fi

# Remove generated Makefiles
if [ -f "Makefile" ]; then
    echo "Removing Makefile..."
    rm -f Makefile
fi

# Remove all .make files
if ls *.make 1> /dev/null 2>&1; then
    echo "Removing *.make files..."
    rm -f *.make
fi

# Remove obj directory if it exists
if [ -d "obj" ]; then
    echo "Removing obj/ directory..."
    rm -rf obj/
fi

# Remove bin directory if it exists
if [ -d "bin" ]; then
    echo "Removing bin/ directory..."
    rm -rf bin/
fi

echo ""
echo "==================================="
echo "Cleanup completed successfully!"
echo "==================================="
echo "Run './pulvis-scripts/build-tools/macos/generate.sh' to regenerate build files"
