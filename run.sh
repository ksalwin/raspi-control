#!/bin/bash

set -e # Exit on error

source "$(dirname "$0")/scripts/exit_helpers.sh"
source "$(dirname "$0")/scripts/print_utils.sh"
source "$(dirname "$0")/scripts/tool_helpers.sh"
source "$(dirname "$0")/scripts/usage.sh"

##### Defines #####
EXECUTABLE_NAME="raspi-control"

# --- Functions ---
function ensure_arg_provided() {
	local arg_value="$1"
	local arg_name="$2"

	if [ -z "$arg_value" ]; then
		exit_with_arg_error "$arg_name"
	fi
}

function build() {
	local project_dir="$1"
	shift # Remove "$1" from "$@", to not repeat "$1" in `cmake -B`
	local build_dir="$project_dir/build"

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

	local timestamp_file="$sysroot_dir/.last_sync"
	local max_age_days=7

	# Check if recent enough
	if [ -f "$timestamp_file" ]; then
		local now=$(date +%s)
		local last_sync=$(date -r "$timestamp_file" +%s)
		local age_days=$(( (now - last_sync) / 86400 ))

		if [ "$age_days" -lt "$max_age_days" ]; then
			print_info "Sysroot is recent ($age_days days old) — skipping sync."
			return
		else
			print_info "Sysroot is $age_days days old — resyncing..."
		fi
	else
		print_info "No previous sync timestamp — syncing now..."
	fi

	print_info "Syncing Raspberry Pi sysroot into $sysroot_dir ..."

	mkdir -p "$sysroot_dir/usr"
	rsync -avz --delete "$raspi_user@$raspi_ip":/usr/include/ "$sysroot_dir/usr/include/"

	mkdir -p "$sysroot_dir/usr/lib"
	rsync -avz --delete "$raspi_user@$raspi_ip":/usr/lib/ "$sysroot_dir/usr/lib/"

	mkdir -p "$sysroot_dir/usr/libexec"
	rsync -avz --delete "$raspi_user@$raspi_ip":/usr/libexec/ "$sysroot_dir/usr/libexec/"

	mkdir -p "$sysroot_dir/lib"
	rsync -avz --delete "$raspi_user@$raspi_ip":/lib/ "$sysroot_dir/lib/"

	print_info "Raspberry Pi sysroot synced to $sysroot_dir"
}


function execute_server_command() {
	local target="$1"
	local command="$2"
	local build_dir="$target/build"

	local raspi_user="$3"
	local raspi_ip="$4"
	local raspi_dest_dir="/home/$raspi_user/$EXECUTABLE_NAME"

	case "$command" in
#		build)
#			ensure_tool_installed arm-linux-gnu-gcc
#			ensure_tool_installed arm-linux-gnu-g++
#			sync_raspi_sysroot "$target/raspi-sysroot" "$raspi_user" "$raspi_ip"
#			local toolchain_file="$(realpath "$target/cmake/raspi-toolchain.cmake")"
#
#			build "$target" -DCMAKE_TOOLCHAIN_FILE="$toolchain_file"
#			echo
#			echo "Build completed."
#			echo "Run executable with: ./$build_dir/$EXECUTABLE_NAME"
#			;;
#		clean)
#			rm -rf "$build_dir"
#			echo "Cleaned build directory."
#			;;
#		deploy)
#			local binary_path="$build_dir/$EXECUTABLE_NAME"
#
#			if [ ! -f "$binary_path" ]; then
#				print_error "Binary not found at $binary_path."
#				echo "Run './run.sh server build <raspi_user> <raspi_ip>'."
#				exit 1
#			fi
#
#			echo "[INFO] Deploying $binary_path to $raspi_user@$raspi_ip:$raspi_dest_dir"
#			ssh "$raspi_user@$raspi_ip" "mkdir -p $raspi_dest_dir"
#			scp "$binary_path" "$raspi_user@$raspi_ip:$raspi_dest_dir/"
#			echo "[INFO] Deployed to Raspberry Pi."
#			;;
		remote-build)
			print_info "Ensuring remote destination directory exists..."
			ssh "$raspi_user@$raspi_ip" "mkdir -p $raspi_dest_dir"

			print_info "Syncing server/ to $raspi_user@$raspi_ip:$raspi_dest_dir/server/"
			rsync -az --delete ./server/ "$raspi_user@$raspi_ip:$raspi_dest_dir/server/"
			rsync -az ./dependencies/  "$raspi_user@$raspi_ip:$raspi_dest_dir/dependencies/"

			print_info "Starting remote build on Raspberry Pi..."
			ssh "$raspi_user@$raspi_ip" bash -c "'
				set -e
				cd $raspi_dest_dir/server
				mkdir -p build
				cd build
				cmake ..
				make -j\$(nproc)
			'"

			print_info "Remote build completed."
			echo "You can run the server with:"
			echo "  ssh $raspi_user@$raspi_ip $raspi_dest_dir/server/build/raspi-control"
			;;
		remote-run)
			print_info "Running executable remotely on Raspberry Pi..."
			ssh "$raspi_user@$raspi_ip" "$raspi_dest_dir/server/build/$EXECUTABLE_NAME"
			;;
		*)
			exit_with_command_error "$target" "$command"
	esac
}


##### Main Execution #####
ensure_arg_provided "$1" "target" 
TARGET="$1"

ensure_arg_provided "$2" "command"
COMMAND="$2"

# Ensure common tools installed
ensure_tool_installed cmake

# Execute command for target
if   [ "$TARGET" == "client" ]; then
	execute_client_command "$TARGET" "$COMMAND"
elif [ "$TARGET" == "server" ]; then
	ensure_arg_provided "$3" "raspi_user"
	ensure_arg_provided "$4" "raspi_ip"
	RASPI_USER="$3"
	RASPI_IP="$4"
	execute_server_command "$TARGET" "$COMMAND" "$RASPI_USER" "$RASPI_IP"
else
	exit_with_target_error "$TARGET"
fi
