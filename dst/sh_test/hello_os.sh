#!/bin/bash
sed -n -e '8p' -e '32p' -e '128p' -e '512p' -e '1024p' $1 > $2
