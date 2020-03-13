cmake_minimum_required(VERSION 3.14)
project(QCreativeTools)

set(CMAKE_CXX_STANDARD 14)

if (APPLE)
    set(Qt5_DIR "~/Qt5.9.0/5.9/clang_64/lib/cmake/Qt5")
else ()
    set(Qt5_DIR "G:\\Qt5.14.1\\5.14.1\\mingw73_64\\lib\\cmake\\Qt5")
endif ()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets Multimedia REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(lib/easyexif)

add_executable(QCreativeTools main.cpp src/main_window.h src/main_window.cpp src/icon_provider.cpp src/icon_provider.h src/tree_view.cpp src/tree_view.h src/list_view.cpp src/list_view.h src/visual_preview/visual_preview.cpp src/visual_preview/visual_preview.h src/scene.cpp src/scene.h src/inspector.cpp src/inspector.h lib/easyexif/exif.cpp lib/easyexif/exif.h src/inspector_line.cpp src/inspector_line.h src/styled_item_delegate.cpp src/styled_item_delegate.h src/icon_proxy.cpp src/icon_proxy.h src/filter_model.cpp src/filter_model.h src/opencv.cpp src/opencv.h)

target_link_libraries(QCreativeTools Qt5::Core Qt5::Widgets Qt5::Multimedia)
target_link_libraries(QCreativeTools ${OpenCV_LIBS})
