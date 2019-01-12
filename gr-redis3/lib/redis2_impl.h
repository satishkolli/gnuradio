/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_REDIS3_REDIS2_IMPL_H
#define INCLUDED_REDIS3_REDIS2_IMPL_H

#include <redis3/redis2.h>
#include <redox.hpp>
#include <chrono>

using namespace std::chrono;

namespace gr {
  namespace redis3 {

    class redis2_impl : public redis2
    {
     private:
      const char *channelName;
            high_resolution_clock::time_point oldtime;
            long count=0;
            int k;
            int j_t=0;
            int cnt_t=0;
            int size_t1;
            int samplerate_t;
            int flag=0;
            redox::Subscriber rdx_sub_t;
            std::string d_redis_host;

            unsigned d_packet_len;
            pmt::pmt_t d_packet_len_pmt;
            pmt::pmt_t d_len_tag_key;
            uint64_t d_next_tag_pos;
	    long time1;
            int z;

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      redis2_impl(int msgsize, const char *channelname, const std::string& len_tag_key, const std::string& redis_host);
      ~redis2_impl();

      // Where all the action really happens
      int work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace redis3
} // namespace gr

#endif /* INCLUDED_REDIS3_REDIS2_IMPL_H */

