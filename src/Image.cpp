#include "Image.h"
#include "global.h"

#include <assert.h>

void Image::set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  assert(x >= 0 && x < w);
  assert(y >= 0 && y < h);
  size_t p = (x * COLOR_TYPE) + (y * w * COLOR_TYPE);
  colors[p] = r;
  colors[p+1] = g;
  colors[p+2] = b;
  colors[p+3] = a;
}

void Image::set_pixel(int x, int y, Color &q){
  set_pixel(x, y, q.r, q.g, q.b, q.a);
}

void Image::blend_pixel(int x, int y, Color &c){
  Color o;
  get_pixel(x, y, o);

  if (o.is_transparent()) {
    set_pixel(x, y, c);
    return;
  }
  
  o.blend(c);
  set_pixel(x, y, o);
}

void Image::get_pixel(int x, int y, Color &c){
  assert(x >= 0 && x < w);
  assert(y >= 0 && y < h);
  size_t p = (x * COLOR_TYPE) + (y * w * COLOR_TYPE);
  c.r = this->colors[p];
  c.g = this->colors[p+1];
  c.b = this->colors[p+2];
  c.a = this->colors[p+3];
}

int Image::get_width() {
  return w;
}

int Image::get_height() {
  return h;
}

void Image::composite(int xoffset, int yoffset, Image &img) {
  assert(xoffset >= 0);
  assert(xoffset + img.get_width() <= w);
  assert(yoffset >= 0);
  assert(yoffset + img.get_height() <= h);
  
  Color c, o;
  
  for (int x = 0; x < img.get_width(); x++) {
    for (int y = 0; y < img.get_height(); y++) {
      img.get_pixel(x, y, c);
      if (c.is_transparent()) continue;
      get_pixel(xoffset + x, yoffset + y, o);
      o.blend(c);
      set_pixel(xoffset + x, yoffset + y, o);
    }
  }
}


void ImageBuffer::set_pixel(int x, int y, int z, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  assert(x >= 0 && x < w);
  assert(y >= 0 && y < h);
  assert(z >= 0 && z < d);
  size_t p = (x * COLOR_TYPE) + (y * w * COLOR_TYPE) * (z * w * h * COLOR_TYPE);
  colors[p] = r;
  colors[p+1] = g;
  colors[p+2] = b;
  colors[p+3] = a;
}

void ImageBuffer::add_pixel(int x, int y, Color &c) {
  if (c.is_opaque()) set_height(x, y, 0);
  add_pixel(x, y, c.r, c.g, c.b, c.a);
}

void ImageBuffer::add_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  uint16_t h = get_height(x, y);
  set_pixel(x, y, h, r, g, b, a);
  set_height(x, y, h + 1);
}

void ImageBuffer::set_height(int x, int y, uint16_t h) {
  assert(x >= 0 && x < w);
  assert(y >= 0 && y < h);
  heights[x + (y * w)] = h;
}

uint16_t ImageBuffer::get_height(int x, int y) {
  assert(x >= 0 && x < w);
  assert(y >= 0 && y < h);
  return heights[x + (y * w)];
}
