.PHONY: default
default: install

.PHONY: install
install:
	@gcc -fPIC -c butil.c -o butil.o
	@gcc -shared -o libbutil.so butil.o
	@sudo mkdir -p /usr/include/butil
	@sudo cp butil.h /usr/include/butil
	@sudo cp libbutil.so /usr/lib

.PHONY: uninstall
uninstall:
	@sudo rm -rf /usr/include/butil
	@sudo rm -f /usr/lib/libbutil.so

.PHONY: clean
clean:
	@rm butil.o libbutil.so
