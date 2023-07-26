#!/bin/bash

# USAGE: 
# To run all the tests 
#   ./runtest.sh 
# To run a single test, e.g., 13
#   ./runtest.sh 13

TESTS0="00 01 02 03 04"
TESTS1="10 11 12 13 14 15 16 17 18 19"
TESTS2="20 21 22 23 24 25 26 27 28 29"
TESTS3="30 31 32 33 34 35 36 37 38 39"
TESTS="$TESTS0 $TESTS1 $TESTS2 $TESTS3"
EXE=prosim

if [ -x $EXE ]; then
	EXECDIR=.
elif [ -x  cmake-build-debug/$EXE ]; then
	EXECDIR=cmake-build-debug
else
	echo Cannot find $EXE
	exit
fi

if [ $1"X" == "X" ]; then
	for i in $TESTS; do
		./tests/test.sh $i $EXECDIR $EXE
	done
else
	./tests/test.sh $1 $EXECDIR $EXE
fi
