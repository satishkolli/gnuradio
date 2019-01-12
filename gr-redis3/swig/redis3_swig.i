/* -*- c++ -*- */

#define REDIS3_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "redis3_swig_doc.i"

%{
#include "redis3/redis_source_sync.h"
#include "redis3/redis_sink_v.h"
#include "redis3/redis_source_sync1.h"
#include "redis3/redis_sink_sync.h"
#include "redis3/redis2.h"
%}


%include "redis3/redis_source_sync.h"
GR_SWIG_BLOCK_MAGIC2(redis3, redis_source_sync);
%include "redis3/redis_sink_v.h"
GR_SWIG_BLOCK_MAGIC2(redis3, redis_sink_v);
%include "redis3/redis_source_sync1.h"
GR_SWIG_BLOCK_MAGIC2(redis3, redis_source_sync1);
%include "redis3/redis_sink_sync.h"
GR_SWIG_BLOCK_MAGIC2(redis3, redis_sink_sync);
%include "redis3/redis2.h"
GR_SWIG_BLOCK_MAGIC2(redis3, redis2);
