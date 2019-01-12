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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "redis2_impl.h"
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <queue>

namespace gr {
  namespace redis3 {
    std::queue < std::string > redis_msgs;
    std::queue < std::string > c2ch_msgs;
    auto got_message = [](const std::string& topic, const std::string& msg) {
        //std::cout << "Redis - CH: " << topic << ", MSG: " << msg << std::endl;
        if (topic == "C2Ch")
        {
            c2ch_msgs.push(msg);
        } else
        {
            redis_msgs.push(msg);
        }
    };

    auto subscribed = [](const std::string& topic) {
        std::cout << "Subscribed to " << topic << std::endl;
    };

    auto unsubscribed = [](const std::string& topic) {
        std::cout << "Un subscribed from " << topic << std::endl;
    };


    redis2::sptr
    redis2::make(int msgsize, const char *channelname, const std::string& len_tag_key, const std::string& redis_host)
    {
      return gnuradio::get_initial_sptr
        (new redis2_impl(msgsize, channelname, len_tag_key, redis_host));
    }

    /*
     * The private constructor
     */
    redis2_impl::redis2_impl(int msgsize, const char *channelname, const std::string& len_tag_key, const std::string& redis_host)
      : gr::tagged_stream_block("redis2",
                                 gr::io_signature::make(0, 0, 0),
                                 gr::io_signature::make(1, 1, sizeof (char)),
                                 len_tag_key
                                 ),
      d_redis_host(redis_host),
      d_packet_len(160),
      d_packet_len_pmt(pmt::from_long(160)),
      d_len_tag_key(pmt::string_to_symbol(len_tag_key)),
      d_next_tag_pos(0),
        z(2)
    {
      std::cout << "using stream based redis block ... " << std::endl;
            channelName=channelname;
            size_t1=msgsize;
            std::cout << "Connecting to redis on host: " << d_redis_host << std::endl;
            if(!rdx_sub_t.connect(d_redis_host,6379))
            {
                std::cout<<"Error"<<std::endl;
            }
            rdx_sub_t.subscribe(channelname,got_message,subscribed,unsubscribed);
            rdx_sub_t.subscribe("C2Ch",got_message,subscribed,unsubscribed);

            oldtime = high_resolution_clock::now();
            count = 1;
	   
    }

    /*
     * Our virtual destructor.
     */
    redis2_impl::~redis2_impl()
    {
    }

    int
    redis2_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      return size_t1;
    }

    int
    redis2_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
             char *out = (char *) output_items[0];
             int bufsize = 0;
             //set_output_multiple(size_t1);
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
            if(!redis_msgs.empty()) {
//            if(x_sync2.length()>0 && channel_2==channelName) {
                std::string m = redis_msgs.front();//redis_msgs.top();
//                std::cout << channelName << '|' << m <<std::endl;
                for (k = 0; k < size_t1; k++) {
                    out[k] = m.at(k);
                }
                redis_msgs.pop();
            }else {
                for (k = 0; k < size_t1; k++) {
                    out[k] = x2.at(k);
                }
            }

            if (!c2ch_msgs.empty()) {
                std::string m = c2ch_msgs.front();
                if(m.substr(0,9)=="ModChange")
                {
                    int x=stoi(m.substr(10,11));
                    //int time=stoi(x_sync2.substr(12,22));
                    std::cout<<"Redis adding tag:"<<x<<std::endl;
                    add_item_tag(0, // Port number
                                 nitems_written(0)+160, // Offset
                                 pmt::string_to_symbol("change_mod"), // Key
                                 pmt::from_uint64(x) // Value
                    );
                    c2ch_msgs.pop();
                    z = x;
                } else {
                    add_item_tag(0, // Port number
                                 nitems_written(0)+160, // Offset
                                 pmt::string_to_symbol("change_mod"), // Key
                                 pmt::from_uint64(z) // Value
                    );
                }
            }

            return size_t1;
        }

  } /* namespace redis3 */
} /* namespace gr */

