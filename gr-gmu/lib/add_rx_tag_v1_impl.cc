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
#include "add_rx_tag_v1_impl.h"

namespace gr {
  namespace gmu {

    add_rx_tag_v1::sptr
    add_rx_tag_v1::make(std::string tag_name,int fft_len)
    {
      return gnuradio::get_initial_sptr
        (new add_rx_tag_v1_impl(tag_name, fft_len));
    }

    /*
     * The private constructor
     */
    add_rx_tag_v1_impl::add_rx_tag_v1_impl(std::string tag_name,int fft_len)
      : gr::sync_block("add_rx_tag_v",
              gr::io_signature::make(1, 1, fft_len*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, fft_len*sizeof(gr_complex)))
    {
	    std::cout << "using add tag vector... " << std::endl;
            if(!rdx_sub_t.connect("localhost",6379))
            {
                std::cout<<"Error"<<std::endl;
            }
	    t_name=tag_name;
	    f_len=fft_len;
            rdx_sub_t.subscribe("C2Ch",got_message,subscribed,unsubscribed);
     }

    /*
     * Our virtual destructor.
     */
    add_rx_tag_v1_impl::~add_rx_tag_v1_impl()
    {
    }

    int
    add_rx_tag_v1_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
     	const gr_complex *in = (const gr_complex *) input_items[0];
      	gr_complex *out = (gr_complex *) output_items[0];
      /*emcpy(output_items[0], input_items[0],noutput_items * f_len*sizeof(gr_complex));
    	if(x_sync.substr(0,11)=="RxModChange")
            {
                int x=stoi(x_sync.substr(12,13));
                std::cout<<"Rx mod adding tag:"<<x<<std::endl;
                 add_item_tag(0, // Port number
                  nitems_written(0), // Offset
                   pmt::string_to_symbol(t_name), // Key
                   pmt::from_uint64(x) // Value
                 );
                 channel1.clear();
                 x_sync.clear();
            }*/

	return noutput_items;
   }

  } /* namespace gmu */
} /* namespace gr */

