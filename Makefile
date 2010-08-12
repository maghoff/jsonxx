
CXXFLAGS = -ggdb
OBJS = src/object_listener.os src/compact_writer.os
TEST_OBJS = tests/test.o

all: src/libjsonxx.so tests/test_ok_timestamp

clean:
	rm -f src/libjsonxx.so $(OBJS) tests/test_ok_timestamp tests/test $(TEST_OBJS)



src/libjsonxx.so: $(OBJS)
	g++ -fPIC -shared -o $@ $(OBJS)

src/object_listener.cpp: src/object_listener.hpp
src/compact_writer.cpp: src/object_listener.hpp src/compact_writer.hpp

tests/test: src/libjsonxx.so $(TEST_OBJS)
	g++ -Lsrc -ljsonxx -o $@ $(TEST_OBJS)

tests/test.o: src/object_listener.hpp src/compact_writer.hpp
tests/test.o: CXXFLAGS += -Isrc

tests/test_ok_timestamp: tests/test
	LD_LIBRARY_PATH=src ./tests/test && touch tests/test_ok_timestamp


%.os: %.cpp
	g++ $(CXXFLAGS) -fPIC -shared -c -o $@ $<

%.o: %.cpp
	g++ $(CXXFLAGS) -c -o $@ $<
