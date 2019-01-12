/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "redis_sink_sync_impl.h"
#include <redox.hpp>

namespace gr {
  namespace redis3 {

    redis_sink_sync::sptr
    redis_sink_sync::make(int msgsize, const char *channel, const std::string& redis_host)
    {
      return gnuradio::get_initial_sptr
        (new redis_sink_sync_impl(msgsize, channel, redis_host));
    }

    /*
     * The private constructor
     */
    redis_sink_sync_impl::redis_sink_sync_impl(int msgsize, const char *channel, const std::string& redis_host)
      : gr::sync_block("redis_sink_sync",
              gr::io_signature::make(1, 1, sizeof(char)*msgsize),
              gr::io_signature::make(0, 0, 0))
    {
      msize=msgsize;
      channelName=channel;
      cnt=0;
      if(!rdx_pub.connect(redis_host,6379))
  {
      std::cout<<"Error: "<<std::endl;
  }


  }

    /*
     * Our virtual destructor.
     */
    redis_sink_sync_impl::~redis_sink_sync_impl()
    {
    }

    int
    redis_sink_sync_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      char *in = (char *) input_items[0];
        std::string str=std::string(in);
        if(str.length()>0)
        {
            cnt++;
            if(str.substr(0,8)!="yyyyyyyy")
            {
                std::cout << channelName << '|' << std::to_string(cnt) << " : " <<str.substr(0,msize)<<std::endl;
                rdx_pub.publish(channelName,str.substr(0,msize));
                noutput_items=1;
            }
            str.clear();
        }
      return noutput_items;
    }

  } /* namespace redis3 */
} /* namespace gr */

