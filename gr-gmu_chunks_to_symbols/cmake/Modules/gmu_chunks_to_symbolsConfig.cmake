INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_GMU_CHUNKS_TO_SYMBOLS gmu_chunks_to_symbols)

FIND_PATH(
    GMU_CHUNKS_TO_SYMBOLS_INCLUDE_DIRS
    NAMES gmu_chunks_to_symbols/api.h
    HINTS $ENV{GMU_CHUNKS_TO_SYMBOLS_DIR}/include
        ${PC_GMU_CHUNKS_TO_SYMBOLS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GMU_CHUNKS_TO_SYMBOLS_LIBRARIES
    NAMES gnuradio-gmu_chunks_to_symbols
    HINTS $ENV{GMU_CHUNKS_TO_SYMBOLS_DIR}/lib
        ${PC_GMU_CHUNKS_TO_SYMBOLS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMU_CHUNKS_TO_SYMBOLS DEFAULT_MSG GMU_CHUNKS_TO_SYMBOLS_LIBRARIES GMU_CHUNKS_TO_SYMBOLS_INCLUDE_DIRS)
MARK_AS_ADVANCED(GMU_CHUNKS_TO_SYMBOLS_LIBRARIES GMU_CHUNKS_TO_SYMBOLS_INCLUDE_DIRS)

