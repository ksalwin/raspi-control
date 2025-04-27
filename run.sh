#!/bin/bash

set -e # Exit on error

source "$(dirname "$0")/scripts/exit_helpers.sh"
source "$(dirname "$0")/scripts/print_utils.sh"
source "$(dirname "$0")/scripts/tool_helpers.sh"
source "$(dirname "$0")/scripts/usage.sh"

##### Defines #####
EXECUTABLE_NAME="raspi-control"

# --- Functions ---
function make_build_dir() {
	local build_dir="$1"
	mkdir -p "$build_dir"
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
