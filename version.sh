#!/usr/bin/env bash
TAG=`git tag -l | tail -1`
REV=`git rev-list mingw | sort | wc -l | awk '{print $1}'`
GHASH=`git rev-list HEAD -n 1 | cut -c 1-7`
echo "[$TAG] r$REV git-$GHASH"