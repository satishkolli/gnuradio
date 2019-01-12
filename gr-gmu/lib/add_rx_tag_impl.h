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

#ifndef INCLUDED_GMU_ADD_RX_TAG_IMPL_H
#define INCLUDED_GMU_ADD_RX_TAG_IMPL_H

#include <gmu/add_rx_tag.h>
#include <redox.hpp>
#include <chrono>

namespace gr {
  namespace gmu {

    std::string x_sync2;
    std::string channel_2;
    auto got_message = [](const std::string& topic, const std::string& msg) {
        x_sync2=msg;
        channel_2=topic;
    };

    auto subscribed = [](const std::string& topic) {
        std::cout << "Subscribed to " << topic << std::endl;
    };

    auto unsubscribed = [](const std::string& topic) {
        std::cout << "Un subscribed from " << topic << std::endl;
    };



    class add_rx_tag_impl : public add_rx_tag
    {
     private:
            redox::Subscriber rdx_sub_t;
	      std::string t_name;
        int fft_len1;
        int z;

     public:
      add_rx_tag_impl(std::string tag_name, int fft_len, const std::string& redis_host);
      ~add_rx_tag_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace gmu
} // namespace gr

#endif /* INCLUDED_GMU_ADD_RX_TAG_IMPL_H */

