#include <assert.h>
#include <stdio.h>
#include <inttypes.h>
#include "include/silo/helper/str.h"
#include "include/silo/xoroshiro_128plus.h"

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

void test_rand(){


	struct xoroshiro_128plus r = init_xoroshiro_128plus(0);
	printf("Rand: %"PRIu64"\n", next(&r));
	printf("Rand: %"PRIu64"\n", next(&r));
	printf("Rand: %"PRIu64"\n", next(&r));
	printf("Rand: %"PRIu64"\n", next(&r));

}

int main(){
	test_my_str();
	test_rand();
	printf("Test done\n");
	return 0;
}
