######################################################################
#  Create by XUEYL 2008-10-17. xueyunlong@gmail.com
#  Version history                                 
#  2008-10-17 V1.0                                     
######################################################################

include Makefile.config

INCL = -I./xlibsrc/ -I./xgreen/
LIB0 = ./xlib/xparser.a
LIBA_DIR = ./xlib
LIBA_EXE = xlib.a xgreen.a xfoot.a
LIBA += $(foreach lib, $(LIBA_EXE), $(LIBA_DIR)/$(lib))

all : $(LIBA_EXE) xtrace

xtrace: xmain.o xtrace.o $(LIBA) $(LIB0)
	$(CPPLINK) -o $@ $+ $(CPPLDFLAGS)
	$(STRIP) $@

xlib.a:
	cd	xlibsrc/;	make

xfoot.a:
	cd  xfoot/; make; 

xgreen.a:
	cd	xgreen/;	make

.PHONY : clean
clean:
	-cd	xlibsrc/;	make clean
	-cd	xfoot/;	make clean
	-cd	xgreen/;	make clean
	-ls -1 xtrace 2>/dev/null | xargs -i mv {} $(TMPDIR)
	-ls -1 *.o *.so tp* *.lis *.a 2>/dev/null | xargs -i mv {} $(TMPDIR)
	-ls -1 *.pc 2>/dev/null | sed -e "s/.pc/.c/g" | xargs -i mv {} $(TMPDIR)
	-ls -1 *.pc 2>/dev/null | sed -e "s/.pc/.cpp/g" | xargs -i mv {} $(TMPDIR)
	
