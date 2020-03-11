# Windows compilation with CMake and CLion

- -DCMAKE_CXX_COMPILER=G:/Qt5.14.1/Tools/mingw730_64/bin/g++.exe
- CLion Working directory: G:\Qt5.14.1\5.14.1\mingw73_64\bin
- set(Qt5_DIR "G:\\Qt5.14.1\\5.14.1\\mingw73_64\\lib\\cmake\\Qt5")

# Notes
## The Meta-Object System
- we strongly recommend that all subclasses of QObject use the Q_OBJECT macro regardless of whether or not they actually use signals, slots, and properties


## The Property System
- QObject::setProperty() can also be used to add new properties to an instance of a class at runtime. When it is called with a name and a value, if a property with the given name exists in the QObject, and if the given value is compatible with the property's type, the value is stored in the property, and true is returned. If the value is not compatible with the property's type, the property is not changed, and false is returned. But if the property with the given name doesn't exist in the QObject (i.e., if it wasn't declared with Q_PROPERTY()), a new property with the given name and value is automatically added to the QObject, but false is still returned. This means that a return of false can't be used to determine whether a particular property was actually set, unless you know in advance that the property already exists in the QObject.
 
 ## [Object Model](https://doc.qt.io/qt-5/object.html)
 
 ## Signals and slot
 - Note that the setValue() function sets the value and emits the signal only if value != m_value. This prevents infinite looping in the case of cyclic connections (e.g., if b.valueChanged() were connected to a.setValue()).
 
 