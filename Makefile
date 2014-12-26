all: build

build:
	mkdir build -p
	cd build; cmake .. ; make -j4

test: build
	cd build; ./http_test

clean:
	rm -rf build

.PHONY: build clean

