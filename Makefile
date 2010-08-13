
CXXFLAGS = -ggdb
OBJS = src/object_listener.os src/compact_writer.os src/object_filter.os src/validating_filter.os
TEST_OBJS = tests/test.o

all: src/libjsonxx.so tests/test_ok_timestamp

clean:
	rm -f src/libjsonxx.so $(OBJS) tests/test_ok_timestamp tests/test $(TEST_OBJS)



src/libjsonxx.so: $(OBJS)
	g++ -fPIC -shared -o $@ $(OBJS)

src/object_filter.cpp: src/object_listener.hpp src/object_filter.hpp
src/object_listener.cpp: src/object_listener.hpp
src/compact_writer.cpp: src/object_listener.hpp src/compact_writer.hpp
src/object_filter.hpp: src/object_listener.hpp
src/validating_filter.hpp: src/object_filter.hpp
src/validating_filter.cpp: src/validating_filter.hpp

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
