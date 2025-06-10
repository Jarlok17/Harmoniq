#!/bin/sh

case "$(uname -s)" in
    Linux*) platform="linux" ;;
    Darwin*) platform="macos" ;;
    CYGWIN*|MINGW*|MSYS*) platform="windows" ;;
    *) echo "Unsupported platform"; exit 1 ;;
esac

echo "Setting up dependencies for Harmoniq on $platform..."

if [ "$platform" = "linux" ]; then
    if command -v apt >/dev/null 2>&1; then
        sudo apt update
        sudo apt install -y cmake ninja-build qt6-base-dev g++ || exit 1
    else
        echo "Please install cmake, ninja-build, qt6-base-dev, g++ using your package manager (e.g., yum, dnf, pacman)."
        exit 1
    fi
elif [ "$platform" = "macos" ]; then
    if ! command -v brew >/dev/null 2>&1; then
        echo "Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    brew install cmake ninja qt@6 || exit 1
elif [ "$platform" = "windows" ]; then
    if [ -d "/mingw64" ]; then
        echo "Detected MSYS2. Installing dependencies..."
        pacman -Syu --noconfirm
        pacman -S --noconfirm mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-qt6-base mingw-w64-x86_64-gcc || exit 1
        export QT_PATH=/mingw64
    else
        echo "MSYS2 not detected. Please install dependencies manually:"
        echo "1. CMake: https://cmake.org/download/ (add to PATH)"
        echo "2. Qt6: https://www.qt.io/download (select mingw_64 or msvc2022_64)"
        echo "3. Visual Studio 2022 (MSVC) or MinGW (via Qt Installer)"
        echo "4. Set QT_PATH, e.g., export QT_PATH=/c/Qt/6.9.1/mingw_64"
        exit 1
    fi
fi

echo "Dependencies installed successfully. Run './scripts/desktop.sh -c -b -r' to build and run Harmoniq."
