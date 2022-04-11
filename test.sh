#!/bin/sh

./private <<EOF
600 URI Acquire
URI: private://httpserver/some/path/image.jpeg
Filename: /tmp/image.jpeg.test
Maximum-Size: 10000000
Fail-Ignore: true

600 URI Acquire
URI: foo://ftpserver/lorem.txt
Filename: /tmp/lorem.txt.test
Maximum-Size: 10000000
Fail-Ignore: true




EOF
