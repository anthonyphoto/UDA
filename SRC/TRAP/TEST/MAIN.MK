##########################
# Define
##########################
CC       = aCC
PROC     = proc

PINCLUDE =
PSYSINC  = sys_include='(/opt/aCC/include/SC,/opt/aCC/include/iostream,/opt/aCC/include/rw,/usr/include,/opt/aCC/include)'
PROCINC  = INCLUDE='($(PINCLUDE))'
PCLIBES  = -I$(ORACLE_HOME)/precomp/public -L$(ORACLE_HOME)/lib -lclntsh




##########################
# Suffixes
##########################
.SUFFIXES:.pc .cpp .o
.pc.cpp:
	$(PROC)   iname=$< code=cpp cpp_suffix=cpp $(PSYSINC) $(PROCINC)
	@#$(PROC) iname=$< code=cpp cpp_suffix=cpp $(PSYSINC) $(PROCINC) sqlcheck=semantics userid= 
	
.pc.o:
	$(PROC)   iname=$< code=cpp cpp_suffix=cpp $(PROCINC) $(PSYSINC)
	@#$(PROC) iname=$< code=cpp cpp_suffix=cpp $(PROCINC) $(PSYSINC) sqlcheck=semantics userid= 
	$(CC)     -c -o $*.o $*.cpp $(PCLIBES)
	
.cpp.o:
	$(CC)     -c -o $*.o $*.cpp $(PCLIBES)
