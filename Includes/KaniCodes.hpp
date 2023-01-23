#pragma once

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
    static std::vector<u16> strToSjis(std::string str);
    static int getMultiByte(std::string str);
    static std::string toLower(std::string str);
    static std::string hiraganaToKanji(std::string hiragana);
    static std::string hiraganaToKatakana(std::string hiragana);
    static std::string katakanaToHiragana(std::string katakana);
  };

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
    bool Open(std::string &out,std::string defaultText);

  private:
    enum
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
    void Komoji(u16 &moji);
    void Dakuten(bool handakuten, u16 &moji);
    bool DrawKeyboard(const Screen &scr, std::string &out);
  };

    bool TouchCircle(u32 x, u32 y, u8 size);
    bool TouchRect(u32 x, u32 y, u32 w, u32 h);

    void setFlagShowScreenBuffer(bool flag);
    void setScreenBuffer(u16 x, u16 y, Color color);
    void fillScreenBuffer(Color color);
    bool ShowScreenBuffer(const Screen &screen);

    std::string ReadSJIS(u32 Address);

    bool colorPicker(Color &out);
}
