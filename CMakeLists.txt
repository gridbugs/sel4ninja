cmake_minimum_required(VERSION 3.7.2)

include(settings.cmake)

project(sel4min C CXX ASM)

find_package(seL4 REQUIRED)
find_package(elfloader-tool REQUIRED)

set(KernelRootCNodeSizeBits 13 CACHE INTERNAL "")
set(KernelDomainSchedule "${CMAKE_CURRENT_LIST_DIR}/domain_schedule.c" CACHE INTERNAL "")
sel4_import_kernel()

ApplyData61ElfLoaderSettings(${KernelPlatform} ${KernelSel4Arch})
elfloader_import_project()

add_subdirectory(apps/hello-ninja)
