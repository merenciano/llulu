#include <llulu/lu_time.h>
#include <llulu/lu_defs.h>
#include "bench_sstr_data.h"
#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>

#include <string>

static const size_t ITERATIONS = 1UL << 25UL;

static void cpp_string_bench()
{
    int idx = 0;
    std::string str;
    bool cnd;
    uint64_t start = lu_time_get();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        std::string a = bench_data[idx].str;
        if (memcmp(a.data(), str.data(), 17) < 0) {
            a = str;
        }

	++idx;
	idx = idx & 15;
        std::string b = bench_data[idx].str;


        if (a != b) {
            a = b;
        } else  {
            a.clear();
        }

        cnd = !(a == b);
        if (cnd || a.empty()) {
            str = a;
        } else {
            str = b;
        }
    }

    uint64_t lapse = lu_time_elapsed(start);
    printf("Elapsed test std::string: %f sec. (%s, %d)\n", lu_time_sec(lapse), str.c_str(), cnd);
}

int main(int argc, char **argv)
{
    cpp_string_bench();
    return 0;
}

