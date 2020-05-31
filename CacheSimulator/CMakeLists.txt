cmake_minimum_required(VERSION 3.16)
project(CacheSimulator C)

set(CMAKE_C_STANDARD 99)

add_executable(CacheSimulator src/main.c include/SnoopingController.c include/SnoopingController.h include/message.h src/main.c include/cache.h include/memory.h include/block.h include/instruction.c include/instruction.h include/randomPoisson.c include/randomPoisson.h include/bus.h)