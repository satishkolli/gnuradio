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
SET(EXT_LIBRARY3 /usr/lib/x86_64-linux-gnu/libmysqlclient.so)
SET(INCL_DIR /usr/include/mysql)

list(APPEND redis3_sources
    redis_source_sync_impl.cc
    redis_sink_v_impl.cc
    redis_source_sync1_impl.cc
    redis_sink_sync_impl.cc
    redis2_impl.cc
)

set(redis3_sources "${redis3_sources}" PARENT_SCOPE)
set(CMAKE_CXX_FLAGS "-std=c++11")
if(NOT redis3_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT redis3_sources)

add_library(gnuradio-redis3 SHARED ${redis3_sources})
target_link_libraries(gnuradio-redis3 ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${EXT_LIBRARY} ${EXT_LIBRARY1} ${EXT_LIBRARY2} ${EXT_LIBRARY3})
set_target_properties(gnuradio-redis3 PROPERTIES DEFINE_SYMBOL "gnuradio_redis3_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-redis3 PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-redis3 RUNTIME_COMPONENT "redis3_runtime" DEVEL_COMPONENT "redis3_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS} ${INCL_DIR})

list(APPEND test_redis3_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_redis3.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_redis3.cc
)

add_executable(test-redis3 ${test_redis3_sources} ${INCL_DIR}/mysql.h)

target_link_libraries(
  test-redis3
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-redis3
)

GR_ADD_TEST(test_redis3 test-redis3)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

