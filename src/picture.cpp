
#include "../include/picture.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <vector>

Picture::Picture() {
  _width = 0;
  _height = 0;
}

Picture::Picture(int width, int height, rgbColor color)
    : _values(width * height * 4) {
  _width = width;
  _height = height;
  const int red = color.r;
  const int green = color.g;
  const int blue = color.b;

  for (size_t k = 0; k < _values.size(); k += 4) {
    _values[k] = red;
    _values[k + 1] = green;
    _values[k + 2] = blue;
    _values[k + 3] = 255;
  }
}

Picture::Picture(const std::vector<std::vector<int>> &grays) {
  if (grays.size() == 0 || grays[0].size() == 0) {
    _width = 0;
    _height = 0;
  } else {
    _values = std::vector<unsigned char>(grays[0].size() * grays.size() * 4);
    _width = grays[0].size();
    _height = grays.size();
    int k = 0;
    for (int y = 0; y < _height; y++)
      for (int x = 0; x < _width; x++) {
        int gray = grays[y][x];
        _values[k] = gray;
        _values[k + 1] = gray;
        _values[k + 2] = gray;
        _values[k + 3] = 255;
        k += 4;
      }
  }
}

Picture::Picture(std::string filename) {
  unsigned int w, h;
  unsigned error = lodepng::decode(_values, w, h, filename.c_str());
  if (error != 0)
    throw std::runtime_error(lodepng_error_text(error));
  _width = w;
  _height = h;
}

void Picture::save(std::string filename) const {
  unsigned error = lodepng::encode(filename.c_str(), _values, _width, _height);
  if (error != 0)
    throw std::runtime_error(lodepng_error_text(error));
}

int Picture::red(int x, int y) const {
  if (0 <= x && x < _width && 0 <= y && y < _height)
    return _values[4 * (y * _width + x)];
  else
    return 0;
}

int Picture::green(int x, int y) const {
  if (0 <= x && x < _width && 0 <= y && y < _height)
    return _values[4 * (y * _width + x) + 1];
  else
    return 0;
}

int Picture::blue(int x, int y) const {
  if (0 <= x && x < _width && 0 <= y && y < _height)
    return _values[4 * (y * _width + x) + 2];
  else
    return 0;
}

void Picture::set(int x, int y, rgbColor color) {
  const int red = color.r;
  const int green = color.g;
  const int blue = color.b;

  if (x >= 0 && y >= 0) {
    ensure(x, y);
    int k = 4 * (y * _width + x);
    _values[k] = red;
    _values[k + 1] = green;
    _values[k + 2] = blue;
    _values[k + 3] = 255;
  }
}

void Picture::add(const Picture &other, int x, int y) {
  ensure(x + other._width - 1, y + other._height - 1);
  int k = 0;
  for (int dy = 0; dy < other._height; dy++)
    for (int dx = 0; dx < other._width; dx++) {
      set(x + dx, y + dy,
          {other._values[k], other._values[k + 1], other._values[k + 2]});
      k += 4;
    }
}

std::vector<std::vector<int>> Picture::grays() const {
  std::vector<std::vector<int>> result(_height);
  for (int y = 0; y < _height; y++) {
    result[y] = std::vector<int>(_width);
    for (int x = 0; x < _width; x++) {
      int k = 4 * (y * _width + x);
      result[y][x] = (int)(0.2126 * _values[k] + 0.7152 * _values[k + 1] +
                           0.0722 * _values[k + 2]);
    }
  }
  return result;
}

void Picture::scale(const size_t factor) {
  const size_t channels = 4;

  const size_t height = static_cast<size_t>(_height);
  const size_t width = static_cast<size_t>(_width);

  const size_t dstWidth = _width * factor * channels;
  const size_t dstHeight = _height * factor * channels;

  std::vector<unsigned char> newValues(_values.size() * factor * factor);

  for (size_t j = 0; j < height; j++) {
    const size_t srcRowIdx = j * width * channels;
    const size_t dstRowIdx = j * factor * dstWidth;

    // Create a temporary row that we can copy multiple times
    std::vector<unsigned char> tempRow(dstWidth);

    // Build first row 'manually'
    for (size_t i = 0; i < width; i++) {
      const size_t srcPixelIdx = srcRowIdx + i * channels;
      const size_t dstPixelIdx = i * factor * channels;

      // Copy the pixel "factor" times
      for (size_t k = 0; k < factor; k++) {
        std::memcpy(tempRow.data() + dstPixelIdx + k * channels,
                    _values.data() + srcPixelIdx, channels);
      }
    }

    // Copy the temporary row "factor" times
    for (size_t x = 0; x < factor; x++) {
      std::memcpy(newValues.data() + dstRowIdx + x * dstWidth, tempRow.data(),
                  tempRow.size());
    }
  }

  _width = dstWidth / channels;
  _height = dstHeight / channels;
  _values.swap(newValues);
}

/**
   Ensures that the given point exists.
 */
void Picture::ensure(int x, int y) {
  if (x >= _width || y >= _height) {
    int new_width = std::max(x + 1, _width);
    int new_height = std::max(y + 1, _height);
    std::vector<unsigned char> new_values(4 * new_width * new_height);
    fill(new_values.begin(), new_values.end(), 255); // fill with white
    int j = 0;
    for (int dy = 0; dy < _height; dy++)
      for (int dx = 0; dx < _width; dx++)
        for (int k = 0; k < 4; k++, j++)
          new_values[4 * (dy * new_width + dx) + k] = _values[j];
    _values.swap(new_values);
    _width = new_width;
    _height = new_height;
  }
}
