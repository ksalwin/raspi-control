#!/bin/bash

set -e # Exit on error

# --- Defines ---

BUILD_DIR="build"
CLIENT_DIR="client"
EXECUTABLE_NAME="raspi-control"

# --- Functions ---
function ensure_arg_provided() {
	if [ "$1" -eq 0 ]; then
		print_usage
		exit 1
	fi
}

function ensure_tool_installed() {
    local tool="$1"
    if ! command -v "$tool" &>/dev/null; then
        printf "\e[31m[Error]\e[0m %s is not installed. " "$tool"
        printf "Install %s and try again.\n" "$tool"
        exit 1
    fi
}

function print_usage() {
	echo "Usage: $0 [command]"
	echo
	echo "Possible commands:"
	echo "	build - build client"
	echo "	clean - remove build artifacts"
	echo "	run   - build and run client app"
	echo "	test  - test client"
}

function make_build_dir() {
  [ -d "$BUILD_DIR" ] || mkdir "$BUILD_DIR"
}

function build_client() {
	make_build_dir
	echo "[Info] Configuring with: cmake -B $BUILD_DIR/ "$@""
	echo
	cmake -B "$BUILD_DIR" "$@"
	cmake --build "$BUILD_DIR"
}

# --- Main Execution ---

ensure_arg_provided "$#"
ensure_tool_installed cmake

case "$1" in
	build)
		cd "$CLIENT_DIR"
		build_client
		echo
		echo "Build completed."
		echo "Run executable with: ./$CLIENT_DIR/$BUILD_DIR/$EXECUTABLE_NAME"
		;;
	
	doc)
		ensure_tool_installed plantuml
		ensure_tool_installed doc
		;;

	clean)
		rm -rf "$CLIENT_DIR"/"$BUILD_DIR"
		echo "Cleaned build directory."
		;;

	run)
		cd "$CLIENT_DIR"
		build_client
		echo
		echo "Running executable..."
		./"$BUILD_DIR"/"$EXECUTABLE_NAME"
		;;

	test)
		cd "$CLIENT_DIR"
		build_client -DBUILD_TESTS=ON
		ctest --test-dir "$BUILD_DIR"
		;;

	*)
		echo "[ERROR] Unknown argument: $1"
		echo
		print_usage
		exit 1
		;;
esac
