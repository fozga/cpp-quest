# Dispatcher Commands:
# make day=NN test     - compile and run tests for day NN
# make day=NN asan     - compile with AddressSanitizer and run tests
# make day=NN valgrind - run tests under Valgrind leak checker
# make day=NN clean    - clean build artifacts for day NN

.PHONY: test asan valgrind clean

test:
	$(MAKE) -C day$(day)/solution test

asan:
	$(MAKE) -C day$(day)/solution asan

valgrind:
	$(MAKE) -C day$(day)/solution valgrind

clean:
	$(MAKE) -C day$(day)/solution clean
