#! /bin/bash

echo "Compiling..."
if clang++ -std=c++11 -stdlib=libc++ *.cpp -o main ; then
	echo "Deleting..."
	rm db_*
	echo "Building DB..."
	./main -b db 9000
	echo "Ingesting log.txt..."
	./main -i db outputlog.txt
	echo "Searching for bad.txt ---> results2.txt..."
	./main -s db bad.txt 6 results2.txt
	echo "Here are your results: "
	more results2.txt
fi
