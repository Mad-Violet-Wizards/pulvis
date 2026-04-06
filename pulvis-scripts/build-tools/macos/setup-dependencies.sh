#!/bin/bash

# Script to set up macOS dependencies
# This script helps identify what needs to be installed for macOS

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/../../.."

echo "==================================="
echo "macOS Dependencies Setup"
echo "==================================="
echo ""

# Check for Homebrew
if ! command -v brew; then
    echo "Homebrew not found!"
    echo "Install from: https://brew.sh"
    echo ""
else
    echo "Homebrew installed"
fi

# Check for Xcode Command Line Tools
if ! command -v clang; then
    echo "Xcode Command Line Tools not found!"
    echo "Install with: xcode-select --install"
    echo ""
else
    echo "Xcode Command Line Tools installed"
    echo "Clang version: $(clang --version | head -n1)"
fi

# Check for Python
if ! command -v python3; then
    echo "Python 3 not found!"
    echo "Install with: brew install python3"
    echo ""
else
    echo "Python 3 installed"
    echo "Version: $(python3 --version)"
fi

# Check for premake5
cd "$PROJECT_ROOT"
if [ ! -f "premake5" ]; then
    echo "premake5 binary not found!"
    echo "Downloading premake5 for macOS..."
    
    PREMAKE_URL="https://github.com/premake/premake-core/releases/download/v5.0.0-alpha16/premake-5.0.0-alpha16-macosx.tar.gz"
    
    curl -L "$PREMAKE_URL" -o /tmp/premake5.tar.gz
    if [ $? -eq 0 ]; then
        # Extract premake5 (libluasocket.dylib is not needed on macOS - it's statically linked)
        tar -xzf /tmp/premake5.tar.gz -C "$PROJECT_ROOT" premake5 
        rm /tmp/premake5.tar.gz
        chmod +x "$PROJECT_ROOT/premake5"
        
        ARCH=$(file "$PROJECT_ROOT/premake5" | grep -o 'arm64\|x86_64\|universal')
        if [[ "$ARCH" == *"arm64"* ]] || [[ "$ARCH" == *"universal"* ]]; then
            echo "premake5 downloaded and installed (works on ARM64)"
        else
            echo "Downloaded premake5 is x86_64, but will work via Rosetta 2"
        fi
    else
        echo "Failed to download premake5!"
        echo "Please download manually from: https://premake.github.io/download"
    fi
    echo ""
else
    ARCH=$(file premake5 | grep -o 'arm64\|x86_64\|universal')
    if [[ "$ARCH" == *"arm64"* ]] || [[ "$ARCH" == *"universal"* ]]; then
        echo "premake5 installed (ARM64 compatible)"
    elif [ "$ARCH" = "x86_64" ]; then
        echo "premake5 is x86_64 (will work via Rosetta 2)"
    else
        echo "Cannot determine premake5 architecture"
    fi
fi

echo ""
echo "==================================="
echo "Dependencies for compilation:"
echo "==================================="
echo ""
echo "The project uses these libraries:"
echo "  - GLFW (for window and input)"
echo "  - GLM, fmt, nlohmann/json, etc. (header-only, already in pulvis-vendor/common/)"
echo ""
echo "Installing GLFW via Homebrew (recommended):"
if ! command -v brew; then
    echo "Install Homebrew first: https://brew.sh"
else
    if brew list glfw; then
        echo "GLFW already installed"
    else
        echo "Installing GLFW..."
        brew install glfw
        if [ $? -eq 0 ]; then
            echo "GLFW installed successfully"
        else
            echo "Failed to install GLFW"
        fi
    fi
fi