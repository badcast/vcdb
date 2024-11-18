add_executable(vcfdb main.cpp)

add_subdirectory(3rdparty/vCard)
add_subdirectory(Interface)
target_link_libraries(vcfdb_interface PUBLIC libVCard)

target_link_libraries(vcfdb PUBLIC libVCard vcfdb_interface)
