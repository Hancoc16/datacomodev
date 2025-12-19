cmake_minimum_required(VERSION 3.10)
project(DataComErrorDetection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Client 1 - Data Sender
add_executable(client1 client1_sender.cpp)
target_include_directories(client1 PRIVATE .)
target_link_libraries(client1 pthread)

# Server - Intermediate Node + Data Corruptor
add_executable(server server.cpp)
target_include_directories(server PRIVATE .)
target_link_libraries(server pthread)

# Client 2 - Receiver + Error Checker
add_executable(client2 client2_receiver.cpp)
target_include_directories(client2 PRIVATE .)
target_link_libraries(client2 pthread)

