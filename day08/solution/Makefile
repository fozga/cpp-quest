CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O0 -g

# Default test build and run
test: tests.cpp exercise.cpp
	$(CXX) $(CXXFLAGS) tests.cpp exercise.cpp -o test_runner
	./test_runner

# AddressSanitizer build and run
asan: tests.cpp exercise.cpp
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer tests.cpp exercise.cpp -o test_runner_asan
	./test_runner_asan

# Valgrind run (expects normal binary from `test`)
valgrind: tests.cpp exercise.cpp
	$(CXX) $(CXXFLAGS) tests.cpp exercise.cpp -o test_runner_valgrind
	valgrind --leak-check=full --track-origins=yes ./test_runner_valgrind

clean:
	rm -f test_runner test_runner_asan test_runner_valgrind
