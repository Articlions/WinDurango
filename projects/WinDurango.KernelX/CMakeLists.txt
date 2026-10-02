cmake_minimum_required(VERSION 4.0)
project(WinDurango.KernelX VERSION 1.0.0)

set(VERSION_SUFFIX "-dev.1") # used for non-stable versions, otherwise blank
set(CMAKE_CXX_STANDARD 20)

find_path(DETOURS_INCLUDE_DIRS "detours/detours.h" REQUIRED HINTS "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include")
find_library(DETOURS_LIBRARY detours REQUIRED HINTS "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib")

# libavcodec supplies the XMA2 decoder the emulated audio processor needs. The Xbox One decodes XMA
# in hardware and no software decoder for it exists on the title's side, so every sound in every
# bank is silent without this.
find_path(FFMPEG_INCLUDE_DIRS "libavcodec/avcodec.h" REQUIRED)
find_library(AVCODEC_LIBRARY avcodec REQUIRED)
find_library(AVUTIL_LIBRARY avutil REQUIRED)

set(FILES
    src/kernelx.cpp
    src/dllmain.cpp
    src/Hooks.cpp
    src/CurrentApp.cpp
    include/WinDurango.KernelX/CurrentApp.h
    include/WinDurango.KernelX/kernelx.h
    include/WinDurango.KernelX/Logan.h
    include/WinDurango.KernelX/Hooks.h
)

add_library(WinDurango.KernelX SHARED ${FILES} "include/WinDurango.KernelX/EraCoreApplication.h" "src/EraCoreApplication.cpp" "include/WinDurango.KernelX/EraCoreWindow.h" "src/EraCoreWindow.cpp" "include/WinDurango.KernelX/MMDeviceEnumerator.h" "src/MMDeviceEnumerator.cpp" "include/WinDurango.KernelX/ForzaThreadHook.h" "include/WinDurango.KernelX/D3D11Peggle2Hook.h" "src/Logan.cpp" "include/WinDurango.KernelX/ACPLoganTypes.h" "src/XmaDecoder.cpp" "include/WinDurango.KernelX/XmaDecoder.h" "include/WinDurango.KernelX/EraNetworking.h" "src/EraNetworking.cpp")

target_link_libraries(WinDurango.KernelX PRIVATE 
    WinDurango.Common 
    WinDurango.Implementation.WinRT 
    ntdll
    ${DETOURS_LIBRARY}
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
)

target_link_options(WinDurango.KernelX
    PUBLIC 
    $<$<CXX_COMPILER_ID:MSVC>:/FORCE:MULTIPLE>
)

target_include_directories(WinDurango.KernelX PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/WinDurango.KernelX/
    ${CMAKE_SOURCE_DIR}/projects/WinDurango.Common/include
    ${CMAKE_SOURCE_DIR}/projects/WinDurango.Implementation.WinRT/include
    ${DETOURS_INCLUDE_DIRS}
    ${FFMPEG_INCLUDE_DIRS}
)

set_target_properties(WinDurango.KernelX PROPERTIES
    OUTPUT_NAME "kernelx"
)