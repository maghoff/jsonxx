#!/usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess, os, urllib2, hashlib

SOURCE="http://github.com/vincenthz/libjson.git"
TARGET="libjson"
TAG="126c44701f34fe5cd92732b904e4bb216adc906d" # ~v0.9

# Direct links to files, in case git is not available:
BASE_URI = 'http://github.com/vincenthz/libjson/raw/%s/' % TAG
FILES = [
    ('json.c', 'c3451003d73a513c37bffca9927b48185426c45f'),
    ('json.h', 'b6837a46023c7bcf470a46e1514803acc4f2a41a'),
]


def acquire_checkout():
    if not os.path.isdir(TARGET):
        subprocess.check_call(['git', 'clone', SOURCE, TARGET])

def ensure_revision():
    ret = subprocess.call(['git', 'checkout', TAG], cwd = TARGET)

    if ret != 0:
        subprocess.check_call(['git', 'pull', SOURCE, 'master'], cwd = TARGET)
        subprocess.check_call(['git', 'checkout', TAG], cwd = TARGET)

def get_with_git():
    acquire_checkout()
    ensure_revision()


def get_with_http():
    try:
        os.mkdir(TARGET)
    except OSError:
        pass

    for filename, desired_hash in FILES:
        local_hash = ''
        f = None
        try:
            f = open(os.path.join(TARGET, filename), 'rb')
            contents = f.read()
            local_hash = hashlib.sha1(contents).hexdigest()
        except:
            pass
        finally:
            if f: f.close()

        if local_hash != desired_hash:
            uri = BASE_URI + filename
            print "Downloading %s..." % uri
            s = urllib2.urlopen(uri)
            t = open(os.path.join(TARGET, filename), 'wb')
            t.write(s.read())
            t.close()
            s.close()


def get_libjson():
    try:
        get_with_git()
    except OSError:
        print "Failed to use git. Trying http..."
        get_with_http()

    return 0


if __name__ == '__main__':
    import sys
    sys.exit(get_libjson())
