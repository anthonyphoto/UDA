include $(HOME)/common/env_proc_32.mk
include $(HOME)/common/env_uda.mk

LOCALFLAGS=-D_NDEBUG_ -D_UDA1_ -Aa -I$(HOME)/src/AAD/INCLUDE

EXE =  AADMainTest  
OBJS = UDRManager.o UdrBds.o BDRManager.o AbnormalAcctDetector.o CTime.o AADMainTest.o $(UDASQLLIBES) $(UDALINKLIBES)

all : $(EXE)

$(EXE) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(PROLDLIBS) $(UDASQLLIBES) $(UDALINKLIBES)

install : $(EXE)
	$(INSTALL2) $(EXE) $(COMMBINPATH)/
	rm -f $(EXE) *.o *.lis core 

clean :	
	rm -f $(EXE) *.o *.lis core 
	touch *.*




