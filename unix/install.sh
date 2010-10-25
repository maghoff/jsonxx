#!/bin/bash

# Use this script to install jsonxx system wide on a UNIX-like system.
#
# Run as super user.

SCRIPT_DIR="$(dirname "$0")"
PROJECT_DIR="$SCRIPT_DIR/.."

PREFIX=/usr
LIB_DIR="$PREFIX/lib"
INCLUDE_DIR="$PREFIX/include/jsonxx"
PKGCONFIG_DIR="$PREFIX/lib/pkgconfig"

mkdir -p "$LIB_DIR"
cp "$PROJECT_DIR/build/release/src/libjsonxx.a" "$LIB_DIR/"

mkdir -p "$INCLUDE_DIR"
cp "$PROJECT_DIR/src/"*.hpp "$INCLUDE_DIR/"

mkdir -p "$PKGCONFIG_DIR"
cp "$PROJECT_DIR/unix/jsonxx.pc" "$PKGCONFIG_DIR/"
