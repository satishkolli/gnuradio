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

#ifndef INCLUDED_GMU_CHUNKS_TO_SYMBOLS_GMU_CHUNKS_TO_SYMBOLS_IMPL_H
#define INCLUDED_GMU_CHUNKS_TO_SYMBOLS_GMU_CHUNKS_TO_SYMBOLS_IMPL_H

#include <gmu_chunks_to_symbols/gmu_chunks_to_symbols.h>

namespace gr {
  namespace gmu_chunks_to_symbols {

    class gmu_chunks_to_symbols_impl : public gmu_chunks_to_symbols
    {
     private:
      int d_D;
      std::vector<gr_complex> d_symbol_table;

     public:
      gmu_chunks_to_symbols_impl(const std::vector<gr_complex> &symbol_table, const int D=1);
      ~gmu_chunks_to_symbols_impl();

      void set_vector_from_pmt(std::vector<float> &symbol_table, pmt::pmt_t &symbol_table_pmt);
      void set_vector_from_pmt(std::vector<gr_complex> &symbol_table, pmt::pmt_t &symbol_table_pmt);

      void handle_set_symbol_table(pmt::pmt_t symbol_table_pmt);
      void set_symbol_table(const std::vector<gr_complex> &symbol_table);
      
      int D() const { return d_D; }
      std::vector<gr_complex> symbol_table() const { return d_symbol_table; }

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

      bool check_topology(int ninputs, int noutputs) { return ninputs == noutputs; }

    };

  } // namespace gmu_chunks_to_symbols
} // namespace gr

#endif /* INCLUDED_GMU_CHUNKS_TO_SYMBOLS_GMU_CHUNKS_TO_SYMBOLS_IMPL_H */

