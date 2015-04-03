######################################################################
#  Create by XUEYL 2014-07-07. xueyunlong@gmail.com
#  Version history                                 
######################################################################
TMPDIR = $(HOME)/tmp/

DEBUG = -O3
#DEBUG = -DDEBUG -g -Wall

#MACHINE	= -m32 -D__USE_FILE_OFFSET64 -D__USE_LARGEFILE64 -D_FILE_OFFSET_BITS=64

OSTYPE = -DOS_LINUX

FLAGS = $(DEBUG) $(MACHINE) $(OSTYPE)

INCL = 
LIB_DIR = 
LIBS = 

AR = ar
ARFLAGS = -rcs

CC = cc 
CLINK = cc
CSTD = -std=c99
CFLAGS = -c $(FLAGS) $(CSTD) $(INCL)
CLDFLAGS = $(LIB_DIR) $(LIBS) 

CPP = g++
CPPLINK	= g++
CPPFLAGS = -c -std=c++11 $(FLAGS) $(INCL)
CPPLDFLAGS = 

PROC= proc
PROCFLAGS = lines=yes parse=none mode=oracle close_on_commit=no dynamic=ansi

PROCPPFLAGS = lines=yes parse=none mode=oracle close_on_commit=no dynamic=ansi cpp_suffix=cpp
#LDFLAGS = -shared $(MACHINE) -lm -Wl,-Bsymbolic,--allow-shlib-undefined --no-whole-archive

GO = gccgo 
GOLINK = gccgo
GOFLAGS = -c $(FLAGS)
GOLDFLAGS = 



#
# $@: holds the name of target
# $<: holds the source file name
# $*: holds the name equal to "%"
# 

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@
%.o : %.cpp %.h
	$(CPP) $(CPPFLAGS) $< -o $@
%.o : %.cc	%.h
	$(CPP) $(CPPFLAGS) $< -o $@
%.o : %.pc
	$(PROC) $(PROCFLAGS) iname=$< oname=$*.c
	$(CC) $(CFLAGS) $*.c -o $@
%.o : %.go
	$(GO) $(GOFLAGS) $< -o $@
%.o : %.l
	flex	-o $<.c  $< 
	cc -c	$<.c -o $@


all : xfoot.a

xfoot.a : xfoot.o
	$(AR) $(ARFLAGS) $@ $?

install:
	-cp xfoot.a ../xlib

.PHONY : clean
clean:
	-ls -1 xfoot.a 2>/dev/null | xargs -i mv {} $(TMPDIR)
	-ls -1 *.o *.so tp* *.lis *.a 2>/dev/null | xargs -i mv {} $(TMPDIR)
	-ls -1 *.pc 2>/dev/null | sed -e "s/.pc/.c/g" | xargs -i mv {} $(TMPDIR)
	-ls -1 *.pc 2>/dev/null | sed -e "s/.pc/.cpp/g" | xargs -i mv {} $(TMPDIR)
