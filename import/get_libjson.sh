#!/bin/bash

SOURCE=http://github.com/vincenthz/libjson.git
TARGET=libjson
TAG=v0.8

git clone "$SOURCE" "$TARGET" &&

pushd "$TARGET" &&
git checkout "$TAG" 2>/dev/null &&
popd
