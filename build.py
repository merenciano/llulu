#!/usr/bin/python

import os
import subprocess

def generate_bench_data():
    subprocess.check_call("tcc test/misc/rand_sstr_gen.c -o build/datagen")
    subprocess.check_call("build/datagen test/bench_sstr_data.h -s69 -cpp")

def build_unittest():
    compiler = "clang"
    comp_flags = ["-std=c99", "-Wall", "-g"]
    include_dirs = ["-Iinclude"]
    sources = ["test/unitt.c"]
    out = "-o bin/unittest"

    command = [compiler, ' '.join(comp_flags), ' '.join(include_dirs), ' '.join(sources), out]
    command = ' '.join(command)
    print(command)
    subprocess.check_call(command, shell=True)


def build_benchmark():
    compiler = "clang++"
    comp_flags = ["-std=c++20", "-Wall", "-O3", "-march=native"]
    include_dirs = ["-Iinclude"]
    sources = ["test/bench.c"]
    out = "-o bin/benchmark"

    command = [compiler, ' '.join(comp_flags), ' '.join(include_dirs), ' '.join(sources), out]
    command = ' '.join(command)
    print(command)
    subprocess.check_call(command, shell=True)


def main():
    os.makedirs('bin', exist_ok=True)
    os.makedirs('build', exist_ok=True)

    #build_unittest()
    generate_bench_data()
    #build_benchmark()


if __name__ == '__main__':
    main()
