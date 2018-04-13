#======================================================================#
#                                                                      #
#      Makefile														   # 
#      -- build the QuEST function library and link user sources       #
#                                                                      #
#======================================================================#

#
# --- USER CONFIG
#

# COMPILER options: {GNU, INTEL}
COMPILER = GNU

# EXECUTABLE TO GENERATE
EXE = my_output

# USER SOURCE FILES
# This makefile expects all user sources to be in the root directory.
# If using more than one source, separate by spaces
MY_C_SOURCES = my_code

# ENABLE DISTRIBUTED PROCESSING (ALLOWS MULTIPLE NODES)
# On: 1, Off: 0
USE_MPI=0

# ENABLE MULTIPLE THREADS PER PROCESS (RECOMMENDED)
# On: 1, Off: 0
USE_OPENMP=1

# PATH TO QUEST LIBRARY SOURCES FROM ROOT DIRECTORY
QUEST_DIR = ../QuEST





#======================================================================#
#                                                                      #
#      Makefile execution 											   #
#                                                                      #
#======================================================================#


#
# --- COMPILER
#
ifneq ($(USE_MPI), 1)
	ifeq ($(COMPILER), GNU)
		# COMPILER = GNU
		CC = gcc
	else ifeq ($(COMPILER), INTEL)
		# COMPILER = INTEL
		CC = icc
	else 
		$(error " *** build error: invalid compiler")
	endif
else 
	# MPI COMPILER
	CC = mpicc
endif


#
# --- OPENMP FLAGS
#
ifeq ($(COMPILER), GNU)
	# Compiler = GNU
	CFLAGS_OMP=-fopenmp
else 
	# Compiler = INTEL
	CFLAGS_OMP=-openmp
endif

ifneq ($(USE_OPENMP), 1)
	# disable OPENMP
	CFLAGS_OMP=
endif


#
# --- OTHER COMPILER FLAGS
#
ifeq ($(COMPILER), GNU)
	# GCC compilers
	CFLAGS     = -O2 -std=c99 -mavx -Wall
else ifeq ($(COMPILER), INTEL)
	# Intel compilers
	CFLAGS     = -O2 -std=c99 -fprotect-parens -Wall -xAVX -axCORE-AVX2 -diag-disable cpu-dispatch
else 
	$(error " *** error: invalid compiler")
endif


#
# --- GSL link
#
GSL_INCLUDE = -I/data/oums-quantopo/corp2627/gsl/include
GSL_LINK = -L/data/oums-quantopo/corp2627/gsl/lib


#
# --- libraries
#
LIBS = -lm -lgsl -lgslcblas


#
# --- targets
#
USER_OBJ = $(addsuffix .o, $(MY_C_SOURCES))
OBJ = qubits.o mt19937ar.o
OBJ += $(USER_OBJ)

# Certain API functions have different implementations for MPI or non-MPI:
# choose the right file
ifneq ($(USE_MPI), 0)
	OBJ += qubits_env_mpi.o
else
	OBJ += qubits_env_local.o
endif


#
# --- rules
#
%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_OMP) $(GSL_INCLUDE) -c $<

%.o: $(QUEST_DIR)/%.c
	$(CC) $(CFLAGS) $(CFLAGS_OMP) $(GSL_INCLUDE) -c $<


#
# --- build
#
default:	$(EXE)

$(EXE):		$(OBJ)
		$(CC) $(CFLAGS) $(CFLAGS_OMP) $(GSL_INCLUDE) -o $(EXE) $(OBJ) $(LIBS) $(GSL_LINK)

.PHONY:		clean veryclean
clean:
		/bin/rm -f *.o $(EXE)
veryclean:	clean
		/bin/rm -f *.h~ *.c~ makefile~
