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
