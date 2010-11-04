all:
	./waf && LD_LIBRARY_PATH=build/debug/src ./build/debug/tests/test

documentation.html:
	./doc/doc.py > documentation.html
