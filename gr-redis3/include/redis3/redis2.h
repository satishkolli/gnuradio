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


#ifndef INCLUDED_REDIS3_REDIS2_H
#define INCLUDED_REDIS3_REDIS2_H

#include <redis3/api.h>
#include <gnuradio/tagged_stream_block.h>
#include <gnuradio/digital/api.h>



namespace gr {
  namespace redis3 {

    /*!
     * \brief <+description of block+>
     * \ingroup redis3
     *
     */
    class REDIS3_API redis2 : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<redis2> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of redis3::redis2.
       *
       * To avoid accidental use of raw pointers, redis3::redis2's
       * constructor is in a private implementation
       * class. redis3::redis2::make is the public interface for
       * creating new instances.
       */
      static sptr make(int msgsize, const char *channelname, const std::string& len_tag_key, const std::string& redis_host);
    };

  } // namespace redis3
} // namespace gr

#endif /* INCLUDED_REDIS3_REDIS2_H */

