#!/bin/bash

set -e # Exit on error

source "$(dirname "$0")/scripts/print_utils.sh"

##### Defines #####
EXECUTABLE_NAME="raspi-control"

# --- Functions ---
function print_usage() {
	echo "[INFO] Usage:"
	echo "  sh $0 <client|server> <command>"
	echo
	echo "Available commands:"
	echo "  build    - Configure and build the project"
	echo "  clean    - Remove build artifacts"
	echo "  run      - Build and run the executable"
	echo "  test     - Build with tests and run them"
	echo "  doc      - Generate documentation (requires 'doc' and 'plantuml')"
	echo
	echo "Example:"
	echo "  sh $0 client build"
}

function print_error() {
	printf "\e[31m[ERROR]\e[0m %s\n" "$1"
}

function exit_with_command_error {
	local target="$1"
	local command="$2"

	print_error "Unknown command: '$command' for target '$target'"
	echo
	print_usage
	exit 1
}

function exit_with_target_error {
	local target="$1"

	print_error "Invalid first argument: '$target'. Expected: <client|server>"
	echo
	print_usage
	exit 1
}

function exit_with_tool_error {
	local tool="$1"

	print_error "$tool is not installed"
	echo
	echo "Install $tool and try again."
	exit 1
}

function ensure_tool_installed() {
    local tool="$1"
    if ! command -v "$tool" &>/dev/null; then
		exit_with_tool_error "$tool"
    fi
}

function make_build_dir() {
	local build_dir="$1"
	[ -d "$build_dir" ] || mkdir "$build_dir"
}

function build() {
	local project_dir="$1"
	local build_dir="$project_dir/build"

	make_build_dir "$build_dir"
	echo "[Info] Configuring with: cmake -B $build_dir -S $project_dir $@"
	echo
	cmake -B "$build_dir" -S "$project_dir" "$@"
	cmake --build "$build_dir" -- -j$(nproc)
}

function execute_client_command() {
	local target="$1"
	local command="$2"
	local build_dir="$target/build"

	case "$command" in
		build)
			build "$target"
			echo
			echo "Build completed."
			echo "Run executable with: ./$build_dir/$EXECUTABLE_NAME"
			;;
		
		doc)
			ensure_tool_installed plantuml
			ensure_tool_installed doc
			;;

		clean)
			rm -rf "$build_dir"
			echo "Cleaned build directory."
			;;

		run)
			build "$target"
			echo
			echo "Running executable..."
			./"$build_dir"/"$EXECUTABLE_NAME"
			;;

		test)
			build "$target" -DBUILD_TESTS=ON
			ctest --test-dir "$build_dir"
			;;

		*)
			exit_with_command_error "$target" "$command"
			;;
	esac
}

function execute_server_command() {
	local target="$1"
	local command="$2"
	local build_dir="$target/build"

	case "$command" in
		build)
			build "$target"
			echo
			echo "Build completed."
			echo "Run executable with: ./$build_dir/$EXECUTABLE_NAME"
			;;

		clean)
			rm -rf "$build_dir"
			echo "Cleaned build directory."
			;;

		run)
			build "$target"
			echo
			echo "Running executable..."
			./"$build_dir"/"$EXECUTABLE_NAME"
			;;

		*)
			exit_with_command_error "$target" "$command"
	esac
}


##### Main Execution #####
TARGET="$1"
COMMAND="$2"

# Ensure common tools installed
ensure_tool_installed cmake

# Execute command for target
if   [ "$TARGET" == "client" ]; then
	execute_client_command "$TARGET" "$COMMAND"

elif [ "$TARGET" == "server" ]; then
	ensure_tool_installed arm-linux-gnu-gcc
	ensure_tool_installed arm-linux-gnu-g++

	execute_server_command "$TARGET" "$COMMAND"

else
	exit_with_target_error "$TARGET"
fi
