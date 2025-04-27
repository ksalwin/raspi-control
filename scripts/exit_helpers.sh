# Source usage.sh relative to where exit_helpers.sh is located.
source "$(dirname "${BASH_SOURCE[0]}")/usage.sh"

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
