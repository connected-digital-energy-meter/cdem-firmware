#include "color.h"
#include <algorithm>    // std::max, std::min

namespace SmartMeter {

  Color::Color()
    : Color(0, 0, 0) {
  }

  Color::Color(int red, int green, int blue) {
    this->red(red);
    this->green(green);
    this->blue(blue);
  }

  int Color::red(void) {
    return _red;
  }

  int Color::green(void) {
    return _green;
  }

  int Color::blue(void) {
    return _blue;
  }

  Color Color::dim(int percentage) {
    return Color(
      percentage*(this->_red)/100,
      percentage*(this->_green)/100,
      percentage*(this->_blue)/100
    );
  }

  void Color::red(int value) {
    _red = std::max(0, std::min(255, value));
  }

  void Color::green(int value) {
    _green = std::max(0, std::min(255, value));
  }

  void Color::blue(int value) {
    _blue = std::max(0, std::min(255, value));
  }

  Color Color::RED() { return Color(255, 0, 0); }
  Color Color::GREEN() { return Color(0, 255, 0); }
  Color Color::BLUE() { return Color(0, 0, 255); }
  Color Color::WHITE() { return Color(255, 255, 255); }
  Color Color::BLACK() { return Color(0, 0, 0); }
  Color Color::ORANGE() { return Color(255, 40, 0); }
  Color Color::YELLOW() { return Color(255, 255, 0); }
  Color Color::PURPLE() { return Color(255, 0, 255); }

};