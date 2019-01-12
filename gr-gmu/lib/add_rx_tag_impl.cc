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
#include "add_rx_tag_impl.h"

namespace gr {
  namespace gmu {

    add_rx_tag::sptr
    add_rx_tag::make(std::string tag_name,int fft_len, const std::string& redis_host)
    {
      return gnuradio::get_initial_sptr
        (new add_rx_tag_impl(tag_name ,fft_len, redis_host));
    }

    /*
     * The private constructor
     */
    add_rx_tag_impl::add_rx_tag_impl(std::string tag_name,int fft_len, const std::string& redis_host)
      : gr::sync_block("add_rx_tag",
              gr::io_signature::make(1, 1, fft_len * sizeof(gr_complex)),
              gr::io_signature::make(1,1, fft_len * sizeof(gr_complex))),
              z(2)
    {
 	   std::cout << "using add tag ... " << std::endl;
            if(!rdx_sub_t.connect(redis_host,6379))
            {
                std::cout<<"Error"<<std::endl;
            }
	          t_name=tag_name;
            fft_len1=fft_len;
            rdx_sub_t.subscribe("C2Ch",got_message,subscribed,unsubscribed);

}

    /*
     * Our virtual destructor.
     */
    add_rx_tag_impl::~add_rx_tag_impl()
    {
    }

    int
    add_rx_tag_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      //fft_len=64;
      for (int i=0;i<noutput_items*fft_len1;i++)
      {
        out[i]=in[i];
      }

//        std::cout << "noutput_items in add_rx_tag_impl: " << noutput_items << std::endl;

      //memcpy(output_items[0], input_items[0],noutput_items *fft_len* sizeof(gr_complex));
        //std::cout << "xsync_2: " << x_sync2 << std::endl;
    	if(x_sync2.substr(0,11)=="RxModChange")
            {
                int x=stoi(x_sync2.substr(12,13));
                std::cout<<"(RxModChange) Rx mod adding/changing tag ==> "<< t_name << "=" << x <<std::endl;
                 add_item_tag(0, // Port number
                  nitems_written(0), // Offset
                   pmt::string_to_symbol(t_name), // Key
                   pmt::from_uint64(x) // Value
                 );
                 z = x;
                 channel_2.clear();
                 x_sync2.clear();
            } else {
            add_item_tag(0, // Port number
                         nitems_written(0)+noutput_items, // Offset
                         pmt::string_to_symbol(t_name), // Key
                         pmt::from_uint64(z) // Value
            );
        }

	return noutput_items;
    }




  } /* namespace gmu */
} /* namespace gr */

