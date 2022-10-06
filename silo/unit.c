#include <assert.h>
#include <stdio.h>
#include "include/silo/helper/str.h"

void test_my_str(){
	char a[] = "abcdefg01234fbg";
	const char *p = my_str_str(a, "01234");
	assert(*p == '0');
	char *end;
	long num = my_str_to_l(p, &end, 10);
	assert(num == 1234);
	assert(*end == 'f');
	assert(size_t_pow(10, 3) == 1000);
	char *b = "r 0234567";
	b+=2;
	uint64_t out;
	b = read_int(b, 7, &out);
	assert(out == 234567);
	assert(b[-1] == '7');
}

int main(){
	test_my_str();
	printf("Test done\n");
	return 0;
}
