The Program is implemented in C++, and the visualization is implemented in Javascript.

The following classes are used for the application:

BloomFilter
	- Contains main implementation of Bloom Filter. 
SeededHash
	- Contains implementation of Hash Function

The code is included in the following files

# Bloom Filters:
app/
	- SeededHash.h    - header file for SeededHash class
	- SeededHash.cpp  - implementation of functions in SeededHash class
	- BloomFilter.h   - header file for BloomFilter class
	- BloomFilter.cpp - implementation of functions in BloomFilter class, and Driver code

# Visualization:
- index.html      - contains Visualization code. Plot a chart using data from tests
- lib/
	d3.legend.js    - visualization library for d3 (this library is not used in any computation of work for Bloom filters, only visualization)

# Other files
data/
	bloom_filter_out.csv - contains data generated after running the Driver code
Report/
	Report.pdf           - Report for this project
	plot.png       		 - An image of the graph ploted by the Visualization

Reademe.md           - Readme file
Makefile			 - The Make file


The Make file accepts the following targets:

make compile
	To compile the program.

make run
	To run the program.

make all
	To compile and run the program.

make plot
	To open local server so the plot can be viewed.

make clean
	To remove the compiled executable.