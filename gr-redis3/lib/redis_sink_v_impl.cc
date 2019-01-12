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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "redis_sink_v_impl.h"
#include <redox.hpp>
#include <iostream>
#include <mysql.h>
#include <unistd.h>
#include <chrono>
using namespace std::chrono;

namespace gr {
  namespace redis3 {
    redox::Redox rdx_pub1;
    int msize;
    int cnt;
    boost::mutex recv_mutex;
    redis_sink_v::sptr
    redis_sink_v::make(int msgsize)
    {
	if (!rdx_pub1.connect("localhost",6379)) {
	    	    std::cout << "Error"<<::std::endl;
	    	    	        	        	     // handle error
	    	 }
      return gnuradio::get_initial_sptr
        (new redis_sink_v_impl(msgsize));
    }

    /*
     * The private constructor
     */
    redis_sink_v_impl::redis_sink_v_impl(int msgsize)
      : gr::sync_block("redis_sink_v",
              gr::io_signature::make(1,1, sizeof(char)*msgsize),
              gr::io_signature::make(0, 0, 0))
    {

	msize=msgsize;	
	ip = (char*)"127.0.0.1";
	//need ip of server
	usr = (char*)"redis";
	pass = (char*)"sider";
	db = (char*)"radio";
	mysql_init(&mysql);
 
	connection = mysql_real_connect(&mysql, ip, usr, pass, db, 0, NULL, 0);
 
	if (connection==NULL)
	{
		std::cout<<mysql_error(&mysql)<<std::endl;

	}


	}

    /*
     * Our virtual destructor.
     */
    redis_sink_v_impl::~redis_sink_v_impl()
    {
    }

    int
    redis_sink_v_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
     noutput_items=1;
        char *in = (char *) input_items[0];
	std::string query;	
	std::string str=std::string(in);
	struct timespec spec;
	cnt++;	
	std::cout<<"RxCh "<<std::to_string(cnt)<<": "<<str.substr(0,msize)<<std::endl;
	if(str.length()>0 )
	{
		struct timeval start, end;

    		long ms = duration_cast< milliseconds >(
    			system_clock::now().time_since_epoch()
		).count();   
		std::string date1=std::to_string(ms);
		rdx_pub1.publish ("RxCh",str.substr(0,msize));
		query = std::string("INSERT into `radio`.`rxtbl` (`msg`, `num`, `time`) VALUES ('") + str + "', 'NULL', '" + date1 + "')";
		mysql_query(&mysql,query.c_str());
		if (query_state !=0) {
			std::cout << mysql_error(connection) << std::endl;
			return 1;
		}
		noutput_items=1;
	}
	
        consume_each (noutput_items);
      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace redis3 */
} /* namespace gr */

