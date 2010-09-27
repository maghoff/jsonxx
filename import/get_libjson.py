#!/usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess, os

SOURCE="http://github.com/vincenthz/libjson.git"
TARGET="libjson"
TAG="126c44701f34fe5cd92732b904e4bb216adc906d" # ~v0.9


def acquire_checkout():
    if not os.path.isdir(TARGET):
        subprocess.check_call(['git', 'clone', SOURCE, TARGET])

def ensure_revision():
    ret = subprocess.call(['git', 'checkout', TAG], cwd = TARGET)

    if ret != 0:
        subprocess.check_call(['git', 'pull', SOURCE, 'master'], cwd = TARGET)
        subprocess.check_call(['git', 'checkout', TAG], cwd = TARGET)


def get_libjson():
    acquire_checkout()
    ensure_revision()

    return 0


if __name__ == '__main__':
    import sys
    sys.exit(get_libjson())
