
OBJS = src/object_listener.os src/compact_writer.os

all: src/libjson.so

clean:
	rm -f src/libjson.so $(OBJS)

src/libjson.so: $(OBJS)
	g++ -fPIC -shared -o $@ $(OBJS)

%.os: %.cpp
	g++ -fPIC -shared -c -o $@ $<


src/object_listener.cpp: src/object_listener.hpp
src/compact_writer.cpp: src/object_listener.hpp src/compact_writer.hpp
