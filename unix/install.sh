#!/bin/bash

# Use this script to install jsonxx system wide on a UNIX-like system.
#
# Run as super user from project root.

PREFIX=/usr
LIB_DIR="$PREFIX/lib"
INCLUDE_DIR="$PREFIX/include/jsonxx"

cp build/release/src/libjsonxx.a "$LIB_DIR/"

mkdir -p "$INCLUDE_DIR"
cp src/*.hpp "$INCLUDE_DIR/"
