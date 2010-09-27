#!/usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess, os

SOURCE="http://github.com/vincenthz/libjson.git"
TARGET="libjson"
TAG="126c44701f34fe5cd92732b904e4bb216adc906d" # ~v0.9


def get_libjson():
    if os.path.isdir(TARGET):
        print "%s exists. Declaring successful acquisition of libjson %s" % (TARGET, TAG)
        return 0

    subprocess.check_call(['git', 'clone', SOURCE, TARGET])
    subprocess.check_call(['git', 'checkout', TAG], cwd = TARGET)

    return 0


if __name__ == '__main__':
    import sys
    sys.exit(get_libjson())
