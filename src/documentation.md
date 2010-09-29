/*doc*

JSONxx
======

[TOC]


Design
------

There are lots of [JSON libraries out there](http://json.org/). This library was
written because the existing C++-libraries at the time only offered a DOM-like
interface, where JSON documents are represented entirely in memory. In many
situations, a streaming model is a better fit.

The design of JSONxx has been guided by the following principles, in order from
the most important:

 1. Prefer to stream the data rather than buffer it.
 2. Prefer ease of use to correctness. (For example when reading large
    integers, JSONxx will silently overflow if the integer will not fit in an
    `int`)
 3. Prefer to minimize external dependencies, platform specific code and
    compiler specific code.


Tutorial
--------

### Generating JSON output ###

The following program will output `{"answer":42}` on `std::cout`:

    :::c++
    #include <iostream>
    #include <jsonxx/compact_writer.hpp>

    int main() {
        jsonxx::compact_writer cw(std::cout);

        // A JSON document has an object as the root:
        cw.start_object();

        // Output a key/value pair:
        cw.pair("answer", 42);

        // When we are done writing the object:
        cw.end_object();

        return 0;
    }

It is tedious to keep track of scope manually, so JSONxx offers some helpful
scope keeping classes. Let's refactor the example into this equivalent program:

    :::c++
    #include <iostream>
    #include <jsonxx/compact_writer.hpp>

    void write_object(jsonxx::object_listener& obj) {
        jsonxx::scoped_object o(obj);

        obj.pair("answer", 42);
    }

    int main() {
        jsonxx::compact_writer cw(std::cout);

        write_object(cw);

        return 0;
    }

Notice that the `write_object` function takes a
`jsonxx::object_listener`-reference. Let's replace the `compact_writer` with
an instance of another subclass of `object_listener`:

    :::c++
    #include <iostream>
    #include <jsonxx/indenting_writer.hpp>

    void write_object(jsonxx::object_listener& obj) {
        jsonxx::scoped_object o(obj);

        obj.pair("answer", 42);
    }

    int main() {
        jsonxx::indenting_writer iw(std::cout);

        write_object(iw);

        return 0;
    }

The output has now turned into:

    :::js
    {
        "answer": 42
    }

*/



/*doc*

Reference
---------

*/

//doc-include: object_listener.hpp
