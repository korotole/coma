cmake_minimum_required(VERSION 3.16.3)

project(coma)

add_executable(${PROJECT_NAME} main.cpp)


add_subdirectory(core)

target_link_directories(${PROJECT_NAME} PRIVATE
                            "."
                            "core"
                            "core/compression"
                            "core/encryption"
)

target_include_directories(${PROJECT_NAME} PUBLIC
                            "." 
                            "conf"
                            "core" 
                            "core/compression"
                            "core/encryption"
)

target_link_libraries(${PROJECT_NAME} CORE)