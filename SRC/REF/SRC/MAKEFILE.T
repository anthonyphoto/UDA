include $(HOME)/common/env_proc_32.mk
include $(HOME)/common/env_uda.mk

EXE = RefMainT 
OBJS = FileLogWrite.o RefMainT.o refmanager.o t_meta_action.o t_017pdsn_out.o collectorRule.o pdsnRule.o SqlErr.o convReFormat.o SendLogManageT.o $(UDASQLLIBES) $(UDALINKLIBES)

all : $(EXE)

$(EXE) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(PROLDLIBS) $(UDASQLLIBES) $(UDALINKLIBES)

install : $(EXE)
	$(INSTALL2) $(EXE) $(COMMBINPATH)/
	rm -rf $(EXE) *.o core 

clean :	
	rm -rf $(EXE) *.o core 
	touch *.cpp *.pc




