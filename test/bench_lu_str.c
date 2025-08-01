#include <llulu/lu_str.h>
#include <llulu/lu_time.h>
#include <llulu/lu_defs.h>
#include <llulu/lu_log.h>
#include <stdint.h> 
#include <stdlib.h>

static const lu_sstr bench_data[] = {
    {"#!m1=5}qYO"}, {"xdN4;Z}4pwZL`!>O'NQ&>`"}, {"R]1+_`8Y>y'P7`MHSF!&#?T'j&,"}, {"cL[Bf#<{Zw"}, {"*0CVUuzWu}Y4T`!Wi(<5a]oG^-"}, {":f="}, {"mJ>D5>X"}, {"=4<nq<GD`q>bK{nn7U."},
    {"B"}, {"NeVb/j@@)03BUlc7_gTA4O2cd0s(bCk"}, {"mGo/du?t9sb|*Bebw-qypW+"}, {"]j(H*7KxF1ob')Wg'^+i<lbI"}, {"]95e{oR#9V,Ps0qW5y.{}k&g"}, {"oKPO)fdkaU=clql>pn1bg6[r"}, {"[_6Df(l8Tr{8^^kzAYk0v]}"}, {"__mr=a$u@79)<>pu<*UwtO;M=JCyGI"}};

int main(int argc, char **argv)
{
    enum { ITERATIONS = 1UL << 25UL };
    int idx = 0;
    lu_sstr str;
    bool cnd;
    uint64_t start = lu_time_get();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        lu_sstr a = bench_data[idx];
        if (lu_sstr_cmpmem(&a, str.str, 17) < 0) {
            lu_sstr_copy(&a, str.str);
        }

        ++idx;
        idx = idx & 15;

        lu_sstr b = bench_data[idx];

        if (!lu_sstr_equal(&a, &b)) {
            a = b;
        } else  {
            lu_sstr_clear(&a);
        }

        cnd = lu_sstr_cmpstr(&a, b.str);
        if (cnd || lu_sstr_empty(&a)) {
            str = a;
        } else {
            str = b;
        }
    }

    uint64_t lapse = lu_time_elapsed(start);
    lu_log("Elapsed test lu_str: %f sec. (%s, %d)\n", lu_time_sec(lapse), str.str, cnd);
    return 0;
}
