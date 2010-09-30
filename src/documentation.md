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

### Generating simple JSON output ###

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
an instance of another class derived from `object_listener`:

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


### Arrays ###

    :::c++
    #include <iostream>
    #include <jsonxx/indenting_writer.hpp>

    void write_object(jsonxx::object_listener& obj) {
        jsonxx::scoped_object o(obj);

        obj.key("arr");
        obj.start_array(); // Or use jsonxx::scoped_array

        for (int i=0; i<5; ++i) obj.value(i);

        obj.end_array();
    }

    int main() {
        jsonxx::indenting_writer iw(std::cout);

        write_object(iw);

        return 0;
    }

This program outputs:

    :::js
    {
        "arr": [
            0,
            1,
            2,
            3,
            4
        ]
    }


### Nested objects ###

    :::c++
    #include <iostream>
    #include <jsonxx/indenting_writer.hpp>

    void write_object(jsonxx::object_listener& obj) {
        jsonxx::scoped_object root(obj);
        obj.key("o");

        {
            jsonxx::scoped_object o(obj);
            obj.key("p");

            {
                jsonxx::scoped_object p(obj);

                // object_listener::pair(key, value) is the same as
                // object_listener::key(key); object_listener::value(value);
                obj.key("q");
                obj.value("r");
            }
        }
    }

    int main() {
        jsonxx::indenting_writer iw(std::cout);

        write_object(iw);

        return 0;
    }

This program outputs:

    :::js
    {
        "o": {
            "p": {
                "q": "r"
            }
        }
    }


### Using `validating_filter` ###

By the semantics of JSON, it does not make sense to call `key` while generating
the contents of an array. While generating the contents of an object, `value`
may not be called several times in a row -- `key` is required.

Classes derived from `object_listener` are not in general expected to handle
invalid input like this in a well-defined manner, so if you want to detect and
debug such usage, you want to use the `validating_filter`:

    :::c++
    #include <iostream>
    #include <jsonxx/indenting_writer.hpp>
    #include <jsonxx/validating_filter.hpp>

    void write_object(jsonxx::object_listener& obj);

    int main() {
        jsonxx::indenting_writer iw(std::cout);
        jsonxx::validating_filter vf(&iw);

        try {
            write_object(vf);
        }
        catch (const jsonxx::validation_error&) {
            std::cerr << "Whoops!" << std::endl;
            return 1;
        }

        return 0;
    }

*/



/*doc*

Reference
---------

*/

//doc-include: object_listener.hpp
