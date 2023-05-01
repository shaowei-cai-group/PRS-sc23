#!/bin/sh
cd common
docker build -t satcomp-light:common .
cd ..

cd leader
docker build -t satcomp-light:leader .
cd ..

cd worker
docker build -t satcomp-light:worker .
cd ..