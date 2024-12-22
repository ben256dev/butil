.PHONY: default
default: install

# Detect platform
OS := $(shell uname -s | tr '[:upper:]' '[:lower:]')
ifeq ($(findstring mingw,$(OS)),mingw)
    PLATFORM = windows
else ifeq ($(OS),linux)
    PLATFORM = linux
else
    $(error Unsupported OS: $(OS))
endif

# Platform-specific commands
ifeq ($(PLATFORM),linux)
    PREFIX = /usr
    INSTALL_CMD = sudo cp
    MKDIR_CMD = sudo mkdir -p
    RM_CMD = sudo rm -rf
else ifeq ($(PLATFORM),windows)
    PREFIX = /c
    INSTALL_CMD = cp
    MKDIR_CMD = mkdir -p
    RM_CMD = rm -rf
endif

.PHONY: build
build:
	@gcc -fPIC -c butil.c -o butil.o
	@ar rcs libbutil.a butil.o

.PHONY: install
install: build
	@$(MKDIR_CMD) $(PREFIX)/include/butil
	@$(INSTALL_CMD) butil.h $(PREFIX)/include/butil
	@$(INSTALL_CMD) bstack.h $(PREFIX)/include/butil
	@$(INSTALL_CMD) libbutil.a $(PREFIX)/lib

.PHONY: uninstall
uninstall:
	@$(RM_CMD) $(PREFIX)/include/butil
	@$(RM_CMD) $(PREFIX)/lib/libbutil.a

.PHONY: clean
clean:
	@rm -f butil.o libbutil.a
