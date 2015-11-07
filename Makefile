# Makefile for the lambda calculator.
# This is just a plug in of my most generaic Makefile.

# Call without target to make the executable
#      with clean to remove intermediate files.
#      with deepclean to remove all generated files.
#      with test to run the program with gdb as a test wrapper.

### Variables & Setup

# The name of the executable.
EXE=lambda

# The base name of every code file used to create the binary.
FILENAMES=main element

# Leave empty to compile for release, otherwise system compiles for debugging.
USE_DEBUG=

# C++ Compiler
CXX=g++
# Flags for compilation
CXXFLAGS=-Wall --std=c++11
# Additional libraries for the linking step.
CXXLIBS=-lsfml-graphics -lsfml-window -lsfml-system
# Additional flags for debug builds.
DEBUG=-ggdb -DDEBUG=true

TMPDIR=.obj
CODEDIR=code
CPPFILES=${FILENAMES:%=${CODEDIR}/%.cpp}
OBJFILES=${FILENAMES:%=${TMPDIR}/%.o}
DEPFILES=${OBJFILES:%.o=%.d}

CXXFLAGS+=${if ${USE_DEBUG},${DEBUG},}

.PHONY : all clean test

### Recipes and Rules

all : ${EXE}

# Rule for the binary
${EXE} : ${OBJFILES}
	${CXX} ${CXXFLAGS} ${CXXLIBS} $^ -o $@

# Rule for object files
${OBJFILES} : ${TMPDIR}/%.o : ${CODEDIR}/%.cpp | ${TMPDIR}
	${CXX} ${CXXFLAGS} -MMD -c $< -o $@

# Rule for the temperary directory
${TMPDIR} :
	mkdir ${TMPDIR}

# Phony rule for cleaning intermediate files
clean :
	-rm ${OBJFILES}
	-rm ${DEPFILES}
	-rmdir ${TMPDIR}

# Phony rule for cleaning generated files
deepclean : clean
	-rm inkv7

# Phony rule for running the test wrapper
test : ${EXE}
	gdb ./${EXE}

### Include Depends Files

-include ${DEPFILES}
