

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
	clang-format -n s21_matrix_oop/sources/*.*
	clang-format -n s21_matrix_oop/*.*
	clang-format -n tests/*.*

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_matrix_oop/

clean:
	rm -rf build report *.o *.a *.html *.css _valgrind_res.txt

