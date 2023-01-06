#ifndef KANICODES_HPP
#define KANICODES_HPP

#include <CTRPluginFramework.hpp>
#include <math.h>
#include "types.h"
#include "Helpers.hpp"

namespace CTRPluginFramework
{
  // osd_plus
  void DrawLine(const Screen &scr, int srcX, int srcY, int dstX, int dstY, const Color &color);
  void DrawPlus(const Screen &scr, const std::string &str, u32 posX, u32 posY, u32 borderWidth, u32 padding, const Color &foreground, const Color &background, const Color &border, int fontAlign);
  void DrawSysfontPlus(const Screen &scr, const std::string &str, u32 posX, u32 posY, u32 borderWidth, u32 padding, const Color &foreground, const Color &background, const Color &border, bool fillBackground, bool rightAligned = false, int fontAlign = 0);
  float DegreeToRadian(float degree);
  void DrawCircle(const Screen &scr, u32 x, u32 y, u32 radiusStart, u32 radiusEnd, int start, int end, const Color &color, int origin);
  void DrawRectPlus(const Screen &scr, u32 posX, u32 posY, u32 width, u32 height, const Color &color, bool filled, int origin);

  class Convert
  {
  public:
    static u16 sjisToUtf16(u16 sjis);
    static u16 utf16ToSjis(u16 utf16);
    static u16 strToSjis(std::string moji);
    static int getMultiByte(std::string str);
    static void SetHiraganaKanji(std::string hiragana, std::string kanji);
    static std::string toLower(std::string str);
    static std::string hiraganaToKanji(std::string hiragana);
    static std::string hiraganaToKatakana(std::string hiragana);
    static std::string katakanaToHiragana(std::string katakana);
  };

  void japKey(std::string &out, std::string text, std::vector<u8> *sjis = 0);
  bool TouchCircle(u32 x, u32 y, u8 size);
  bool TouchRect(u32 x, u32 y, u32 w, u32 h);
  
  void setFlagShowScreenBuffer(bool flag);
  void setScreenBuffer(u16 x, u16 y, Color color);
  void fillScreenBuffer(Color color);
  bool ShowScreenBuffer(const Screen &screen);

  std::string ReadSJIS(u32 Address);

  Result colorPicker(Color &out);
}

#endif