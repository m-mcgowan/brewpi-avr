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
include makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1979302600/Actuator.o \
	${OBJECTDIR}/_ext/1979302600/Brewpi.o \
	${OBJECTDIR}/_ext/1979302600/Commands.o \
	${OBJECTDIR}/_ext/1979302600/Comms.o \
	${OBJECTDIR}/_ext/1979302600/Display.o \
	${OBJECTDIR}/_ext/1979302600/FilterCascaded.o \
	${OBJECTDIR}/_ext/1979302600/FilterFixed.o \
	${OBJECTDIR}/_ext/1979302600/GenericContainer.o \
	${OBJECTDIR}/_ext/1979302600/Memops.o \
	${OBJECTDIR}/_ext/1979302600/NullLcdDriver.o \
	${OBJECTDIR}/_ext/1979302600/Profile.o \
	${OBJECTDIR}/_ext/1979302600/Sensor.o \
	${OBJECTDIR}/_ext/1979302600/SystemProfile.o \
	${OBJECTDIR}/_ext/1979302600/Temperature.o \
	${OBJECTDIR}/_ext/1979302600/Ticks.o \
	${OBJECTDIR}/_ext/1979302600/Values.o \
	${OBJECTDIR}/_ext/1979302600/ValuesProgmem.o \
	${OBJECTDIR}/_ext/1979304334/Main.o \
	${OBJECTDIR}/_ext/1979304334/Print.o \
	${OBJECTDIR}/_ext/1979304334/timems.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/brewpi_lib \
	${TESTDIR}/TestFiles/f1

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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbrewpi_lib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbrewpi_lib.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbrewpi_lib.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbrewpi_lib.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbrewpi_lib.a

${OBJECTDIR}/_ext/1979302600/Actuator.o: ../brewpi_avr/Actuator.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Actuator.o ../brewpi_avr/Actuator.cpp

${OBJECTDIR}/_ext/1979302600/Brewpi.o: ../brewpi_avr/Brewpi.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Brewpi.o ../brewpi_avr/Brewpi.cpp

${OBJECTDIR}/_ext/1979302600/Commands.o: ../brewpi_avr/Commands.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Commands.o ../brewpi_avr/Commands.cpp

${OBJECTDIR}/_ext/1979302600/Comms.o: ../brewpi_avr/Comms.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Comms.o ../brewpi_avr/Comms.cpp

${OBJECTDIR}/_ext/1979302600/Display.o: ../brewpi_avr/Display.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Display.o ../brewpi_avr/Display.cpp

${OBJECTDIR}/_ext/1979302600/FilterCascaded.o: ../brewpi_avr/FilterCascaded.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/FilterCascaded.o ../brewpi_avr/FilterCascaded.cpp

${OBJECTDIR}/_ext/1979302600/FilterFixed.o: ../brewpi_avr/FilterFixed.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/FilterFixed.o ../brewpi_avr/FilterFixed.cpp

${OBJECTDIR}/_ext/1979302600/GenericContainer.o: ../brewpi_avr/GenericContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/GenericContainer.o ../brewpi_avr/GenericContainer.cpp

${OBJECTDIR}/_ext/1979302600/Memops.o: ../brewpi_avr/Memops.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Memops.o ../brewpi_avr/Memops.cpp

${OBJECTDIR}/_ext/1979302600/NullLcdDriver.o: ../brewpi_avr/NullLcdDriver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/NullLcdDriver.o ../brewpi_avr/NullLcdDriver.cpp

${OBJECTDIR}/_ext/1979302600/Profile.o: ../brewpi_avr/Profile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Profile.o ../brewpi_avr/Profile.cpp

${OBJECTDIR}/_ext/1979302600/Sensor.o: ../brewpi_avr/Sensor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Sensor.o ../brewpi_avr/Sensor.cpp

${OBJECTDIR}/_ext/1979302600/SystemProfile.o: ../brewpi_avr/SystemProfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/SystemProfile.o ../brewpi_avr/SystemProfile.cpp

${OBJECTDIR}/_ext/1979302600/Temperature.o: ../brewpi_avr/Temperature.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Temperature.o ../brewpi_avr/Temperature.cpp

${OBJECTDIR}/_ext/1979302600/Ticks.o: ../brewpi_avr/Ticks.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Ticks.o ../brewpi_avr/Ticks.cpp

