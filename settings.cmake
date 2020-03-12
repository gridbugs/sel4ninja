cmake_minimum_required(VERSION 3.7.2)

set(project_dir "${CMAKE_CURRENT_LIST_DIR}/../../")
file(GLOB project_modules ${project_dir}/projects/*)
list(
    APPEND
        CMAKE_MODULE_PATH
        ${project_dir}/kernel/
        ${project_dir}/tools/seL4/cmake-tool/helpers/
        ${project_dir}/tools/seL4/elfloader-tool/
        ${project_modules}
)

set(NANOPB_SRC_ROOT_FOLDER "${project_dir}/tools/nanopb" CACHE INTERNAL "")

set(SEL4_CONFIG_DEFAULT_ADVANCED ON)

include(application_settings)

include(${CMAKE_CURRENT_LIST_DIR}/easy-settings.cmake)

correct_platform_strings()

find_package(seL4 REQUIRED)
sel4_configure_platform_settings()

ApplyCommonReleaseVerificationSettings(${RELEASE} ${VERIFICATION})
