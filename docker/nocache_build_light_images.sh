#!/bin/sh
cd common
docker build --no-cache -t satcomp-light:common .
cd ..

cd leader
docker build --no-cache -t satcomp-light:leader .
cd ..

cd worker
docker build --no-cache -t satcomp-light:worker .
cd ..
