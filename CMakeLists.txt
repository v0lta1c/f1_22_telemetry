cmake_minimum_required(VERSION 3.29) # Version of CMAKE required 

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(F1TelemetryApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/Users/mannb/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

# Ensure MSVC sets the correct __cplusplus value
if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)
find_package(nlohmann_json CONFIG REQUIRED)

# Specify the source files
file(GLOB SOURCES "src/*.cpp" "src/ui/*.cpp" "src/telemetry/*.cpp" "src/network/*.cpp")

if(SOURCES STREQUAL "")
    message(FATAL_ERROR "No source files founds in src directory!")
endif()

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Qt Libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        Qt6::Core 
        Qt6::Gui 
        Qt6::Widgets
        Qt6::Network
        nlohmann_json::nlohmann_json)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Gui_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Qt6Network_INCLUDE_DIRS}
)
