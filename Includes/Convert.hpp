#pragma once

#include <CTRPluginFramework.hpp>
#include "types.h"

namespace CTRPluginFramework
{

  class Convert
  {
  public:
    static u16 sjisToUtf16(const u16 &sjis);
    static u16 utf16ToSjis(const u16 &utf16);
    static std::vector<u16> strToSjis(const std::string &str);
    static int getMultiByte(const std::string &str);
    static std::string toLower(std::string str);
    static std::string hiraganaToKanji(std::string hiragana);
    static std::string hiraganaToKatakana(const std::string &hiragana);
    static std::string katakanaToHiragana(const std::string &katakana);
    static void initHiraganaKanjiList(void);
    static void addHiraganaKanjiList(const std::string &hiragana, const std::string &kanji);

  private:
    struct HIRAGANA_KANJI
    {
      std::string hiragana;
      std::string kanji;
    };

    static std::vector<HIRAGANA_KANJI> hiragana_kanji_list;
  };
}