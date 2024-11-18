# Source From https://github.com/ivanenko/vCard

file(GLOB_RECURSE HEADER_FILES "*.hpp" "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.c")
add_library(VCard STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(VCard PUBLIC include)
set_target_properties(VCard PROPERTIES BUILD_INTERFACE include)
