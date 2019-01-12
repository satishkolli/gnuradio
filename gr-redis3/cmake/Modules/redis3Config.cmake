INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_REDIS3 redis3)

FIND_PATH(
    REDIS3_INCLUDE_DIRS
    NAMES redis3/api.h
    HINTS $ENV{REDIS3_DIR}/include
        ${PC_REDIS3_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    REDIS3_LIBRARIES
    NAMES gnuradio-redis3
    HINTS $ENV{REDIS3_DIR}/lib
        ${PC_REDIS3_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(REDIS3 DEFAULT_MSG REDIS3_LIBRARIES REDIS3_INCLUDE_DIRS)
MARK_AS_ADVANCED(REDIS3_LIBRARIES REDIS3_INCLUDE_DIRS)

