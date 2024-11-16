all: build

build:
	cmake -DCMAKE_BUILD_TYPE=debug -G "Unix Makefiles" -B cmake-build-debug
	make -C cmake-build-debug
	cp cmake-build-debug/dsa ./dsa

test:
	cmake -DCMAKE_BUILD_TYPE=test -G "Unix Makefiles" -B cmake-build-test
	make -C cmake-build-test

run-test: test
	./cmake-build-test/tests/dsa-test

coverage:
	gcovr -e "tst/" -e "cmake-build-test/"

clean-build:
	rm -rf cmake-build-debug
	rm -f ./dsa

clean-test:
	rm -rf cmake-build-test

clean: clean-build clean-test

re: clean all

.PHONY: build test run-test coverage clean-build clean-test clean re