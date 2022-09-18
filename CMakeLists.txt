cmake_minimum_required(VERSION 3.16.3)

project(coma)

add_executable(${PROJECT_NAME} src/main.cpp)


add_subdirectory("src")

target_link_directories(${PROJECT_NAME} PRIVATE
                            "conf"
                            "src"
                            "src/action"
)

target_include_directories(${PROJECT_NAME} PUBLIC
                            "conf"
                            "src" 
                            "src/action"
)

target_link_libraries(${PROJECT_NAME} src)