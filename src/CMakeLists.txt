add_executable(vcdb main.cpp)

add_subdirectory(Interface)

find_package(OpenMP REQUIRED)

target_link_libraries(vcdb PUBLIC vcdbInterface stdc++fs OpenMP::OpenMP_CXX)
