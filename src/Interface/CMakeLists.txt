file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.c")
file(GLOB_RECURSE HEADER_FILES "*.hpp" "*.h")

add_library(vcdbInterface STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(vcdbInterface PUBLIC .)
set_target_properties(vcdbInterface PROPERTIES BUILD_INTERFACE .)

find_package(SQLite3 REQUIRED)

add_subdirectory(3rdparty/vCard)

target_link_libraries(vcdbInterface PRIVATE VCard SQLite::SQLite3)
