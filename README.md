# Usage

```bash
mkdir build
cd build
cmake ..
make
./qCreativeTools
```

## Note
Before compiling, go to `main.cpp`, line 10 and set the path to your path.

```cpp
const QString Utility::path = QString("/Users/artich/dev/qCreativeTools");
```

Go to `CMakeLists.txt` and set `Qt5_DIR` to your Qt directory.

# Requirements
- OpenCV
- Qt
