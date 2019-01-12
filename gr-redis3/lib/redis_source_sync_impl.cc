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
#include "redis_source_sync_impl.h"
#include <sstream>
#include <string.h>
#include <redox.hpp>
#include <chrono>

using namespace std::chrono;
namespace gr {
    namespace redis3 {

        std::string x_sync;

        auto got_message = [](const std::string &topic, const std::string &msg) {
            x_sync = msg;
        };

        auto subscribed = [](const std::string &topic) {
            std::cout << "Subscribed to " << topic << std::endl;
        };

        auto unsubscribed = [](const std::string &topic) {
            std::cout << "Un subscribed from " << topic << std::endl;
        };

        redis_source_sync::sptr
        redis_source_sync::make(int msgsize) {
            return gnuradio::get_initial_sptr
                    (new redis_source_sync_impl(msgsize));
        }

        /*
         * The private constructor
         */
        redis_source_sync_impl::redis_source_sync_impl(int msgsize)
                : gr::sync_block("redis_source_sync",
                                 gr::io_signature::make(0, 0, 0),
                                 gr::io_signature::make(1, 1, sizeof(char))) {
            size_t1 = msgsize;
            if (!rdx_sub_t.connect("localhost", 6379)) {
                std::cout << "Error";
            }
            rdx_sub_t.subscribe("TxCh", got_message, subscribed, unsubscribed);
            oldtime = high_resolution_clock::now();
            count = 1;
        }

        /*
         * Our virtual destructor.
         */
        redis_source_sync_impl::~redis_source_sync_impl() {
        }

        int
        redis_source_sync_impl::work(int noutput_items,
                                     gr_vector_const_void_star &input_items,
                                     gr_vector_void_star &output_items) {
            //std::cout << "noutput_items: " << noutput_items << std::endl;
            char *out = (char *) output_items[0];
            int bufsize = 0;
            set_output_multiple(size_t1);
            out = (char *) output_items[0];
            std::string x2;
            auto currentTime = high_resolution_clock::now();

            //std::cout << "counter : " << count << std::endl;

            long difference = duration_cast<std::chrono::nanoseconds>(currentTime - oldtime).count();

            x2 = "yyyyyyyy";
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
            if(x_sync.length()>0) {
                for (k = 0; k < size_t1; k++) {
                    out[k] = x_sync.at(k);
                }
                x_sync.clear();
                add_item_tag(0, // Port number
                             nitems_written(0), // Offset
                             pmt::mp("test_tag"), // Key
                             pmt::from_double(std::abs(1000.00)) // Value
                );
               // std::cout <<"In Redis: "<<out <<std::endl;
            }else {
                for (k = 0; k < size_t1; k++) {
                    out[k] = x2.at(k);
                }
                add_item_tag(0, // Port number
                             nitems_written(0), // Offset
                             pmt::mp("test_tag"), // Key
                             pmt::from_double(std::abs(2000.00)) // Value
                );
            }
            //consume_each(size_t1);
            // Tell runtime system how many output items we produced.
            return size_t1;
            // Do <+signal processing+>

            // Tell runtime system how many output items we produced.
            //return noutput_items;
        }

    } /* namespace redis3 */
} /* namespace gr */

