function ensure_tool_installed() {
    local tool="$1"
    if ! command -v "$tool" &>/dev/null; then
		exit_with_tool_error "$tool"
    fi
}
