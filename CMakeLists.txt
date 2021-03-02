cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 11)

project(line-analyze-rev)

include_directories(src/include)
include_directories(srclibs/libxlsxwriter/include)

file(GLOB app_SRC "src/*.cpp")
file(GLOB libxlsxwriter_SRC "srclibs/libxlsxwriter/src/*.c")
file(GLOB minizip_SRC "srclibs/libxlsxwriter/third_party/minizip/*.c")

list(FILTER minizip_SRC EXCLUDE REGEX ".*iowin32\.c")
list(FILTER minizip_SRC EXCLUDE REGEX ".*miniunz\.c")
list(FILTER minizip_SRC EXCLUDE REGEX ".*minizip\.c")

add_executable(line-analyze-rev ${libxlsxwriter_SRC} srclibs/libxlsxwriter/third_party/tmpfileplus/tmpfileplus.c srclibs/libxlsxwriter/third_party/md5/md5.c ${minizip_SRC} ${app_SRC})
target_link_libraries(line-analyze-rev z)
