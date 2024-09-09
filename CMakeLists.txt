cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME "EmbedCalendar")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

link_directories("D:\\sdk\\skia\\out"
"D:\\sdk\\curl\\build\\lib"
"D:\\sdk\\libwebsockets\\build\\lib")

include_directories("D:\\sdk\\skia"
"D:\\sdk\\rapidjson\\include"
"D:\\sdk\\curl\\include"
"D:\\sdk\\libwebsockets\\build\\include")

add_executable(${PROJECT_NAME} WIN32)

add_subdirectory(src)
add_subdirectory(res)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/NODEFAULTLIB:libcmt /DEFAULTLIB:msvcrt ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/NODEFAULTLIB:libcmtd /DEFAULTLIB:msvcrtd ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
    #set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "_4000")
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DPI_AWARE "PerMonitor")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
    target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:UNICODE CURL_STATICLIB DEBUG WIN32_LEAN_AND_MEAN NOMINMAX _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS SK_DEBUG WIN32 _WINDOWS>)
    target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:UNICODE CURL_STATICLIB NDEBUG WIN32_LEAN_AND_MEAN NOMINMAX _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS SK_RELEASE WIN32 _WINDOWS>)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
        wldap32
        ws2_32
        userenv
        psapi
        iphlpapi
        crypt32
        ole32
        winspool
        oleaut32
        uuid
        comdlg32
        advapi32
        kernel32 
        user32 
        shell32 
        shcore 
        imm32 
        gdi32 
        usp10 
        dwmapi 
        winmm
        websockets_static
        libcurl
        skia     
    )
elseif(APPLE)
endif()