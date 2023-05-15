#!/bin/sh

docker build -t satcomp-nopre-prs:common ../ --file common/Dockerfile
docker build -t satcomp-nopre-prs:leader ../ --file leader/Dockerfile