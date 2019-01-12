# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

SET(EXT_LIBRARY /usr/lib/x86_64-linux-gnu/libhiredis.so)
SET(EXT_LIBRARY1 /usr/local/lib64/libredox.so)
SET(EXT_LIBRARY2 /usr/lib/x86_64-linux-gnu/libev.so)

list(APPEND gmu_sources
    add_rx_tag_impl.cc
)

set(gmu_sources "${gmu_sources}" PARENT_SCOPE)
set(CMAKE_CXX_FLAGS "-std=c++11")
if(NOT gmu_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT gmu_sources)

add_library(gnuradio-gmu SHARED ${gmu_sources})
target_link_libraries(gnuradio-gmu ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${EXT_LIBRARY} ${EXT_LIBRARY1} ${EXT_LIBRARY2})
set_target_properties(gnuradio-gmu PROPERTIES DEFINE_SYMBOL "gnuradio_gmu_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-gmu PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-gmu RUNTIME_COMPONENT "gmu_runtime" DEVEL_COMPONENT "gmu_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_gmu_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_gmu.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_gmu.cc
)

add_executable(test-gmu ${test_gmu_sources})

target_link_libraries(
  test-gmu
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-gmu
)

GR_ADD_TEST(test_gmu test-gmu)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

