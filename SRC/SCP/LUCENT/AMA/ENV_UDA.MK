#######################################################################
#
# Paths for include files and libraries
#
########################################################################

LLIBTHREAD = -lpthread

# uda (Common Path Define)
PROJECTPATH = $(HOME)
COMMINCPATH = $(PROJECTPATH)/include
COMMLIBPATH = $(PROJECTPATH)/lib
COMMOBJPATH = $(PROJECTPATH)/obj
COMMBINPATH = $(PROJECTPATH)/bin

# Local (Define Local Path)
PROC = proc

INSTALL1 = /usr/bin/cp
INSTALL2 = /usr/bin/mv
LOCALFLAGS = 
###############################################################################
#
# Compile option flags and link libraries
#
###############################################################################
CC = aCC
LFLAGS = -L$(COMMLIBPATH) $(LLIBTHREAD)
CFLAGS = -D_HPUX_SOURCE -D__TEST +DA1.0 +p -g -O -I$(COMMINCPATH) $(MFLAGS) $(LFLAGS) $(LOCALFLAGS)

# arcived library
UDALINKLIBES = $(COMMLIBPATH)/libuda.a
AMALINKLIBES = $(COMMLIBPATH)/ama.lib
#UDASQLLIBES = $(COMMLIBPATH)/libuda_sql.a

#complier
#####################################
# Suffix rules
#
# convert: .pc -> .cpp -> .o
#####################################
CCPSYSINC=sys_include='(/opt/aCC/include/SC,/opt/aCC/include/iostream,/opt/aCC/include/rw,/usr/include,/opt/aCC/include)'

.SUFFIXES:.pc .cpp .o  

.pc.cpp:
	$(PROC)  iname=$*.pc code=cpp cpp_suffix=cpp $(CCPSYSINC) INCLUDE=$(COMMINCPATH)

.pc.o:
	$(PROC)  iname=$*.pc code=cpp cpp_suffix=cpp $(CCPSYSINC) INCLUDE=$(COMMINCPATH)
	$(CC) -c -o $*.o $(CFLAGS) $*.cpp  -I$(ORACLE_HOME)/precomp/public 

.cpp.o:
	$(CC) -c  -o $*.o $(CFLAGS) $*.cpp  -I$(ORACLE_HOME)/precomp/public

