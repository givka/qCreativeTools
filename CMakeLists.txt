cmake_minimum_required(VERSION 3.14)
project(QtTest)

set(CMAKE_CXX_STANDARD 14)

# Tell cmake where Qt is located
set(Qt5_DIR "G:\\Qt5.14.1\\5.14.1\\mingw73_64\\lib\\cmake\\Qt5")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# Tell cmake to find the modules Qt5Core and Qt5widgets
find_package(Qt5 COMPONENTS Core Widgets Multimedia REQUIRED)

include_directories(lib/easyexif)

add_executable(QtTest main.cpp src/main_window.h src/main_window.cpp src/image_viewer.cpp src/image_viewer.h src/icon_provider.cpp src/icon_provider.h src/tree_view.cpp src/tree_view.h src/list_view.cpp src/list_view.h src/visual_preview/visual_preview.cpp src/visual_preview/visual_preview.h src/scene.cpp src/scene.h src/inspector.cpp src/inspector.h lib/easyexif/exif.cpp lib/easyexif/exif.h src/inspector_line.cpp src/inspector_line.h src/styled_item_delegate.cpp src/styled_item_delegate.h)

# Link the library to the executable
target_link_libraries(QtTest Qt5::Core Qt5::Widgets Qt5::Multimedia)
