#!/bin/sh

usage() {
    echo "./desktop.sh [command]"
    echo "    -c        configure"
    echo "    -b        build"
    echo "    -i        install"
    echo "    -r        run"
    echo "    -h        show this help"
    exit 1
}

if [ $# -eq 0 ]; then
    echo "use -h for help"
    exit 1
fi

configure=0
build=0
install=0
run=0

while getopts "cbirh" opt; do
    case "$opt" in
        c) configure=1 ;;
        b) build=1 ;;
        i) install=1 ;;
        r) run=1 ;;
        h) usage ;;
        ?) usage ;;
    esac
done

case "$(uname -s)" in
    Linux*) platform="linux" ;;
    Darwin*) platform="macos" ;;
    CYGWIN*|MINGW*|MSYS*) platform="windows" ;;
    *) echo "Unsupported platform"; exit 1 ;;
esac

find_qt6() {
    local qt_path=""
    case "$platform" in
        linux)
            for dir in /usr/lib/qt6 /usr/local/qt6 /opt/qt6; do
                if [ -d "$dir/lib/cmake/Qt6" ]; then
                    qt_path="$dir"
                    break
                fi
            done
            ;;
        macos)
            for dir in /opt/homebrew/opt/qt6 /usr/local/opt/qt6; do
                if [ -d "$dir/lib/cmake/Qt6" ]; then
                    qt_path="$dir"
                    break
                fi
            done
            ;;
        windows)
            for dir in "C:/Qt/6.9.1" "C:/Program Files/Qt/6.9.1"; do
                for version in "$dir"/mingw_64 "$dir"/msvc2019_64; do
                    if [ -d "$version/lib/cmake/Qt6" ]; then
                        qt_path="$version"
                        break 2
                    fi
                done
            done
            ;;
    esac

    if [ -z "$qt_path" ]; then
        echo "Qt6 not found automatically. Please install Qt6 or set QT_PATH environment variable."
        exit 1
    fi
    echo "$qt_path"
}

QT_PATH=$(find_qt6)
if [ -z "$QT_PATH" ] && [ -n "$QT_PATH" ]; then
    QT_PATH="$QT_PATH"
fi

if [ "$platform" = "windows" ]; then
    GENERATOR="MinGW Makefiles"
    BUILD_DIR="build/windows"
    BIN_DIR="bin"
    CMAKE_FLAGS=""
elif [ "$platform" = "macos" ]; then
    GENERATOR="Ninja"
    BUILD_DIR="build/macos"
    BIN_DIR="bin"
elif [ "$platform" = "linux" ]; then
    GENERATOR="Ninja"
    BUILD_DIR="build/linux"
    BIN_DIR="bin"
else
    echo "Unknown platform for build configuration"
    exit 1
fi

mkdir -p "$BUILD_DIR"

if [ "$configure" -eq 1 ]; then
    cmake -B "$BUILD_DIR" -G "$GENERATOR" \
          -DCMAKE_PREFIX_PATH="$QT_PATH" \
          -DCMAKE_BUILD_TYPE=Release \
          $CMAKE_FLAGS
    if [ $? -ne 0 ]; then
        echo "Configuration failed"
        exit 1
    fi
    cp "$BUILD_DIR/compile_commands.json" build/ 2>/dev/null || echo "No compile_commands.json to copy"
fi

if [ "$build" -eq 1 ]; then
    if [ "$platform" = "windows" ]; then
        cmake --build "$BUILD_DIR" --config Release -j16
        if [ $? -ne 0 ]; then
            echo "Build failed"
            exit 1
        fi
        /c/Qt/6.9.1/mingw_64/bin/windeployqt --qmldir src/qml "$BUILD_DIR/$BIN_DIR/Harmoniq.exe"
        if [ $? -ne 0 ]; then
            echo "windeployqt failed"
            exit 1
        fi
    else
        cmake --build "$BUILD_DIR" -j16
        if [ $? -ne 0 ]; then
            echo "Build failed"
            exit 1
        fi
    fi
fi

if [ "$run" -eq 1 ]; then
    if [ "$platform" = "windows" ]; then
        if [ -f "$BUILD_DIR/$BIN_DIR/Harmoniq.exe" ]; then
            "$BUILD_DIR/$BIN_DIR/Harmoniq.exe"
        else
            echo "Harmoniq.exe not found in $BUILD_DIR/$BIN_DIR"
            exit 1
        fi
    else
        "$BUILD_DIR/$BIN_DIR/Harmoniq"
    fi
    if [ $? -ne 0 ]; then
        echo "Run failed"
        exit 1
    fi
fi
