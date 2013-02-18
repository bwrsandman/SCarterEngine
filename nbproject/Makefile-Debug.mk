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
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Gui.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/Scenes/SceneMorph.o \
	${OBJECTDIR}/Scenes/SceneBase.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Wall -std=c++11 `pkg-config --cflags gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2 glew` 
CXXFLAGS=-Wall -std=c++11 `pkg-config --cflags gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2 glew` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/scarter_engine

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/scarter_engine: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} `pkg-config --libs gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2 glew` -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/scarter_engine  ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/Gui.o: Gui.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/Gui.o Gui.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/Scenes/SceneMorph.o: Scenes/SceneMorph.cpp 
	${MKDIR} -p ${OBJECTDIR}/Scenes
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/Scenes/SceneMorph.o Scenes/SceneMorph.cpp

${OBJECTDIR}/Scenes/SceneBase.o: Scenes/SceneBase.cpp 
	${MKDIR} -p ${OBJECTDIR}/Scenes
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/Scenes/SceneBase.o Scenes/SceneBase.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/scarter_engine

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
