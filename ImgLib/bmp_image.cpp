#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>
#include <iostream>

using namespace std;

namespace img_lib {

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

PACKED_STRUCT_BEGIN BitmapFileHeader {
    // поля заголовка Bitmap File Header
    BitmapFileHeader(int width, int height) {
        bfSize = GetBMPStride(width) * height;
    }
    char bfType[2] = {'B', 'M'};
    uint32_t bfSize = {};
    uint32_t bfReserved = 0;
    uint32_t bfOffBits = 54; // равен размеру обоих частей заголовка - 54 байта
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    // поля заголовка Bitmap Info Header
    BitmapInfoHeader(int width, int height)
        : biWidth(width), biHeight(height) {
        biSizeImage = GetBMPStride(width) * height;
    }
    uint32_t biSize = 40; // размер второй части 40 байт
    int32_t biWidth = {};
    int32_t biHeight = {};
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 24;
    uint32_t biCompression = 0;
    uint32_t biSizeImage = {};
    int32_t biXPelsPerMeter = 11811;
    int32_t biYPelsPerMeter = 11811;
    int32_t biClrUsed = 0;
    int32_t biClrImportant = 0x1000000;
}
PACKED_STRUCT_END

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    ofstream out(file, ios::binary);
    if (!out) {
        cerr << "Failed to open file for writing: " << file << endl;
        return false;
    }

    int width = image.GetWidth();
    int height = image.GetHeight();
    int stride = GetBMPStride(width);

    BitmapFileHeader fileHeader(width, height);
    BitmapInfoHeader infoHeader(width, height);

    // Корректируем размер файла, включая размер заголовков
    fileHeader.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + stride * height;

    // Запись заголовков в файл
    out.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    out.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));

    // Буфер для хранения строки пикселей
    vector<char> buffer(stride);

    // Запись пикселей в файл
    for (int y = height - 1; y >= 0; --y) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < width; ++x) {
            buffer[x * 3 + 0] = static_cast<char>(line[x].b);
            buffer[x * 3 + 1] = static_cast<char>(line[x].g);
            buffer[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        // Заполнение оставшихся байтов в строке нулями
        for (int i = width * 3; i < stride; ++i) {
            buffer[i] = 0;
        }
        out.write(buffer.data(), stride);
    }

    if (!out) {
        cerr << "Failed to write to file: " << file << endl;
        return false;
    }

    return true;
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    ifstream ifs(file, ios::binary);
    if (!ifs) {
        cerr << "Failed to open file for reading: " << file << endl;
        return {};
    }

    // Считываем заголовок BitmapFileHeader
    BitmapFileHeader fileHeader(0, 0);
    ifs.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (!ifs || fileHeader.bfType[0] != 'B' || fileHeader.bfType[1] != 'M') {
        cerr << "Invalid BMP file header: " << file << endl;
        return {};
    }

    // Считываем заголовок BitmapInfoHeader
    BitmapInfoHeader infoHeader(0, 0);
    ifs.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    if (!ifs || infoHeader.biSize != 40 || infoHeader.biBitCount != 24) {
        cerr << "Unsupported BMP format: " << file << endl;
        return {};
    }

    int w = infoHeader.biWidth;
    int h = abs(infoHeader.biHeight); // Высота может быть отрицательной

    int stride = GetBMPStride(w);
    Image result(w, h, Color::Black());
    std::vector<char> buff(stride);

    // Переходим к данным пикселей
    ifs.seekg(fileHeader.bfOffBits, ios::beg);

    for (int y = h - 1; y >= 0; --y) {
        Color* line = result.GetLine(y);
        ifs.read(buff.data(), stride);

        for (int x = 0; x < w; ++x) {
            line[x].b = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }

    if (!ifs) {
        cerr << "Failed to read from file: " << file << endl;
        return {};
    }

    return result;
}
}  // namespace img_lib
