cmake_minimum_required(VERSION 3.5.0)
project(GJK VERSION 0.1.0 LANGUAGES C CXX)

include(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)


find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp GJK.h Transform.h shader.h)


target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${PROJECT_SOURCE_DIR}/vs.glsl"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/vs.glsl"
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${PROJECT_SOURCE_DIR}/vs-3d.glsl"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/vs-3d.glsl"
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${PROJECT_SOURCE_DIR}/fs.glsl"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/fs.glsl"
)



