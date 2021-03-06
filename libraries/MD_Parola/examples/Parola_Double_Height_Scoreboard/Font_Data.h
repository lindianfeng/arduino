// Data file for numeric scoreboard fonts
#pragma once

MD_MAX72XX::fontType_t NumFontLower[] PROGMEM = 
{
  'F', 1, 48, 57, 8,
  10, 0, 127, 255, 192, 192, 192, 192, 255, 127, 0, // 48 - '0'
  10, 0, 0, 0, 0, 0, 0, 0, 127, 255, 0,             // 49 - '1'
  10, 0, 127, 255, 193, 193, 193, 193, 193, 128, 0, // 50 - '2'
  10, 0, 128, 193, 193, 193, 193, 193, 255, 126, 0, // 51 - '3'
  10, 0, 0, 1, 1, 1, 1, 1, 127, 255, 0,             // 52 - '4'
  10, 0, 128, 193, 193, 193, 193, 193, 255, 126, 0, // 53 - '5'
  10, 0, 127, 255, 193, 193, 193, 193, 255, 127, 0, // 54 - '6'
  10, 0, 0, 0, 0, 0, 0, 0, 127, 255, 0,             // 55 - '7'
  10, 0, 126, 255, 193, 193, 193, 193, 255, 126, 0, // 56 - '8'
  10, 0, 128, 193, 193, 193, 193, 193, 255, 127, 0, // 57 - '9'
};

MD_MAX72XX::fontType_t NumFontUpper[] PROGMEM = 
{
  'F', 1, 48, 57, 8,
  10, 0, 254, 255, 3, 3, 3, 3, 255, 254, 0,         // 48 - '0'
  10, 0, 0, 0, 0, 0, 0, 0, 254, 255, 0,             // 49 - '1'
  10, 0, 1, 131, 131, 131, 131, 131, 255, 254, 0,   // 50 - '2'
  10, 0, 1, 131, 131, 131, 131, 131, 255, 126, 0,   // 51 - '3'
  10, 0, 255, 254, 128, 128, 128, 128, 254, 255, 0, // 52 - '4'
  10, 0, 254, 255, 131, 131, 131, 131, 131, 1, 0,   // 53 -'5'
  10, 0, 254, 255, 131, 131, 131, 131, 131, 1, 0,   // 54 - '6'
  10, 0, 1, 3, 3, 3, 3, 3, 255, 254, 0,             // 55 - '7'
  10, 0, 126, 255, 131, 131, 131, 131, 255, 126, 0, // 56 - '8'
  10, 0, 254, 255, 131, 131, 131, 131, 255, 254, 0, // 57 - '9'
};

