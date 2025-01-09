file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.c")
file(GLOB_RECURSE HEADER_FILES "*.hpp" "*.h")

add_library(vcdbInterface STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(vcdbInterface PUBLIC .)
set_target_properties(vcdbInterface PROPERTIES BUILD_INTERFACE .)

target_link_libraries(vcdbInterface PRIVATE VCard)

find_package(SQLite3)
if(SQLite3_FOUND)
  target_compile_definitions(vcdbInterface PRIVATE VCDB_HAS_SQLITE3)
  target_link_libraries(vcdbInterface PRIVATE SQLite::SQLite3)
endif()

find_package(OpenSSL)
if(OpenSSL_FOUND)
  target_compile_definitions(vcdbInterface PUBLIC VCDB_HAS_OPENSSL)
  target_link_libraries(vcdbInterface PRIVATE OpenSSL::Crypto)
endif()

add_subdirectory(3rdparty/vCard)
