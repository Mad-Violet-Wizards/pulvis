#!/bin/bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR/../../.."

echo "==================================="
echo "Cleaning generated build files..."
echo "==================================="

cd "$PROJECT_ROOT"

echo "Removing build/ directories..."
find . -type d -name "build" -prune -exec rm -rf {} + 2>/dev/null || true

echo "Removing Makefile files..."
find . -type f -name "Makefile" -delete 2>/dev/null || true

echo "Removing *.make files..."
find . -type f -name "*.make" -delete 2>/dev/null || true

echo "Removing obj/ directories..."
find . -type d -name "obj" -prune -exec rm -rf {} + 2>/dev/null || true

echo ""
echo "==================================="
echo "Cleanup completed successfully!"
echo "==================================="
echo "Run './pulvis-scripts/build-tools/macos/generate.sh' to regenerate build files"
