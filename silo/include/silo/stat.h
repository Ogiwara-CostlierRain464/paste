#ifndef PASTE_STAT_H
#define PASTE_STAT_H

#include <stdlib.h>
#include <stdio.h>
#include "tuple.h"

struct KeyStat{ // per key in a thread
	// in read, thread i access to key k and failed getting lock
	// r-w contention
	size_t waitWUnlockInReadCount;
	// in commit, thread i try to read key k and failed because of
	// ahead write
	// r-w contention
	size_t readLockFailCount;
	// in write, thread i try to w-lock key k and fail because of
	// concurrent w-lock
	// w-w contention
	size_t waitWUnlockInWriteCount;
};

struct KeyStat* make_arr_per_conn(size_t tuple_num){
	return calloc(tuple_num, sizeof(struct KeyStat)); // init with zero
}

static void stat_calculate_and_print(
    struct KeyStat** arr, //arr[thread][key]
    size_t conn_num,
    size_t tuple_num,
    size_t tuple_div_num,
    size_t conn_div_num){
	for(size_t i = 0; i < conn_num; i+=conn_div_num){
		size_t counter = 0;
		size_t conn_unit_len = conn_num / conn_div_num;
		for(size_t c = i; c < i+conn_unit_len; c++){
			for(size_t k = 0; k < tuple_num; k++){
				counter+= arr[c][k].waitWUnlockInReadCount;
				counter+= arr[c][k].readLockFailCount;
				counter+= arr[c][k].waitWUnlockInWriteCount;
			}
		}

		printf("conn %zu - %zu:\t\t%zu\n", i, i+conn_unit_len, counter);
	}


	struct KeyStat sum_arr[tuple_num];
	for(size_t i = 0; i < conn_num; i++){
		struct KeyStat *conn_arr = arr[i];
		for(size_t k = 0; k < tuple_num; k++){
			sum_arr[k].waitWUnlockInReadCount += conn_arr[k].waitWUnlockInReadCount;
			sum_arr[k].readLockFailCount += conn_arr[k].readLockFailCount;
			sum_arr[k].waitWUnlockInWriteCount += conn_arr[k].waitWUnlockInWriteCount;
		}
	}
	// how to report?
	// Just be simple bar
	// 0-2500: 		||||||||||||||||||
	// 2500-5000:		||||| ||||| |||||
	// 997500-1000000: 	||||| ||||| ||||| |||||

	// per thread
	// T1:

	// I need to check per contention

	size_t unit_len = tuple_num / tuple_div_num;
	for(size_t i = 0; i < tuple_num; i+=unit_len){
		size_t counter = 0;
		for(size_t k = i; k < i+unit_len; k++){
			counter+= sum_arr[k].waitWUnlockInReadCount;
			counter+= sum_arr[k].readLockFailCount;
			counter+= sum_arr[k].waitWUnlockInWriteCount;
		}

		printf("key %zu - %zu:\t\t%zu\n", i, i+unit_len, counter);
	}
}

#endif // PASTE_STAT_H
