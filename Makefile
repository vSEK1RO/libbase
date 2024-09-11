-l =

SHARED ?= false
DEBUG ?= false

.PHONY: build i install uni uninstall\
		tools library tests clean

LIB = base
OBJS =\
	hex\
	baseN\
	hash/sha256\

TOOLS =\

TESTS =\

ifeq (${origin CC}, default)
CC = g++
endif
CFLAGS = -Wall -Wextra -Werror -Wno-unused-result -fPIC
ifneq (${DEBUG}, false)
CFLAGS += -fsanitize=address,undefined -g -Og
-g = -g
else
CFLAGS += -O3
endif

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
LIBDIR = lib
TESTDIR = test
USRDIR = /usr
DIRS =\
	${BINDIR}\
	${BINDIR}/hash\
	${OBJDIR}\
	${OBJDIR}/hash\
	${LIBDIR}\

build: library tools

i: install
install: build ${patsubst %, ${USRDIR}/${BINDIR}/btc-%${-g}, ${TOOLS}}\
	${USRDIR}/${LIBDIR}/lib${LIB}${-g}.a ${USRDIR}/${LIBDIR}/lib${LIB}${-g}.so
	
uni: uninstall
uninstall:
	rm -f ${USRDIR}/${LIBDIR}/lib${LIB}${-g}.a
	rm -f ${USRDIR}/${LIBDIR}/lib${LIB}${-g}.so
	rm -f ${patsubst %, ${USRDIR}/${BINDIR}/btc-%${-g}, ${TOOLS}}

clean:
	rm -rf ${OBJDIR} ${LIBDIR} ${BINDIR}

ifneq (${OBJS},)

library: ${DIRS} ${LIBDIR}/lib${LIB}${-g}.a ${BINDIR}/lib${LIB}${-g}.so

${OBJDIR}/%${-g}.o: ${SRCDIR}/%.cpp ${INCDIR}/${LIB}/%.hpp
	${CC} -o $@ -c $< -I${INCDIR} ${-l} ${CFLAGS}

${BINDIR}/lib${LIB}${-g}.so: ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
	${CC} -shared -o $@ $^ 

${LIBDIR}/lib${LIB}${-g}.a: ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
	ar rcs $@ $^

${USRDIR}/${LIBDIR}/lib${LIB}${-g}.so: ${BINDIR}/lib${LIB}${-g}.so
	cp $< $@

${USRDIR}/${LIBDIR}/lib${LIB}${-g}.a: ${LIBDIR}/lib${LIB}${-g}.a
	cp $< $@

endif
ifneq (${TOOLS},)

tools: library ${DIRS} ${patsubst %, ${BINDIR}/%${-g}, ${TOOLS}}

${BINDIR}/%${-g}: ${SRCDIR}/%.cpp ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
ifneq (${SHARED}, false)
	${CC} -o $@ $< -I${INCDIR} -L${BINDIR} ${-l} -l${LIB}${-g} ${CFLAGS}
else
	${CC} -o $@ $< -I${INCDIR} -L${LIBDIR} ${-l} -l${LIB}${-g} ${CFLAGS}
endif

${USRDIR}/${BINDIR}/btc-%${-g}: ${BINDIR}/%${-g}
	cp $< $@

endif
ifneq (${TESTS},)

tests: library ${DIRS} ${patsubst %, ${BINDIR}/%${-g}, ${TESTS}}
	${patsubst %, ./${BINDIR}/%${-g};, ${TESTS}}

${BINDIR}/%${-g}: ${TESTDIR}/%.cpp ${patsubst %, ${OBJDIR}/%${-g}.o, ${OBJS}}
	${CC} -o $@ $< -I${INCDIR} -L${LIBDIR} ${-l} -l${LIB}${-g} -lgtest ${CFLAGS}

endif

${DIRS}:
	mkdir $@