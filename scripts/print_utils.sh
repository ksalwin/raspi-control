function print_error() {
	printf "\e[31m[ERROR]\e[0m %s\n" "$1"
}

function print_info() {
	echo "[INFO] $1"
}
