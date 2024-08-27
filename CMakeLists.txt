cmake_minimum_required(VERSION 3.5.0)
project(GJK VERSION 0.1.0 LANGUAGES C CXX)

include(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})


target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/shaders/"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)



