#pragma once

namespace SmartMeter {
  class Color {

    public:
      Color();
      Color(int red, int green, int blue);

    public:
      int red(void);
      int green(void);
      int blue(void);

    public:
      Color dim(int percentage);

    public:
      static Color RED();
      static Color GREEN();
      static Color BLUE();
      static Color WHITE();
      static Color BLACK();
      static Color ORANGE();

    private:
      void red(int value);
      void green(int value);
      void blue(int value);

    private:
      int _red = 0;
      int _green = 0;
      int _blue = 0;
  };
};
