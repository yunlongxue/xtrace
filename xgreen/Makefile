######################################################################
#  Create by XUEYL 2008-10-17. xueyunlong@gmail.com
#  Version history                                 
#  2008-10-17 V1.0                                     
######################################################################

include ../Makefile.config

INCL = 
LIB_DIR = 
LIBS = 

PRODUCTS = xgreen.a
all : $(PRODUCTS)

xgreen.a : xgreen.o
	$(AR) $(ARFLAGS) $@ $?
	cp	$@	../xlib/

.PHONY : clean
clean :
	ls -1 $(PRODUCTS) 2>/dev/null | xargs -i mv {} $(TMPDIR) 
	ls -1 *.o *.so tp* *.lis *.a 2>/dev/null | xargs -i mv {} $(TMPDIR) 
	ls -1 *.pc 2>/dev/null | sed -e "s/.pc/.c/g" | xargs -i mv {} $(TMPDIR)
	ls -1 *.pc 2>/dev/null | sed -e "s/.pc/.cpp/g" | xargs -i mv {} $(TMPDIR) 
	
