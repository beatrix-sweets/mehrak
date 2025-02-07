#! /bin/bash

## usa imageMagik per convertire da BMP a formato RAW, poi usa lo script
## python per creare il file header e mette un commento con file di orgine
#convert $1 -depth 8 rgb:output.raw && python3 ./scripts/generate_header.py ./output.raw && ( echo "//from file: $1" | cat - image.h > temp && mv temp image.h)

convert $1 -depth 8 rgb:output.raw && 
	python3 ./scripts/generate_header.py ./output.raw && 
	sed -i "1s#^#//from file: $1\n#" image.h
