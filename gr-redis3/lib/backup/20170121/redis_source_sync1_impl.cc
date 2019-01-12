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
#include "redis_source_sync1_impl.h"
#include <sstream>
#include <string.h>
#include <redox.hpp>
#include <chrono>
#include <gnuradio/math.h>
#include <gnuradio/gr_complex.h>
using namespace std::chrono;

namespace gr {
    namespace redis3 {

        std::string x_sync1;
        std::string channel;
        auto got_message = [](const std::string& topic, const std::string& msg) {
            x_sync1=msg;
            channel=topic;
        };

        auto subscribed = [](const std::string& topic) {
            std::cout << "Subscribed to " << topic << std::endl;
        };

        auto unsubscribed = [](const std::string& topic) {
            std::cout << "Un subscribed from " << topic << std::endl;
        };
        redis_source_sync1::sptr
        redis_source_sync1::make(int msgsize,const char *channelname)
        {
            return gnuradio::get_initial_sptr
                    (new redis_source_sync1_impl(msgsize, channelname));
        }

        /*
         * The private constructor
         */
        redis_source_sync1_impl::redis_source_sync1_impl(int msgsize,const char *channelname)
                : gr::sync_block("redis_source_sync1",
                                 gr::io_signature::make(0, 0, 0),
                                 gr::io_signature::make(1, 1, sizeof(char)))
        {
            channelName=channelname;
            size_t1=msgsize;
            if(!rdx_sub_t.connect("localhost",6379))
            {
                std::cout<<"Error"<<std::endl;
            }
            rdx_sub_t.subscribe(channelname,got_message,subscribed,unsubscribed);
            
            oldtime = high_resolution_clock::now();
            count = 1;
        }

        /*
         * Our virtual destructor.
         */
        redis_source_sync1_impl::~redis_source_sync1_impl()
        {
        }

        int
        redis_source_sync1_impl::work(int noutput_items,
                                      gr_vector_const_void_star &input_items,
                                      gr_vector_void_star &output_items)
        {
             char *out = (char *) output_items[0];
             int bufsize = 0;
             set_output_multiple(size_t1);
             //out = (char *) output_items[0];
             std::string x2;
             auto currentTime = high_resolution_clock::now();
            //std::cout << "counter : " << count << std::endl;
            long difference = duration_cast<std::chrono::nanoseconds>(currentTime - oldtime).count();
            x2 = "yyyyyyyy";
            x2+=channelName;
            x2 += "|";
            std::ostringstream oss;
            oss << count;
            x2 += oss.str();
            x2 += "|";
            std::string time;
            long ms = duration_cast<milliseconds>(
                    system_clock::now().time_since_epoch()
            ).count();
            time = std::to_string(ms);

            x2 += time;
            x2 += "extra";
            while (x2.length() < size_t1)
                x2 += "x";
            oldtime = currentTime;
            count++;
            if(x_sync1.length()>0 && channel==channelName) {
                for (k = 0; k < size_t1; k++) {
                    out[k] = x_sync1.at(k);
                }
                std::cout<<channelName<<'|'<<x_sync1<<std::endl;
                x_sync1.clear();
            }else {
                for (k = 0; k < size_t1; k++) {
                    out[k] = x2.at(k);
                }
            }
            /*add_item_tag(0, // Port number
            nitems_written(0)+count*160, // Offset
            pmt::mp("packet_len"), // Key
            pmt::from_double(size_t1) // Value
            );*/
            //consume_each(size_t1);
            // Tell runtime system how many output items we produced.
            return size_t1;
        }

    } /* namespace redis3 */
} /* namespace gr */
