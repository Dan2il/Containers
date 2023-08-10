test: gtest_install
	cmake -S . -B build
	cmake --build build
	cd build && ctest --rerun-failed --output-on-failure

gtest_install:
	if test ! -d googletest; \
	then \
	`git clone https://github.com/google/googletest.git -b v1.13.0`; \
	cd googletest; cmake -B build googletest/build ..; \
	fi

valgrind: test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=_valgrind_res.txt ./build/s21_containers_tests

leaks: test
	leaks -atExit -- ./build/s21_containers_tests

style:
	clang-format -n ./source/*/*.cc source/*/*.h source/*.h

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --language=c++ --std=c++17 ./source/*/*.cc ./source/*/*.h ./source/*.h 
	# cppcheck --enable=all --suppress=missingIncludeSystem source/list/*

clean:
	rm -rf build report *.o *.a *.html *.css _valgrind_res.txt

