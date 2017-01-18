CC = g++
CFLAGS = -std=c++11
TARGET = bloom_filters_run
DIR = app
FILES = SeededHash.cpp BloomFilter.cpp

compile: $(TARGET)
$(TARGET):
	@cd app; \
	$(CC) $(CFLAGS) -o $(TARGET) $(FILES); \
	echo Program compiled; \
	echo Run file $(TARGET) or 'make run'; \
	cd ..

run:
	@cd app; \
	./$(TARGET); \
	echo To view plot, 'make plot'; \
	cd ..

all:
	@@$(MAKE) compile; \
	@$(MAKE) run

plot:
	@echo Open Browser to http://localhost:8000; \
	python -m SimpleHTTPServer

clean:
	@cd app; \
	$(RM) $(TARGET); \
	echo Executables removed; \
	cd ..