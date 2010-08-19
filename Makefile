all:
	./waf && LD_LIBRARY_PATH=build/debug/src ./build/debug/tests/test
