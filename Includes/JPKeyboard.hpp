#pragma once

#include <CTRPluginFramework.hpp>
#include "types.h"

namespace CTRPluginFramework
{
  class JPKeyboard
  {
  public:
    JPKeyboard(std::string text = "");

    static bool LoadKanjiList(void);

    void SetMaxLength(u32 max);
    void CanSwich(bool canSwich);
    void CanAbort(bool canAbort);
    void CanConvert(bool canConvert);
    bool Open(std::string &out);
    bool Open(std::string &out, std::string defaultText);

    static void Komoji(u16 &moji);

  private:
    enum StringOp
    {
      DELETE,
      KOMOJI,
      HYPHEN,
      DAKUTEN,
      HANDAKUTEN
    };
    std::string _text;
    u32 _maxLength;
    bool _canSwich;
    bool _canAbort;
    bool _canConvert;
    bool _flick;

    u16 U16_ChrArray[60 + 1];
    bool KatakanaMode;
    bool KeyboardOpened;
    u8 selectedIndex;
    std::vector<u16> InputChrs;
    std::string InputStr;
    const std::string Hiragana =
        "わらやまはなたさかあ"
        "をりゆみひにちしきい"
        "んるよむふぬつすくう"
        "、れ！めへねてせけえ"
        "。ろ？もほのとそこお";
    const std::string Katakana =
        "ワラヤマハナタサカア"
        "ヲリユミヒニチシキイ"
        "ンルヨムフヌツスクウ"
        "、レ！メヘネテセケエ"
        "。ロ？モホノトソコオ";
    const std::string FlickHiragana =
        "あいうえおかきくけこさしすせそ"
        "たちつてとなにぬねのはひふへほ"
        "まみむめもや（ゆ）よらりるれろ"
        "゛゛小゜大わをんー〜、。？！…";
    const std::string FlickKatakana =
        "アイウエオカキクケコサシスセソ"
        "タチツテトナニヌネノハヒフヘホ"
        "マミムメモヤ（ユ）ヨラリルレロ"
        "゛゛小゜大ワヲンー〜、。？！…";

    void MakeU16Array(void);
    void Dakuten(bool handakuten, u16 &moji);
    bool DrawKeyboard(const Screen &scr, std::string &out);
  };
}