#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1979304334/Main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../brewpi_lib/dist/Release/MinGW-Windows/libbrewpi_lib.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/brewpi_sim.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/brewpi_sim.exe: ../brewpi_lib/dist/Release/MinGW-Windows/libbrewpi_lib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/brewpi_sim.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/brewpi_sim ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1979304334/Main.o: ../brewpi_cpp/Main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979304334
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979304334/Main.o ../brewpi_cpp/Main.cpp

# Subprojects
.build-subprojects:
	cd ../brewpi_lib && ${MAKE}  -f makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/brewpi_sim.exe

# Subprojects
.clean-subprojects:
	cd ../brewpi_lib && ${MAKE}  -f makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
