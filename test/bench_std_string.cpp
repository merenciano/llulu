#include <llulu/lu_time.h>
#include <llulu/lu_defs.h>
#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>

#include <string>

static const size_t ITERATIONS = 1UL << 25UL;

static const std::string bench_data[] = {
    {"#!m1=5}qYO"}, {"xdN4;Z}4pwZL`!>O'NQ&>`"}, {"R]1+_`8Y>y'P7`MHSF!&#?T'j&,"}, {"cL[Bf#<{Zw"}, {"*0CVUuzWu}Y4T`!Wi(<5a]oG^-"}, {":f="}, {"mJ>D5>X"}, {"=4<nq<GD`q>bK{nn7U."},
    {"B"}, {"NeVb/j@@)03BUlc7_gTA4O2cd0s(bCk"}, {"mGo/du?t9sb|*Bebw-qypW+"}, {"]j(H*7KxF1ob')Wg'^+i<lbI"}, {"]95e{oR#9V,Ps0qW5y.{}k&g"}, {"oKPO)fdkaU=clql>pn1bg6[r"}, {"[_6Df(l8Tr{8^^kzAYk0v]}"}, {"__mr=a$u@79)<>pu<*UwtO;M=JCyGI"}};

static void cpp_string_bench()
{
    int idx = 0;
    std::string str;
    bool cnd;
    uint64_t start = lu_time_get();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        std::string a = bench_data[idx];
        if (memcmp(a.data(), str.data(), 17) < 0) {
            a = str;
        }

	++idx;
	idx = idx & 15;
        std::string b = bench_data[idx];


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

