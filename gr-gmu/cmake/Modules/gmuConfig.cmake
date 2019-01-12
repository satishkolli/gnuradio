INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_GMU gmu)

FIND_PATH(
    GMU_INCLUDE_DIRS
    NAMES gmu/api.h
    HINTS $ENV{GMU_DIR}/include
        ${PC_GMU_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GMU_LIBRARIES
    NAMES gnuradio-gmu
    HINTS $ENV{GMU_DIR}/lib
        ${PC_GMU_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMU DEFAULT_MSG GMU_LIBRARIES GMU_INCLUDE_DIRS)
MARK_AS_ADVANCED(GMU_LIBRARIES GMU_INCLUDE_DIRS)