${OBJECTDIR}/_ext/1979302600/Values.o: ../brewpi_avr/Values.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Values.o ../brewpi_avr/Values.cpp

${OBJECTDIR}/_ext/1979302600/ValuesProgmem.o: ../brewpi_avr/ValuesProgmem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/ValuesProgmem.o ../brewpi_avr/ValuesProgmem.cpp

${OBJECTDIR}/_ext/1979304334/Main.o: ../brewpi_cpp/Main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979304334
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979304334/Main.o ../brewpi_cpp/Main.cpp

${OBJECTDIR}/_ext/1979304334/Print.o: ../brewpi_cpp/Print.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979304334
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979304334/Print.o ../brewpi_cpp/Print.cpp

${OBJECTDIR}/_ext/1979304334/timems.o: ../brewpi_cpp/timems.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979304334
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979304334/timems.o ../brewpi_cpp/timems.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/brewpi_lib: ${TESTDIR}/_ext/1356371059/ArrayEepromAccess_Test.o ${TESTDIR}/_ext/1041936519/TemperatureFormatsTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc}   -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/brewpi_lib $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f1: ${TESTDIR}/_ext/1356371059/newsimpletest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/_ext/1356371059/ArrayEepromAccess_Test.o: ../brewpi_cpp/test/ArrayEepromAccess_Test.cpp 
	${MKDIR} -p ${TESTDIR}/_ext/1356371059
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${TESTDIR}/_ext/1356371059/ArrayEepromAccess_Test.o ../brewpi_cpp/test/ArrayEepromAccess_Test.cpp


${TESTDIR}/_ext/1041936519/TemperatureFormatsTest.o: ../brewpi_avr/test/TemperatureFormatsTest.cpp 
	${MKDIR} -p ${TESTDIR}/_ext/1041936519
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${TESTDIR}/_ext/1041936519/TemperatureFormatsTest.o ../brewpi_avr/test/TemperatureFormatsTest.cpp


${TESTDIR}/_ext/1356371059/newsimpletest.o: ../brewpi_cpp/test/newsimpletest.cpp 
	${MKDIR} -p ${TESTDIR}/_ext/1356371059
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/_ext/1356371059/newsimpletest.o ../brewpi_cpp/test/newsimpletest.cpp


