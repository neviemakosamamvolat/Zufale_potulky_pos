cmake_minimum_required(VERSION 3.25)
project(remote_system_monitor_client_test C)
set(CMAKE_C_STANDARD 11)
add_executable(remote_system_monitor_client main.c socket_utils.c cpu_protocol.c)
target_compile_options(remote_system_monitor_client PRIVATE
-Wall
-Wextra
-Wpedantic
)