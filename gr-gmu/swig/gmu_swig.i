/* -*- c++ -*- */

#define GMU_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gmu_swig_doc.i"

%{
#include "gmu/add_rx_tag.h"
%}


%include "gmu/add_rx_tag.h"
GR_SWIG_BLOCK_MAGIC2(gmu, add_rx_tag);
