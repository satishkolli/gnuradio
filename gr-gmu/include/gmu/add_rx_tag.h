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


#ifndef INCLUDED_GMU_ADD_RX_TAG_H
#define INCLUDED_GMU_ADD_RX_TAG_H

#include <gmu/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace gmu {

    /*!
     * \brief <+description of block+>
     * \ingroup gmu
     *
     */
    class GMU_API add_rx_tag : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<add_rx_tag> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of gmu::add_rx_tag.
       *
       * To avoid accidental use of raw pointers, gmu::add_rx_tag's
       * constructor is in a private implementation
       * class. gmu::add_rx_tag::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string tag_name,int fft_len, const std::string& redis_host);
    };

  } // namespace gmu
} // namespace gr

#endif /* INCLUDED_GMU_ADD_RX_TAG_H */

