# Source From https://github.com/ivanenko/vCard

file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.c")
file(GLOB_RECURSE HEADER_FILES "*.hpp" "*.h")
add_library(libVCard STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(libVCard PUBLIC include)
set_target_properties(libVCard PROPERTIES BUILD_INTERFACE include)
