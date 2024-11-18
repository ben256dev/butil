.PHONY: default
default: install

.PHONY: install
install:
	@sudo mkdir -p /usr/include/butil
	@sudo cp butil.h /usr/include/butil

.PHONY: uninstall
uninstall:
	@sudo rm -rf /usr/include/butil
