/* -*- c++ -*- */

#define GMU_CHUNKS_TO_SYMBOLS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gmu_chunks_to_symbols_swig_doc.i"

%{
#include "gmu_chunks_to_symbols/gmu_chunks_to_symbols.h"
%}


%include "gmu_chunks_to_symbols/gmu_chunks_to_symbols.h"
GR_SWIG_BLOCK_MAGIC2(gmu_chunks_to_symbols, gmu_chunks_to_symbols);
