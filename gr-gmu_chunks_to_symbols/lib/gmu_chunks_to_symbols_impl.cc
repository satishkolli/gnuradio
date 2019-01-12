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
#include "gmu_chunks_to_symbols_impl.h"
#include <gnuradio/tag_checker.h>
#include <assert.h>

#define SQRT_TWO 0.707107

namespace gr {
  namespace gmu_chunks_to_symbols {

    gmu_chunks_to_symbols::sptr
    gmu_chunks_to_symbols::make(const std::vector<gr_complex> &symbol_table, const int D)
    {
      return gnuradio::get_initial_sptr
        (new gmu_chunks_to_symbols_impl(symbol_table, D));
    }

    /*
     * The private constructor
     */
    gmu_chunks_to_symbols_impl::gmu_chunks_to_symbols_impl(const std::vector<gr_complex> &symbol_table, const int D)
      : gr::sync_interpolator("gmu_chunks_to_symbols",
              gr::io_signature::make(1, -1, sizeof(unsigned char)),
			   gr::io_signature::make(1, -1, sizeof(gr_complex)),
			   D),
      d_D(D), d_symbol_table(symbol_table)
    {
      message_port_register_in(pmt::mp("set_symbol_table"));
      set_msg_handler(
        pmt::mp("set_symbol_table"),
        boost::bind(&gmu_chunks_to_symbols_impl::handle_set_symbol_table,
                    this, _1));
    }
    /*
     * Our virtual destructor.
     */
    gmu_chunks_to_symbols_impl::~gmu_chunks_to_symbols_impl()
    {
    }

 void
    gmu_chunks_to_symbols_impl::set_vector_from_pmt(std::vector<gr_complex> &symbol_table, pmt::pmt_t &symbol_table_pmt) {
      symbol_table.resize(0);
      for (unsigned int i=0; i<pmt::length(symbol_table_pmt); i++) {
        symbol_table.push_back(pmt::c32vector_ref(symbol_table_pmt, i));
      }      
    }

    void
    gmu_chunks_to_symbols_impl::set_vector_from_pmt(std::vector<float> &symbol_table, pmt::pmt_t &symbol_table_pmt) {
      symbol_table.resize(0);
      for (unsigned int i=0; i<pmt::length(symbol_table_pmt); i++) {
        float f = pmt::f32vector_ref(symbol_table_pmt, i);
        symbol_table.push_back(f);
      }
    }
    
    void
    gmu_chunks_to_symbols_impl::handle_set_symbol_table(pmt::pmt_t symbol_table_pmt)
    {
      std::vector<gr_complex> symbol_table;
      set_vector_from_pmt(symbol_table, symbol_table_pmt);
      set_symbol_table(symbol_table);
    }

    
    void
    gmu_chunks_to_symbols_impl::set_symbol_table(const std::vector<gr_complex> &symbol_table)
    {
      d_symbol_table.resize(0);
      for (unsigned int i=0; i<symbol_table.size(); i++) {
        d_symbol_table.push_back(symbol_table[i]);
      }
    }

    int
    gmu_chunks_to_symbols_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      assert(noutput_items % d_D == 0);
      assert(input_items.size() == output_items.size());
      int nstreams = input_items.size();

//        std::vector<tag_t> tags;
//        get_tags_in_window(tags, 0, 0, noutput_items, pmt::mp("change_mod"));
//
//      if(!tags.empty()) {
//            std::cout << "CHANGING MODULATION TO 2 IN CHUNKS TO SYMBOLS -----> " << std::endl;
//            std::vector<gr_complex> d_constellation;
//          d_constellation[0] = gr_complex(-SQRT_TWO, -SQRT_TWO);
//          d_constellation[1] = gr_complex(SQRT_TWO, -SQRT_TWO);
//          d_constellation[2] = gr_complex(-SQRT_TWO, SQRT_TWO);
//          d_constellation[3] = gr_complex(SQRT_TWO, SQRT_TWO);
//
//            set_symbol_table(d_constellation);
//        }

        for(int m = 0; m < nstreams; m++) {
        const unsigned char *in = (unsigned char*)input_items[m];
        gr_complex *out = (gr_complex*)output_items[m];

        std::vector<tag_t> tags;
        get_tags_in_range(tags, m, nitems_read(m), nitems_read(m)+noutput_items/d_D);
        tag_checker tchecker(tags);

        // per stream processing
        for(int i = 0; i < noutput_items / d_D; i++) {
          
          std::vector<tag_t> tags_now;
          tchecker.get_tags(tags_now, i+nitems_read(m));
          for (unsigned int j=0; j<tags_now.size(); j++) {
            tag_t tag = tags_now[j];
            dispatch_msg(tag.key, tag.value);
          }
          assert(((unsigned int)in[i]*d_D+d_D) <= d_symbol_table.size());
          memcpy(out, &d_symbol_table[(unsigned int)in[i]*d_D], d_D*sizeof(gr_complex));
          out+=d_D;
        }
      }
      return noutput_items;
    }

  } /* namespace gmu_chunks_to_symbols */
} /* namespace gr */

