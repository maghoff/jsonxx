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


*/

//doc-include: object_listener.hpp
