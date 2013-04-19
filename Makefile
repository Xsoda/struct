# Struct Makefile

CC=gcc
CFLAGS= -Wall -g -fPIC
WARNINGS= -Wall -W -Wstrict-prototypes -Wwrite-strings
DEBUG?= -g -ggdb

LIBNAME=libstruct
TEST=main
DYLIBSUFFIX=so
DYLIBNAME=$(LIBNAME).$(DYLIBSUFFIX)
DYLIB_MAKE_CMD=$(CC) -shared -Wl -O2 -fPIC -o $(DYLIBNAME) $(LDFLAGS)

TEST_SRC=main.c
TEST_OBJ=$(TEST_SRC:.c=.o)
DYLIB_SRC=struct.c
DYLIB_OBJ=$(DYLIB_SRC:.c=.o)

PREFIX?=/usr/local
INSTALL_LIB_PATH=$(PREFIX)/lib
INSTALL_INCLUDE_PATH=$(PREFIX)/include

INSTALL?= cp -a


all: $(DYLIBNAME) $(TEST)

$(DYLIBNAME): $(DYLIB_OBJ)
	$(DYLIB_MAKE_CMD) $(DYLIB_OBJ)

$(TEST): $(TEST_OBJ)
	export LD_LIBRARY_PATH=`pwd`
	$(CC) -W1 -O2 $< -L. -lstruct -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

install: $(DYLIBNAME)
	mkdir -p $(INSTALL_INCLUDE_PATH) $(INSTALL_LIB_PATH)
	$(INSTALL) logger.h $(INSTALL_INCLUDE_PATH)
	$(INSTALL) $(DYLIBNAME) $(INSTALL_LIB_PATH)/$(DYLIBNAME) 
	cd $(INSTALL_LIB_PATH) && ln -sf $(DYLIBNAME)

clean:
	rm -f $(DYLIBNAME) $(TEST_OBJ) $(DYLIB_OBJ)
