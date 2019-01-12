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

#ifndef INCLUDED_REDIS3_REDIS_SINK_V_IMPL_H
#define INCLUDED_REDIS3_REDIS_SINK_V_IMPL_H

#include <redis3/redis_sink_v.h>
#include <mysql.h>
namespace gr {
  namespace redis3 {

    class redis_sink_v_impl : public redis_sink_v
    {
     private:
	MYSQL mysql,*connection;
	std::string query;
	MYSQL_RES result;
	MYSQL_ROW row;
	int query_state;
	char * ip;
	//need ip of server
	char * usr;
	char * pass;
	char * db;

     public:
      redis_sink_v_impl(int msgsize);
      ~redis_sink_v_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace redis3
} // namespace gr

#endif /* INCLUDED_REDIS3_REDIS_SINK_V_IMPL_H */

