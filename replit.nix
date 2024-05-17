{ pkgs }: {
	deps = [
   pkgs.doxygen
   pkgs.cmake
		pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}