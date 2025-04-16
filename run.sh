#!/bin/bash

set -e # Exit on error

##### Defines #####
BUILD_DIR="build"
CLIENT_DIR="client"
SERVER_DIR="server"
EXECUTABLE_NAME="raspi-control"

# --- Functions ---
function print_error() {
	printf "\e[31m[ERROR]\e[0m %s\n" "$1"
}

function ensure_arg_provided() {
	# Check if 1st argument is `client` or `server`
	if [[ "$1" != "client" && "$1" != "server" ]]; then
		print_error "First argument is missing: <client|server>"
		echo
		print_usage
		exit 1
	fi

	# Check if 2nd argument was provided
	if [ "$#" -ne 2 ]; then
        print_error "Second argument is missing: <command>"
		echo
		print_usage
		exit 1
	fi
}

function ensure_tool_installed() {
    local tool="$1"
    if ! command -v "$tool" &>/dev/null; then
        print_error "$tool is not installed"
		echo
		echo "Install $tool and try again."
        exit 1
    fi
}

function print_usage() {
	echo "[INFO] Usage:"
	echo "sh $0 <client|server> <command>"
	echo
	echo "Possible commands:"
	echo "\tbuild - build client"
	echo "\tclean - remove build artifacts"
	echo "\trun   - build and run client app"
	echo "\ttest  - test client"
}

function make_build_dir() {
  [ -d "$BUILD_DIR" ] || mkdir "$BUILD_DIR"
}

function execute_command() {
	local mode="$1"
	local command="$2"
	local project_dir="$3"

	case "$command" in
		build)
			cd "$project_dir"
			build
			echo
			echo "Build completed."
			echo "Run executable with: ./$PROJECT_DIR/$BUILD_DIR/$EXECUTABLE_NAME"
			;;
		
		doc)
			ensure_tool_installed plantuml
			ensure_tool_installed doc
			;;

		clean)
			rm -rf "$project_dir"/"$BUILD_DIR"
			echo "Cleaned build directory."
			;;

		run)
			cd "$project_dir"
			build
			echo
			echo "Running executable..."
			./"$BUILD_DIR"/"$EXECUTABLE_NAME"
			;;

		test)
			cd "$project_dir"
			build -DBUILD_TESTS=ON
			ctest --test-dir "$BUILD_DIR"
			;;

		*)
			print_error "Unknown argument: $1"
			echo
			print_usage
			exit 1
			;;
	esac
}

function build() {
	make_build_dir
	echo "[Info] Configuring with: cmake -B $BUILD_DIR/ "$@""
	echo
	cmake -B "$BUILD_DIR" "$@"
	cmake --build "$BUILD_DIR"
}

##### Main Execution #####

# Ensure arguments provided
ensure_arg_provided "$@"

# Ensure tools installed
ensure_tool_installed cmake

if [ "$1" == "server" ]; then
	ensure_tool_installed arm-linux-gnueabihf-gcc
	ensure_tool_installed arm-linux-gnueabihf-g++
fi

# Set project dir
if [ "$1" == "server" ]; then
	PROJECT_DIR=$SERVER_DIR
	exit 0
else
	PROJECT_DIR=$CLIENT_DIR
fi

# Execute command
execute_command "$1" "$2" "$PROJECT_DIR"
