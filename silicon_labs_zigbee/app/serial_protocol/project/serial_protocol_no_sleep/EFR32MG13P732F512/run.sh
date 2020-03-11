#!/bin/bash
if [ $# -lt 1 ]; then
	echo "command error, please run --help"
	exit
fi

make_clean_func()
{
	echo "make clean..."
	make clean
}

make_release_func()
{
	echo "build release..."
	cp -rf ./package.json ../.
	python ../pre-build.py
	mv ../config.c ../src/
	mv ../config.h ../include/
	make -j8
}

make_debug_func()
{
	echo "build debug..."
	cp -rf ./package.json ../.
	python ../pre-build.py
	mv ../config.c ../src/
	mv ../config.h ../include/
	make -j8 DEBUG_SYMBOLS=TRUE
}

if [ $1 = "clean" ]; then
	make_clean_func
elif [ $1 = "build" ]; then
	if [ $# -eq 1 ]; then
		make_clean_func 
		make_release_func
	elif [ $2 -eq "0" ]; then
		make_clean_func
		make_release_func
	elif [ $2 -eq "1" ]; then
		make_clean_func
		make_debug_func
	else
		echo "command error, please run --help"
	fi
elif [ $1 = "--help" ]; then
	echo "parameters is clean/build"
	echo "clean: make clean"
	echo "build 0: make all for release"
	echo "build 1: make all for debug"
else
	echo "command error, please run --help"
fi
