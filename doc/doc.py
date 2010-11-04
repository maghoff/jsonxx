#!/usr/bin/env python
# -*- coding: utf-8 -*-

import textwrap, markdown, os, base64


doc_file = 'src/documentation.md'


css_file = 'doc/jsonxx.css'

header = """\
<html>
<head>
<style type="text/css">%s</style>
</head>
<body>
"""

footer = """\
</body>
</html>
"""

include_tag = '//doc-include:'
open_tag = '/*doc*'
close_tag = '*/'


def generate_doc_fragments(filename):
    root = os.path.split(filename)[0]
    with open(filename, 'r') as f:
        buf = f.read()
        i = 0
        while i < len(buf):
            inc = buf.find(include_tag, i)
            o = buf.find(open_tag, i)

            if inc != -1 and (inc < o or o == -1):
                i = inc+len(include_tag)
                end = buf.find('\n', i)
                if end == -1: end = len(buf)
                fn = buf[i:end].strip()
                for fragment in generate_doc_fragments(os.path.join(root, fn)):
                    yield fragment
                i = end
                continue

            if o == -1:
                break

            i = o + len(open_tag)

            c = buf.find(close_tag, i)
            if c == -1:
                raise Exception(
                    "%s: Open tag (%s) without matching close tag (%s)" %
                    (file, open_tag, close_tag)
                )

            i = c + len(close_tag)

            yield buf[o + len(open_tag):c]

def create_image():
    data_uri_scheme = '<img src="data:image/jpeg;base64,%s" alt="STREEM PROSESING lambdacat" />'
    with open('doc/stream_processing.jpg', 'rb') as f:
        data = f.read()
    return data_uri_scheme % base64.b64encode(data)

def main():
    document = ""

    for fragment in generate_doc_fragments(doc_file):
        sf = fragment.strip(' ')
        partitions = sf.partition('\n')
        document += partitions[0] + '\n'
        document += textwrap.dedent(partitions[2])
        document += '\n\n'

    css = open(css_file, 'r').read()

    print header % css
    print markdown.markdown(document, ['codehilite', 'toc']).replace('[[[[JSONXX:STREEM PROSESING]]]]', create_image())
    print footer

    return 0


if __name__ == '__main__':
    import sys
    sys.exit(main())
