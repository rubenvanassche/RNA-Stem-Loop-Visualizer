# This file is a part of the RNA Stem Loop Visualizer software.
#
# Copyright (C) 2013
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
# Last modified: 24 December 2013.
# By: Pieter Lauwers.
cmake_minimum_required(VERSION 2.6)
project("RNA Stem Loop Visualizer")

# Some OS X changes
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   SET(OperatingSystem "Mac OS X")
   SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# define DATADIR for tests that need input files
add_definitions(-DDATADIR="${CMAKE_SOURCE_DIR}/data/")

# Set flags
set(CMAKE_CXX_FLAGS "-std=c++11 -g -pedantic -Wall -Wextra")

# Lists TinyXML related files (no main)
set(TINYXMLSRC
    src/TinyXML/tinyxml.cpp 
    src/TinyXML/tinyxmlparser.cpp 
    src/TinyXML/tinyxmlerror.cpp 
    src/TinyXML/tinystr.cpp
    )

# Lists Turing related files (no main)
set(TURINGSRC
    src/Turing.cpp
    )

# Lists CNF related files (no main)
set(CNFSRC
    src/CFG.cpp
    src/CNF.cpp
    )

# Lists PDA related files (no main)
set(PDASRC
    src/PDA.cpp
    )

# Lists LLParser related files (no main)
set(LLPARSERSRC
    src/LLParser.cpp
    src/LLParserInputOutput.cpp
    )

# Lists all test files (no main)
set(TESTSRC
    src/test_CFG.cpp
    src/test_CNF.cpp
    src/test_Turing.cpp
    src/test_PDA.cpp
    src/test_LLParser.cpp   
    )

# build the main program
add_executable(RNA-Stem-Loop-Visualizer src/RNAslv.cpp)

# build all the tests
add_executable(Tests src/Tests.cpp ${TINYXMLSRC} ${TURINGSRC} ${CNFSRC} ${PDASRC} ${LLPARSERSRC} ${TESTSRC})

# build the Turing workshop
add_executable(RunTuring src/runTuringInput.cpp ${TINYXMLSRC} ${TURINGSRC})

# build the CYK workshop
add_executable(RunCYK src/runCYK.cpp ${TINYXMLSRC} ${CNFSRC})

# build the PDA workshop
add_executable(RunPDA src/runPDAInput.cpp ${TINYXMLSRC} ${PDASRC})

# build the LLParser workshop
add_executable(RunLLParser src/runLLParserInput.cpp ${LLPARSERSRC})

# Install all executables into the bin directory.
install(TARGETS 
    RNA-Stem-Loop-Visualizer 
    Tests 
    RunTuring 
    RunCYK 
    RunPDA 
    RunLLParser
    DESTINATION ${PROJECT_SOURCE_DIR}/bin
    )
