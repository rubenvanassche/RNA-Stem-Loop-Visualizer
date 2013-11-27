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
# Last modified: 15 November 2013
# By: Stijn Wouters

# Some OS X changes
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   SET(OperatingSystem "Mac OS X")
   SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Lists all sourcefiles (no testfiles and no main)
set(SRC CFG.cpp CNF.cpp Turing.cpp TinyXML/tinyxml.cpp TinyXML/tinyxmlparser.cpp TinyXML/tinyxmlerror.cpp TinyXML/tinystr.cpp)

# Lists all testfiles, DO NOT remove Tests.cpp
set(TESTSRC Tests.cpp test_CFG.cpp test_Turing.cpp)

# Set flags
set( CMAKE_CXX_FLAGS "-Wall -std=c++11 " )

# Target to build all the tests
add_executable(Tests ${TESTSRC} ${SRC})

# Target to build the main program
add_executable(RNAslv RNAslv.cpp ${SRC})

# Install all executables into the bin directory.
install(TARGETS Tests RNAslv DESTINATION ${PROJECT_SOURCE_DIR}/bin)
