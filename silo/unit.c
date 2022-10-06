#include <assert.h>
#include <stdio.h>
#include "include/silo/helper/str.h"

void test_my_str(){
	char a[] = "abcdefg1234fbg";
	const char *p = my_str_str(a, "1234");
	assert(*p == '1');
	char *end;
	long num = my_str_to_l(p, &end, 10);
	assert(num == 1234);
	assert(*end == 'f');
}

int main(){
	test_my_str();
	printf("Test done\n");
	return 0;
}
