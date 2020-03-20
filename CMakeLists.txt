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

add_executable(QCreativeTools main.cpp src/media/media_window.h src/media/media_window.cpp src/media/icon_provider.cpp src/media/icon_provider.h src/media/tree_view.cpp src/media/tree_view.h src/media/list_view.cpp src/media/list_view.h src/media/visual_preview.cpp src/media/visual_preview.h src/media/scene.cpp src/media/scene.h src/media/inspector.cpp src/media/inspector.h lib/easyexif/exif.cpp lib/easyexif/exif.h src/media/inspector_line.cpp src/media/inspector_line.h src/media/styled_item_delegate.cpp src/media/styled_item_delegate.h src/media/icon_proxy.cpp src/media/icon_proxy.h src/media/filter_model.cpp src/media/filter_model.h src/media/opencv.cpp src/media/opencv.h src/settings/settings_window.cpp src/settings/settings_window.h src/settings/settings.cpp src/settings/settings.h src/utility.cpp src/utility.h src/easing/easing_window.cpp src/easing/easing_window.h)

target_link_libraries(QCreativeTools Qt5::Core Qt5::Widgets Qt5::Multimedia)
target_link_libraries(QCreativeTools ${OpenCV_LIBS})
