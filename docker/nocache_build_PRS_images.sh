#!/bin/sh

docker build --no-cache -t satcomp-prs-nopre:common ../ --file common/Dockerfile
docker build --no-cache -t satcomp-prs-nopre:leader ../ --file leader/Dockerfile