${OBJECTDIR}/_ext/1979302600/Actuator_nomain.o: ${OBJECTDIR}/_ext/1979302600/Actuator.o ../brewpi_avr/Actuator.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Actuator.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Actuator_nomain.o ../brewpi_avr/Actuator.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Actuator.o ${OBJECTDIR}/_ext/1979302600/Actuator_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Brewpi_nomain.o: ${OBJECTDIR}/_ext/1979302600/Brewpi.o ../brewpi_avr/Brewpi.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Brewpi.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Brewpi_nomain.o ../brewpi_avr/Brewpi.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Brewpi.o ${OBJECTDIR}/_ext/1979302600/Brewpi_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Commands_nomain.o: ${OBJECTDIR}/_ext/1979302600/Commands.o ../brewpi_avr/Commands.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Commands.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Commands_nomain.o ../brewpi_avr/Commands.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Commands.o ${OBJECTDIR}/_ext/1979302600/Commands_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Comms_nomain.o: ${OBJECTDIR}/_ext/1979302600/Comms.o ../brewpi_avr/Comms.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Comms.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Comms_nomain.o ../brewpi_avr/Comms.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Comms.o ${OBJECTDIR}/_ext/1979302600/Comms_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Display_nomain.o: ${OBJECTDIR}/_ext/1979302600/Display.o ../brewpi_avr/Display.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Display.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Display_nomain.o ../brewpi_avr/Display.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Display.o ${OBJECTDIR}/_ext/1979302600/Display_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/FilterCascaded_nomain.o: ${OBJECTDIR}/_ext/1979302600/FilterCascaded.o ../brewpi_avr/FilterCascaded.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/FilterCascaded.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/FilterCascaded_nomain.o ../brewpi_avr/FilterCascaded.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/FilterCascaded.o ${OBJECTDIR}/_ext/1979302600/FilterCascaded_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/FilterFixed_nomain.o: ${OBJECTDIR}/_ext/1979302600/FilterFixed.o ../brewpi_avr/FilterFixed.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/FilterFixed.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/FilterFixed_nomain.o ../brewpi_avr/FilterFixed.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/FilterFixed.o ${OBJECTDIR}/_ext/1979302600/FilterFixed_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/GenericContainer_nomain.o: ${OBJECTDIR}/_ext/1979302600/GenericContainer.o ../brewpi_avr/GenericContainer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/GenericContainer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/GenericContainer_nomain.o ../brewpi_avr/GenericContainer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/GenericContainer.o ${OBJECTDIR}/_ext/1979302600/GenericContainer_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Memops_nomain.o: ${OBJECTDIR}/_ext/1979302600/Memops.o ../brewpi_avr/Memops.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Memops.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Memops_nomain.o ../brewpi_avr/Memops.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Memops.o ${OBJECTDIR}/_ext/1979302600/Memops_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/NullLcdDriver_nomain.o: ${OBJECTDIR}/_ext/1979302600/NullLcdDriver.o ../brewpi_avr/NullLcdDriver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/NullLcdDriver.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/NullLcdDriver_nomain.o ../brewpi_avr/NullLcdDriver.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/NullLcdDriver.o ${OBJECTDIR}/_ext/1979302600/NullLcdDriver_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Profile_nomain.o: ${OBJECTDIR}/_ext/1979302600/Profile.o ../brewpi_avr/Profile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Profile.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Profile_nomain.o ../brewpi_avr/Profile.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Profile.o ${OBJECTDIR}/_ext/1979302600/Profile_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Sensor_nomain.o: ${OBJECTDIR}/_ext/1979302600/Sensor.o ../brewpi_avr/Sensor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Sensor.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Sensor_nomain.o ../brewpi_avr/Sensor.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Sensor.o ${OBJECTDIR}/_ext/1979302600/Sensor_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/SystemProfile_nomain.o: ${OBJECTDIR}/_ext/1979302600/SystemProfile.o ../brewpi_avr/SystemProfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/SystemProfile.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/SystemProfile_nomain.o ../brewpi_avr/SystemProfile.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/SystemProfile.o ${OBJECTDIR}/_ext/1979302600/SystemProfile_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Temperature_nomain.o: ${OBJECTDIR}/_ext/1979302600/Temperature.o ../brewpi_avr/Temperature.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Temperature.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Temperature_nomain.o ../brewpi_avr/Temperature.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Temperature.o ${OBJECTDIR}/_ext/1979302600/Temperature_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Ticks_nomain.o: ${OBJECTDIR}/_ext/1979302600/Ticks.o ../brewpi_avr/Ticks.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Ticks.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Ticks_nomain.o ../brewpi_avr/Ticks.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Ticks.o ${OBJECTDIR}/_ext/1979302600/Ticks_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/Values_nomain.o: ${OBJECTDIR}/_ext/1979302600/Values.o ../brewpi_avr/Values.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/Values.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/Values_nomain.o ../brewpi_avr/Values.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/Values.o ${OBJECTDIR}/_ext/1979302600/Values_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979302600/ValuesProgmem_nomain.o: ${OBJECTDIR}/_ext/1979302600/ValuesProgmem.o ../brewpi_avr/ValuesProgmem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979302600
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979302600/ValuesProgmem.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979302600/ValuesProgmem_nomain.o ../brewpi_avr/ValuesProgmem.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979302600/ValuesProgmem.o ${OBJECTDIR}/_ext/1979302600/ValuesProgmem_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979304334/Main_nomain.o: ${OBJECTDIR}/_ext/1979304334/Main.o ../brewpi_cpp/Main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979304334
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979304334/Main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979304334/Main_nomain.o ../brewpi_cpp/Main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979304334/Main.o ${OBJECTDIR}/_ext/1979304334/Main_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979304334/Print_nomain.o: ${OBJECTDIR}/_ext/1979304334/Print.o ../brewpi_cpp/Print.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979304334
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979304334/Print.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979304334/Print_nomain.o ../brewpi_cpp/Print.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979304334/Print.o ${OBJECTDIR}/_ext/1979304334/Print_nomain.o;\
	fi

${OBJECTDIR}/_ext/1979304334/timems_nomain.o: ${OBJECTDIR}/_ext/1979304334/timems.o ../brewpi_cpp/timems.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1979304334
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/1979304334/timems.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1979304334/timems_nomain.o ../brewpi_cpp/timems.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/1979304334/timems.o ${OBJECTDIR}/_ext/1979304334/timems_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/brewpi_lib || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbrewpi_lib.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
