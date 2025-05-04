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
	shift # Remove "$1" from "$@", to not repeat "$1" in `cmake -B`
	local build_dir="$project_dir/build"

	make_build_dir "$build_dir"
	echo "[INFO] Configuring with: cmake -B $build_dir -S $project_dir $@"
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

function sync_raspi_sysroot() {
	local sysroot_dir="$1"
	local raspi_user="$2"
	local raspi_ip="$3"

	# Skip if sysroot exists and is non-empty
	if [ -d "$sysroot_dir" ] && [ "$(ls -A "$sysroot_dir")" ]; then
		print_info "Raspberry Pi sysroot already exists at $sysroot_dir"
		return
	fi

	print_info "Syncing Raspberry Pi sysroot into $sysroot_dir ..."
	mkdir -p "$sysroot_dir"

	rsync -avz --delete "$raspi_user@$raspi_ip":/lib/ "$sysroot_dir/lib/"
	rsync -avz --delete "$raspi_user@$raspi_ip":/usr/ "$sysroot_dir/usr/"
	rsync -avz --delete "$raspi_user@$raspi_ip":/opt/ "$sysroot_dir/opt/"
	rsync -avz --delete "$raspi_user@$raspi_ip":/etc/ "$sysroot_dir/etc/"

	print_info "Raspberry Pi sysroot synced to $sysroot_dir"
}


function execute_server_command() {
	local target="$1"
	local command="$2"
	local build_dir="$target/build"

	case "$command" in
		build)
			local raspi_user="$3"
			local raspi_ip="$4"
			sync_raspi_sysroot "$target/raspi-sysroot" "$raspi_user" "$raspi_ip"

			local toolchain_file="$(realpath "$target/cmake/raspi-toolchain.cmake")"

			build "$target" -DCMAKE_TOOLCHAIN_FILE="$toolchain_file"
			echo
			echo "Build completed."
			echo "Run executable with: ./$build_dir/$EXECUTABLE_NAME"
			;;
		build)
			local toolchain_file="$(realpath "$target/cmake/raspi-toolchain.cmake")"
			build "$target" -DCMAKE_TOOLCHAIN_FILE="$toolchain_file"
			echo
			echo "Build completed."
			echo "Run executable with: ./$build_dir/$EXECUTABLE_NAME"
			;;
		clean)
			rm -rf "$build_dir"
			echo "Cleaned build directory."
			;;
		deploy)
			local binary_path="$build_dir/$EXECUTABLE_NAME"
			local raspi_user="$3"
			local raspi_ip="$4"
			local raspi_dest_dir="/home/$raspi_user/$EXECUTABLE_NAME"

			if [ ! -f "$binary_path" ]; then
				print_error "Binary not found at $binary_path."
				echo "Run './run.sh server build <raspi_user> <raspi_ip>'."
				exit 1
			fi

			echo "[INFO] Deploying $binary_path to $raspi_user@$raspi_ip:$raspi_dest_dir"
			ssh "$raspi_user@$raspi_ip" "mkdir -p $raspi_dest_dir"
			scp "$binary_path" "$raspi_user@$raspi_ip:$raspi_dest_dir/"
			echo "[INFO] Deployed to Raspberry Pi."
			;;
		run)
			local toolchain_file="$(realpath "$target/cmake/raspi-toolchain.cmake")"
			build "$target" -DCMAKE_TOOLCHAIN_FILE="$toolchain_file"
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
