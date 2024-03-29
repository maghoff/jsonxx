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
 2. Aim for ease of use. (Possibly at the expense of ease of implementation)
 3. Prefer to minimize external dependencies, platform specific code and
    compiler specific code.

[[[[JSONXX:STREEM PROSESING]]]]


Status
------

This document describes version 1 of the library.

JSONxx was written by me, Magnus Hoff, to solve my JSON generating problems
once and for all. This means that even though JSON parsing has been tacked on,
the library is not really designed with parsing in mind. I hope to address
this in a future version.

Several other libraries use the name JSONxx (with varying capitalization). I
should find a new and better name.

This library is open sourced under [the MIT license][MIT] by
[Vizrt](http://www.vizrt.com/).

The [source][source] is currently hosted at bitbucket.

[MIT]: http://www.opensource.org/licenses/mit-license.php
[source]: http://bitbucket.org/maghoff/jsonxx


Installation
------------

 1. Get hold of the source code: `hg clone http://bitbucket.org/maghoff/jsonxx && cd jsonxx`
 2. Build: `./waf configure && ./waf build && ./waf check`
 3. Build documentation: `./doc/doc.py > documentation.html`
 4. For UNIX-like systems: `./unix/install.sh`


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
the contents of an array. Conversely, when you are generating the contents of an
object, you should not call `value` several times in a row -- here `key` is
required. These are examples of invalid input for an `object_listener`.

Classes derived from `object_listener` are not expected to handle invalid input
in a well-defined manner, so if you want to detect and debug such usage, you
want to use the `validating_filter`:

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


### Parsing ###

    :::c++
    #include <cassert>
    #include <sstream>
    #include <jsonxx/compact_writer.hpp>
    #include <jsonxx/parser.hpp>

    int main() {
        std::string json = "{\"a\":42,\"b\":[1,2,3]}";

        std::stringstream out;
        jsonxx::compact_writer iw(out);
        jsonxx::parser p(&iw);

        p.parse(json);

        assert(p.is_done());
        assert(out.str() == json);

        return 0;
    }


*/



/*doc*

Reference
---------

*/

//doc-include: object_listener.hpp
