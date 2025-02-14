# ImgLib — библиотека для работы с изображениями

## Описание
**ImgLib** — это кросс-платформенная библиотека на C++ для чтения и записи изображений в форматах BMP, JPEG и PPM. Проект создавался для:
- Работы с бинарными форматами (BMP, PPM).
- Интеграции сторонних библиотек (LibJPEG).
- Сборки проектов с помощью CMake.
- Обработки пиксельных данных.

**Стек технологий**:
- Язык: C++17
- Сборка: CMake 3.25+
- Зависимости: LibJPEG (статическая линковка)
- Дополнительно: файловые системы C++17, структуры с выравниванием (Packed Structs)

---

## Системные требования
- Компилятор с поддержкой C++17 (GCC 10+, Clang 12+, MSVC 19.30+)
- Установленный LibJPEG (статическая сборка)
- CMake ≥ 3.25.1

---

# Инструкция по развёртыванию проекта

## 1. Установите зависимости

### Для Linux (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install -y git build-essential cmake libjpeg-dev
```

### Для Windows:

#### 1. Если не установлено, установите:
- [Git for Windows](https://gitforwindows.org/) (включая Git Bash)
- [CMake](https://cmake.org/download/)
- [Visual Studio 2022](https://visualstudio.microsoft.com/ru/) с компонентами:
   - "Desktop development with C++"
   - "C++ CMake tools"

#### 2. Соберите LibJPEG вручную (обязательно!):
```bash
# Скачайте исходники с https://www.ijg.org/ (jpegsr.zip)
unzip jpegsr.zip
cd jpeg-9f  # версия может отличаться

# Сборка статической библиотеки
mkdir build && cd build
../configure --enable-static=yes --enable-shared=no
make

# Создайте структуру папок для CMake
mkdir -p /c/libs/libjpeg/{include,lib/Release}
cp *.h /c/libs/libjpeg/include
cp .libs/libjpeg.a /c/libs/libjpeg/lib/Release
```
## 2. Соберите проект

### Для Linux:
```bash
git clone https://github.com/yourusername/ImgLib.git
cd ImgLib
mkdir build && cd build
cmake -DLIBJPEG_DIR=/usr/lib/x86_64-linux-gnu ..  # путь к libjpeg-dev
cmake --build . --config Release
```

### Для Windows (через Visual Studio):
```bash
git clone https://github.com/yourusername/ImgLib.git
cd ImgLib
mkdir build && cd build
cmake -DLIBJPEG_DIR="C:/libs/libjpeg" -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

## 3. Проверьте работу
```bash
# Конвертация JPEG → BMP
./ImgConverter/imgconv input.jpg output.bmp

# Конвертация BMP → PPM
./ImgConverter/imgconv input.bmp output.ppm
```
# Планы по доработке

### 1. Поддержка формата PNG
Использовать библиотеку libpng для чтения/записи PNG с прозрачностью.

### 2. Оптимизация памяти
Внедрить lazy-loading для больших изображений.

### 3. Расширенные операции
Добавить базовые преобразования: обрезка, поворот, изменение размера.

### 4. Тестирование
Написать unit-тесты с Google Test для проверки корректности чтения/записи.

### 5. CI/CD
Настроить GitHub Actions для автоматической сборки на Windows/Linux/macOS.

