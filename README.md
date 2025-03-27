# Rotating Objects by Quaternion

Интерактивная визуализация вращающегося икосаэдра с использованием OpenGL.

## О проекте

Это пет-проект, который служит подготовительным этапом для более сложного проекта по визуализации вращения выпуклой оболочки трехмерных точек. В данном проекте реализованы ключевые компоненты, которые будут использованы в следующем проекте:

- Работа с графикой через OpenGL
- Реализация кватернионов для плавного вращения в 3D пространстве
- Корректная визуализация граней с учетом их глубины
- Динамическое изменение оси вращения
- Возможность вращать объекты с расширением *.obj 

Этот проект помогает отработать базовые концепции и технические решения, которые затем будут масштабированы для работы с произвольными наборами точек и их выпуклыми оболочками.

## Описание

Проект представляет собой визуализацию демонстрационных файлов, которые плавно вращается в пространстве с динамически меняющейся осью вращения.

## Зависимости

- CMake (минимальная версия 3.10)
- OpenGL
- GLFW3
- GLEW

## Установка зависимостей

### macOS
```bash
# Установка через Homebrew
brew install cmake
brew install glfw
brew install glew
```

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install cmake
sudo apt-get install libglfw3-dev
sudo apt-get install libglew-dev
sudo apt-get install libgl1-mesa-dev
```

### Windows
1. Установите CMake с официального сайта: https://cmake.org/download/
2. Установите vcpkg:
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
```
3. Установите зависимости через vcpkg:
```bash
vcpkg install glfw3:x64-windows
vcpkg install glew:x64-windows
```

## Сборка проекта

1. Клонируйте репозиторий:
```bash
git clone https://github.com/your-username/rotating-icosahedron.git
cd rotating-icosahedron
```

2. Создайте директорию для сборки:
```bash
mkdir build
cd build
```

3. Сгенерируйте файлы сборки и соберите проект:
```bash
cmake ..
make
./rotatingFig
```

## Управление

- ESC - выход из программы

## Лицензия

MIT License 