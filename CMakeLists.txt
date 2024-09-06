cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME "EmbedCalendar")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")


link_directories("D:\\sdk\\skia\\out")
link_directories("D:\\sdk\\curl\\build\\lib\\Release")
link_directories("D:\\sdk\\curl\\build\\lib\\Debug")
include_directories("D:\\sdk\\skia")
include_directories("D:\\sdk\\rapidjson\\include")
include_directories("D:\\sdk\\curl\\include")

add_executable(${PROJECT_NAME})
add_subdirectory(src)
add_subdirectory(res)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DPI_AWARE "PerMonitor")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
    target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:UNICODE DEBUG NOMINMAX _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS SK_DEBUG WIN32 _WINDOWS>)
    target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:UNICODE NDEBUG NOMINMAX _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS SK_RELEASE WIN32 _WINDOWS>)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} skia libcurl kernel32 user32 shell32 shcore imm32 gdi32 usp10 dwmapi d2d1 winmm)
elseif(APPLE)
endif()