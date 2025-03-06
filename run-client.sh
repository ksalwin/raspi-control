#!/bin/bash

set -e # Exit on error

BUILD_DIR="build"
CLIENT_DIR="client"

function print_usage() {
	echo "Usage: $0 [command]"
	echo
	echo "Possible commands:"
	echo "	build - build client"
	echo "	clean - remove build artifacts"
	echo "	run   - build and run client app"
	echo "	test  - test client"
}

# Check if input argument was provided
if [ $# -eq 0 ]; then
	print_usage
	exit 1
fi

function make_build_dir() {
  [ -d "$BUILD_DIR" ] || mkdir "$BUILD_DIR"
}

function build_client() {
	make_build_dir
	cmake -B "$BUILD_DIR" "$@"
	cmake --build "$BUILD_DIR"
}

case "$1" in
	build)
		cd "$CLIENT_DIR"
		build_client
		echo
		echo "Build completed."
		echo "Run executable with: ./$CLIENT_DIR/$BUILD_DIR/raspi-control"
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
		./"$BUILD_DIR"/raspi-control
		;;

	test)
		cd "$CLIENT_DIR"
		build_client -DTEST=ON
		ctest --test-dir "$BUILD_DIR"
		;;

	*)
		echo "[ERROR] Unknown argument: $1"
		echo
		print_usage
		exit 1
		;;
esac
