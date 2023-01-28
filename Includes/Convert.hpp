#pragma once

#include <CTRPluginFramework.hpp>
#include "types.h"

namespace CTRPluginFramework
{
  struct HIRAGANA_KANJI
  {
    std::string hiragana;
    std::string kanji;
  };

  static std::vector<HIRAGANA_KANJI> hiragana_kanji;

  class Convert
  {
  public:
    static u16 sjisToUtf16(u16 sjis);
    static u16 utf16ToSjis(u16 utf16);
    static std::vector<u16> strToSjis(std::string str);
    static int getMultiByte(std::string str);
    static std::string toLower(std::string str);
    static std::string hiraganaToKanji(std::string hiragana);
    static std::string hiraganaToKatakana(std::string hiragana);
    static std::string katakanaToHiragana(std::string katakana);
  };
}