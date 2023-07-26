cmake_minimum_required(VERSION 3.18)
project(prosim C)

set(CMAKE_C_STANDARD 99)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(prosim main.c context.c context.h process.c process.h prio_q.h prio_q.c)
find_package(Threads REQUIRED)
target_link_libraries(prosim PRIVATE Threads::Threads)