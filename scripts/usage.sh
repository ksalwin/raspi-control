function print_usage() {
	echo "Usage:"
	echo "  sh $0 <target> <command> [args]"
	echo
	echo "Available targets:"
	echo "  client    - For desktop-side logic"
	echo "  server    - For Raspberry Pi or embedded side"
	echo
	echo "Example:"
	echo "  sh $0 client build"
	echo
}

function print_usage_client() {
	echo "[client] commands:"
	echo "  build    - Configure and build the project"
	echo "  clean    - Remove build artifacts"
	echo "  run      - Build and run the executable"
	echo "  test     - Build with tests and run them"
	echo "  doc      - Generate documentation (requires 'doc' and 'plantuml')"
	echo
}

function print_usage_server() {
	echo "[server] commands:"
	echo "  remote-build  <user> <ip>  - Sync and build project remotely"
	echo "  remote-run    <user> <ip>  - Run built executable on Raspberry Pi"
	echo
}
