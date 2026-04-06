#!/bin/bash

# Script to build the project on macOS

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/../../.."

# Default configuration is Debug, but can be overridden
CONFIG="${1:-Debug}"

echo "==================================="
echo "Building project (Configuration: $CONFIG)..."
echo "==================================="

cd "$PROJECT_ROOT"

# Check if Makefile exists
if [ ! -f "Makefile" ]; then
    echo "Error: Makefile not found!"
    echo "Please run generate.sh first to generate build files."
    exit 1
fi

# Build the project
# -j$(sysctl -n hw.ncpu) uses all available CPU cores for parallel compilation
make config=$(echo "$CONFIG" | tr '[:upper:]' '[:lower:]') -j$(sysctl -n hw.ncpu)

if [ $? -eq 0 ]; then
    echo ""
    echo "==================================="
    echo "Build completed successfully!"
    echo "==================================="
    echo "Executable location: build/$CONFIG/pulvis-game"
    echo "Run './pulvis-scripts/build-tools/macos/run.sh' to execute the game"
else
    echo ""
    echo "==================================="
    echo "Error: Build failed!"
    echo "==================================="
    exit 1
fi
