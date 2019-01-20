/* -*- c++ -*- */
/* 
 * Copyright 2013 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "crc32_bb_impl.h"

namespace gr {
  namespace digital {

    crc32_bb::sptr
    crc32_bb::make(bool check, const std::string &lengthtagname, bool packed, const std::string &metrics_prefix) {      
      return gnuradio::get_initial_sptr(new crc32_bb_impl(check, lengthtagname, packed,metrics_prefix));
    }

    crc32_bb_impl::crc32_bb_impl(bool check, const std::string &lengthtagname, bool packed, const std::string &metrics_prefix)
        : tagged_stream_block("crc32_bb",
                              io_signature::make(1, 1, sizeof(char)),
                              io_signature::make(1, 1, sizeof(char)),
                              lengthtagname),
          d_check(check), d_packed(packed),
          d_npass(0), d_nfail(0) {
      d_crc_length = 4;
      metric_prefix_g = metrics_prefix;
      if (!d_packed) {
        d_crc_length = 32;
        d_unpacked_crc = new char[d_crc_length];        
      }
      set_tag_propagation_policy(TPP_DONT);
      std::cout << "Metric prefix====>" << metrics_prefix << std::endl;
      metric_registry = cppmetrics::core::MetricRegistry::DEFAULT_REGISTRY();
       cppmetrics::core::ConsoleReporter console_reporter(metric_registry, std::cout);
       console_reporter.start(boost::chrono::milliseconds(10000));

      if (!graphite_reporter_) {
              cppmetrics::graphite::GraphiteSenderPtr graphite_sender(
                new cppmetrics::graphite::GraphiteSenderTCP("graphite.gmu", 2003));

        graphite_reporter_.reset(
          new cppmetrics::graphite::GraphiteReporter(metric_registry, graphite_sender, "radio-radio")
        );
        graphite_reporter_->start(boost::chrono::milliseconds(30000));
      }
    }

    crc32_bb_impl::~crc32_bb_impl() {
      if (!d_packed){
        delete[] d_unpacked_crc;
      }
    }

    int
    crc32_bb_impl::calculate_output_stream_length(const gr_vector_int &ninput_items) {
      if (d_check) {
        return ninput_items[0] - d_crc_length;
      } else {
        return ninput_items[0] + d_crc_length;
      }
    }

    int
    crc32_bb_impl::work(int noutput_items,
                        gr_vector_int &ninput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items) {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      long packet_length = ninput_items[0];
      int packet_size_diff = d_check ? -d_crc_length : d_crc_length;
      unsigned int crc;

       cppmetrics::core::CounterPtr msgs(metric_registry->counter("msgs"));
       cppmetrics::core::CounterPtr errors(metric_registry->counter("error"));

      cppmetrics::core::MeterPtr msgs_meter_ptr(metric_registry->meter("msgs_" + metric_prefix_g));
      cppmetrics::core::MeterPtr error_meter_ptr(metric_registry->meter("error_" + metric_prefix_g));

      msgs_meter_ptr -> mark();

      if (d_check) {
        d_crc_impl.reset();
        d_crc_impl.process_bytes(in, packet_length - d_crc_length);
        crc = d_crc_impl();
        if (d_packed) {
          if (crc != *(unsigned int *) (in + packet_length - d_crc_length)) { // Drop package
            d_nfail++;
            error_meter_ptr -> mark();
            return 0;
          }
        }
        else{
          for(int i=0; i < d_crc_length; i++){
            if(((crc >> i) & 0x1) != *(in + packet_length - d_crc_length + i)) { // Drop package
              d_nfail++;
              error_meter_ptr -> mark();
              return 0;
            }
          }
        }
        d_npass++;

        //std::cout <<"Total " <<d_npass+d_nfail << " "<<"Fail "<<d_nfail<<std::endl;
        memcpy((void *) out, (const void *) in, packet_length - d_crc_length);
      } else {
        d_crc_impl.reset();
        d_crc_impl.process_bytes(in, packet_length);
        crc = d_crc_impl();
        memcpy((void *) out, (const void *) in, packet_length);
        if (d_packed) {
          memcpy((void *) (out + packet_length), &crc, d_crc_length); // FIXME big-endian/little-endian, this might be wrong
        }
        else {
          for (int i = 0; i < d_crc_length; i++) { // unpack CRC and store in buffer
            d_unpacked_crc[i] = (crc >> i) & 0x1;
          }
          memcpy((void *) (out + packet_length), (void *) d_unpacked_crc, d_crc_length);
        }
      }

      std::vector <tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + packet_length);
      for (size_t i = 0; i < tags.size(); i++) {
        tags[i].offset -= nitems_read(0);
        if (d_check && tags[i].offset > (unsigned int) (packet_length + packet_size_diff)) {
          tags[i].offset = packet_length - d_crc_length - 1;
        }
        add_item_tag(0, nitems_written(0) + tags[i].offset,
                     tags[i].key,
                     tags[i].value);
      }

      return packet_length + packet_size_diff;
    }

  } /* namespace digital */
} /* namespace gr */

