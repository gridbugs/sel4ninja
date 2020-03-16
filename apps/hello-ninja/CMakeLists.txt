cmake_minimum_required(VERSION 3.7.2)

project(hello-world C)

find_package(musllibc REQUIRED)
find_package(util_libs REQUIRED)
find_package(seL4_libs REQUIRED)
find_package(sel4_projects_libs REQUIRED)

musllibc_setup_build_environment_with_sel4runtime()
sel4_import_libsel4()
util_libs_import_libraries()
sel4_libs_import_libraries()
sel4_projects_libs_import_libraries()

file(
    GLOB
        static
        src/*.c
)

add_executable(hello-world EXCLUDE_FROM_ALL ${static})

target_include_directories(hello-world PRIVATE "include")

target_link_libraries(
    hello-world
    PUBLIC
        sel4_autoconf
        muslc
        sel4
        sel4runtime
        sel4allocman
        sel4vka
        sel4utils
        sel4platsupport
        sel4muslcsys
)

target_compile_options(hello-world PRIVATE -Werror -Wall -g)

include(rootserver)
DeclareRootserver(hello-world)
