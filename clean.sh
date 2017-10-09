#!/bin/sh

echo "clean all *.aux,*.o, *.log!"
find . -name "*.aux" |xargs rm -f
find . -name "*.o" |xargs rm -f
find . -name "*.log"|xargs rm -f


