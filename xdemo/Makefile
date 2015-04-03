######################################################################
#  Create by XUEYL 2008-10-17. xueyunlong@gmail.com
#  Version history                                 
#  2008-10-17 V1.0                                     
######################################################################

include ./Makefile.config

INCL = 
LIB_DIR = 
LIBS = 

XTRACE = ./xtrace
PRODUCT = xdump xmultiProcess xmultiThread xsoPart

all : $(PRODUCT)

xdump : xdump.o
	$(CLINK) -o $@ $+ $(CLDFLAGS)

xmultiProcess : xmultiProcess.o
	$(CLINK) -o $@ $+ $(CLDFLAGS)

xmultiThread : xmultiThread.o
	$(CLINK) -o $@ $+ $(CLDFLAGS) -lpthread

xsoPart : xsoPart0.o xsoPart1.so xsoPart2.o xsoPart3.so
	$(CPPLINK) -o $@ $+ $(CPPLDFLAGS) -lpthread

.PHONY : clean
clean:
	-$(XTRACE) -c xdump.c
	-$(XTRACE) -c xmultiProcess.c
	-$(XTRACE) -c xmultiThread.c
	-$(XTRACE) -c xsoPart0.cpp
	-$(XTRACE) -c xsoPart1.cpp
	-$(XTRACE) -c xsoPart2.c
	-$(XTRACE) -c xsoPart3.c
	-ls -1 $(PRODUCT) 2>/dev/null | xargs -i mv {} $(TMPDIR)
	-ls -1 *.o *.so tp* *.lis *.a 2>/dev/null | xargs -i mv {} $(TMPDIR)
	-ls -1 *.pc 2>/dev/null | sed -e "s/.pc/.c/g" | xargs -i mv {} $(TMPDIR)
	-ls -1 *.pc 2>/dev/null | sed -e "s/.pc/.cpp/g" | xargs -i mv {} $(TMPDIR)
	
