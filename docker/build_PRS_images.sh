#!/bin/sh

docker build -t satcomp-prs-nopre:common ../ --file common/Dockerfile
docker build -t satcomp-prs-nopre:leader ../ --file leader/Dockerfile