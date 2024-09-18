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
	base58\
	base64\
	hash/sha256\

TOOLS =\

TESTS =\
	test-hex\
	test-baseN\
	test-base58\
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
CFLAGS += -Xlinker -rpath=${LIBDIR}
-lLIB = -l:lib${LIB}${-g}.so
else
-lLIB = -l:lib${LIB}${-g}.a
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
install:\
	build\
	${USRINC}/${LIB}.hpp\
	${patsubst %, ${USRINC}/${LIB}/%.hpp, ${OBJS}}\
	${patsubst %, ${USRLIB}/lib${LIB}${-g}%, .so .a}\
	${patsubst %, ${USRBIN}/%${-g}, ${TOOLS}}

uni: uninstall
uninstall:
	rm -f ${USRINC}/${LIB}.hpp
	rm -f ${patsubst %, ${USRINC}/${LIB}/%.hpp, ${OBJS}}
	rm -f ${patsubst %, ${USRLIB}/lib${LIB}${-g}%, .so .a}
	rm -f ${patsubst %, ${USRBIN}/%${-g}, ${TOOLS}}

clean:
	rm -rf ${OBJDIR}/* ${LIBDIR}/* ${BINDIR}/*

ifneq (${OBJS},)

library: ${DIRS} ${patsubst %, ${LIBDIR}/lib${LIB}${-g}%, .so .a}

${OBJDIR}/%${-g}.o: ${SRCDIR}/%.cpp ${INCDIR}/${LIB}/%.hpp
	${CC} -o $@ -c $< -I${INCDIR} ${-l} ${CFLAGS}

${LIBDIR}/lib${LIB}${-g}.so: ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
	${CC} -shared -o $@ $^ 

${LIBDIR}/lib${LIB}${-g}.a: ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
	ar rcs $@ $^

${USRINC}/%: ${INCDIR}/%
	install -Dm644 $< $@

${USRLIB}/lib${LIB}${-g}%: ${LIBDIR}/lib${LIB}${-g}%
	install -Dm755 $< $@

endif
ifneq (${TOOLS},)

tools: library ${DIRS} ${patsubst %, ${BINDIR}/%${-g}, ${TOOLS}}

${BINDIR}/%${-g}: ${SRCDIR}/%.cpp ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
	${CC} -o $@ $< -I${INCDIR} -L${LIBDIR} ${-l} ${-lLIB} ${CFLAGS}

${USRBIN}/%${-g}: ${BINDIR}/%${-g}
	install -Dm755 $< $@

endif
ifneq (${TESTS},)

tests: library ${DIRS} ${patsubst %, ${BINDIR}/%${-g}, ${TESTS}}
	${patsubst %, ./${BINDIR}/%${-g};, ${TESTS}}

${BINDIR}/%${-g}: ${TESTDIR}/%.cpp ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
	${CC} -o $@ $< -I${INCDIR} -L${LIBDIR} ${-l} ${-lLIB} -lgtest ${CFLAGS}

endif

${DIRS}:
	mkdir $@