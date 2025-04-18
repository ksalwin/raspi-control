#!/bin/bash

set -e # Exit on error

##### Defines #####
EXECUTABLE_NAME="raspi-control"

# --- Functions ---
function print_error() {
	printf "\e[31m[ERROR]\e[0m %s\n" "$1"
}

function ensure_arg_provided() {
	# Check if 1st argument is `client` or `server`
	if [[ "$1" != "client" && "$1" != "server" ]]; then
		print_error "Invalid first argument: '$1'. Expected: <client|server>"
		echo
		print_usage
		exit 1
	fi

	# Check if 2nd argument was provided
	if [ "$#" -ne 2 ]; then
		print_error "Missing second argument <command> for target '$1'"
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

function make_build_dir() {
	local build_dir="$1"
	[ -d "$build_dir" ] || mkdir "$build_dir"
}

function execute_command() {
	local project_dir="$1"
	local command="$2"
	local build_dir="$project_dir/build"

	case "$command" in
		build)
			build "$project_dir"
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
			build "$project_dir"
			echo
			echo "Running executable..."
			./"$build_dir"/"$EXECUTABLE_NAME"
			;;

		test)
			build "$project_dir" -DBUILD_TESTS=ON
			ctest --test-dir "$build_dir"
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
	local project_dir="$1"
	local build_dir="$project_dir/build"

	make_build_dir "$build_dir"
	echo "[Info] Configuring with: cmake -B $build_dir -S $project_dir $@"
	echo
	cmake -B "$build_dir" -S "$project_dir" "$@"
	cmake --build "$build_dir" -- -j$(nproc)
}

##### Main Execution #####

# Ensure arguments provided
ensure_arg_provided "$@"
TARGET="$1"
COMMAND="$2"

# Ensure tools installed
ensure_tool_installed cmake

if [ "$TARGET" == "server" ]; then
	ensure_tool_installed arm-linux-gnueabihf-gcc
	ensure_tool_installed arm-linux-gnueabihf-g++
fi

# Execute command
execute_command "$TARGET" "$COMMAND"
