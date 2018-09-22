//
// Created by amindelavar on 9/19/2018.
//
#include <MPL/system.h>


void init_built_in_funcs(){
	add_to_bifs(0,0,0,0,0);
	/**
	 * a=bool|str|num : value
	 * aa=bool[?,..]|str[?,..]|num[?,..] : var
	 * st=struct : value
	 * sta=struct[?,..] : var
	 */
	//----------------------------------mpl built_in
	add_to_bifs(1,MPL_BUILT_IN_TYPE,"len","aa","num[?]");
	add_to_bifs(2,MPL_BUILT_IN_TYPE,"print","a","bool");
	add_to_bifs(3,MPL_BUILT_IN_TYPE,"printA","aa","bool");
	add_to_bifs(4,MPL_BUILT_IN_TYPE,"printS","st","bool");
	add_to_bifs(5,MPL_BUILT_IN_TYPE,"printSA","sta","bool");
	add_to_bifs(6,MPL_BUILT_IN_TYPE,"push","aa;aa","bool");
	add_to_bifs(7,MPL_BUILT_IN_TYPE,"pop","aa","bool");
	add_to_bifs(8,MPL_BUILT_IN_TYPE,"typeof","a","str");
	add_to_bifs(9,MPL_BUILT_IN_TYPE,"destroy","aa","bool");
	add_to_bifs(10,MPL_BUILT_IN_TYPE,"input","bool","str");
	add_to_bifs(11,MPL_BUILT_IN_TYPE,"errhandle","num;str;str","bool");
	add_to_bifs(12,MPL_BUILT_IN_TYPE,"config_all","","str[?,2]");
	add_to_bifs(13,MPL_BUILT_IN_TYPE,"define_all","","str[?,2]");
	add_to_bifs(14,MPL_BUILT_IN_TYPE,"embed_run","str[?]","num");
	add_to_bifs(15,MPL_BUILT_IN_TYPE,"input","num","str");
	//----------------------------------data types built_in
	
	//----------------------------------os built_in
	
	
	
	
	
	

}
