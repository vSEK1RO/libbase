-l =

SHARED ?= false
DEBUG ?= false
USRDIR ?= /usr

.PHONY: build i install uni uninstall\
		tools library tests clean

LIB = base
OBJS =\
	hex\
	baseN\
	base64\
	hash/sha256\

TOOLS =\

TESTS =\
	test-hex\
	test-baseN\
	test-base64\
	hash/test-sha256\

ifeq (${origin CC}, default)
CC = g++
endif

CFLAGS = -std=c++23 -Wall -Wextra -Werror -Wno-unused-result -fPIC

ifneq (${DEBUG}, false)
CFLAGS += -fsanitize=address,undefined -g -O0
-g = -g
else
CFLAGS += -O3
endif

ifneq (${SHARED}, false)
CFLAGS += -Xlinker -rpath=${BINDIR}
endif

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
LIBDIR = lib
TESTDIR = test
USRLIB = ${USRDIR}/lib
USRBIN = ${USRDIR}/bin
USRINC = ${USRDIR}/include
DIRS =\
	${BINDIR}\
	${BINDIR}/hash\
	${OBJDIR}\
	${OBJDIR}/hash\
	${LIBDIR}\

build: library tools

i: install
install: build\
	${USRLIB}/lib${LIB}${-g}.a\
	${USRLIB}/lib${LIB}${-g}.so\
	${patsubst %, ${USRBIN}/%${-g}, ${TOOLS}}

uni: uninstall
uninstall:
	rm -f ${USRLIB}/lib${LIB}${-g}.a
	rm -f ${USRLIB}/lib${LIB}${-g}.so
	rm -f ${patsubst %, ${USRBIN}/%${-g}, ${TOOLS}}

clean:
	rm -rf ${OBJDIR}/* ${LIBDIR}/* ${BINDIR}/*

ifneq (${OBJS},)

library: ${DIRS} ${LIBDIR}/lib${LIB}${-g}.a ${BINDIR}/lib${LIB}${-g}.so

${OBJDIR}/%${-g}.o: ${SRCDIR}/%.cpp ${INCDIR}/${LIB}/%.hpp
	${CC} -o $@ -c $< -I${INCDIR} ${-l} ${CFLAGS}

${BINDIR}/lib${LIB}${-g}.so: ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
	${CC} -shared -o $@ $^ 

${LIBDIR}/lib${LIB}${-g}.a: ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
	ar rcs $@ $^

${USRLIB}/lib${LIB}${-g}.so: ${BINDIR}/lib${LIB}${-g}.so
	cp $< $@

${USRLIB}/lib${LIB}${-g}.a: ${LIBDIR}/lib${LIB}${-g}.a
	cp $< $@

endif
ifneq (${TOOLS},)

tools: library ${DIRS} ${patsubst %, ${BINDIR}/%${-g}, ${TOOLS}}

${BINDIR}/%${-g}: ${SRCDIR}/%.cpp ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
ifneq (${SHARED}, false)
	${CC} -o $@ $< -I${INCDIR} -L${BINDIR} ${-l} -l:lib${LIB}${-g}.so ${CFLAGS}
else
	${CC} -o $@ $< -I${INCDIR} -L${LIBDIR} ${-l} -l:lib${LIB}${-g}.a ${CFLAGS}
endif

${USRBIN}/%${-g}: ${BINDIR}/%${-g}
	cp $< $@

endif
ifneq (${TESTS},)

tests: library ${DIRS} ${patsubst %, ${BINDIR}/%${-g}, ${TESTS}}
	${patsubst %, ./${BINDIR}/%${-g};, ${TESTS}}

${BINDIR}/%${-g}: ${TESTDIR}/%.cpp ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
ifneq (${SHARED}, false)
	${CC} -o $@ $< -I${INCDIR} -L${BINDIR} ${-l} -l:lib${LIB}${-g}.so -lgtest ${CFLAGS}
else
	${CC} -o $@ $< -I${INCDIR} -L${LIBDIR} ${-l} -l:lib${LIB}${-g}.a -lgtest ${CFLAGS}
endif

endif

${DIRS}:
	mkdir $@