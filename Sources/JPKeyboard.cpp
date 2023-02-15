#include "JPKeyboard.hpp"
#include "Convert.hpp"
#include "KaniCodes.hpp"

namespace CTRPluginFramework
{
  JPKeyboard::JPKeyboard(std::string text)
  {
    _text = text;
    _maxLength = 16;
    _canSwich = true;
    _canAbort = true;
    _canConvert = true;
    _flick = true;
    selectedIndex = 0;
  }

  // https://gist.github.com/HidegonSan/e04334ed8d22691a9ac394a74ca1877f
  char *_u8slice(char *buf, const char *str, u32 begin, u32 end)
  {
    u32 count = 0;
    char *p = buf; // returnするポインタを格納
    while (*str != '\0')
    {
      if ((*str & 0xC0) != 0x80)
      {
        count++;
      }
      if (count > begin && count <= end)
      {
        *buf = *str;
        buf++;
      } // 指定範囲の文字を格納
      str++;
    }
    *buf = '\0'; // 末尾にNULL文字を格納
    return p;
  }

  // 正しく文字を切り出す
  std::string _string_subtract(const std::string &str, u32 start, u32 length)
  {
    char buf[128];
    char *sliced = _u8slice(buf, str.c_str(), start, start + length);
    std::string ret = sliced;
    return ret;
  }

  std::string _alphabet_to_hiragana(std::string input)
  {
    if (input.empty() || 5 < input.size())
      return "";

    transform(input.begin(), input.end(), input.begin(), tolower);
    std::string ret = "";

    // Thanks: https://support.microsoft.com/ja-jp/topic/%E3%83%AD%E3%83%BC%E3%83%9E%E5%AD%97%E5%85%A5%E5%8A%9B%E3%81%AE%E3%81%A4%E3%81%A5%E3%82%8A%E4%B8%80%E8%A6%A7%E8%A1%A8%E3%82%92%E7%A2%BA%E8%AA%8D%E3%81%97%E3%81%A6%E3%81%BF%E3%82%88%E3%81%86-bcc0ad7e-2781-cc9a-e524-7de506d8fdae

    static const std::vector<std::vector<std::string>> convert_table = {
        {"a", "あ"},
        {"i", "yi", "い"},
        {"u", "wu", "whu", "う"},
        {"e", "え"},
        {"o", "お"},
        {"wha", "うぁ"},
        {"whi", "wi", "うぃ"},
        {"whe", "we", "うぇ"},
        {"who", "うぉ"},
        {"la", "xa", "ぁ"},
        {"li", "xi", "lyi", "xyi", "ぃ"},
        {"lu", "xu", "ぅ"},
        {"le", "xe", "lye", "xye", "ぇ"},
        {"lo", "xo", "ぉ"},
        {"ye", "いぇ"},
        {"ka", "ca", "か"},
        {"ki", "き"},
        {"ku", "cu", "qu", "く"},
        {"ke", "け"},
        {"ko", "co", "こ"},
        {"kya", "きゃ"},
        {"kyi", "きぃ"},
        {"kyu", "きゅ"},
        {"kye", "きぇ"},
        {"kyo", "きょ"},
        {"qya", "くゃ"},
        {"qyu", "くゅ"},
        {"qyo", "くょ"},
        {"qwa", "wa", "kwa", "くぁ"},
        {"qui", "qi", "qyi", "くぃ"},
        {"qwu", "くぅ"},
        {"qwe", "qe", "qye", "くぇ"},
        {"qwo", "qo", "くぉ"},
        {"ga", "が"},
        {"gi", "ぎ"},
        {"gu", "ぐ"},
        {"ge", "げ"},
        {"go", "ご"},
        {"gya", "ぎゃ"},
        {"gyi", "ぎぃ"},
        {"gyu", "ぎゅ"},
        {"gye", "ぎぇ"},
        {"gyo", "ぎょ"},
        {"gwa", "ぐぁ"},
        {"gwi", "ぐぃ"},
        {"gwu", "ぐぅ"},
        {"gwe", "ぐぇ"},
        {"gwo", "ぐぉ"},
        {"lka", "xka", "ヵ"},
        {"lke", "xke", "ヶ"},
        {"sa", "さ"},
        {"si", "ci", "shi", "し"},
        {"su", "す"},
        {"se", "ce", "せ"},
        {"so", "そ"},
        {"sya", "sha", "しゃ"},
        {"syi", "しぃ"},
        {"syu", "shu", "しゅ"},
        {"sye", "she", "しぇ"},
        {"syo", "sho", "しょ"},
        {"swa", "すぁ"},
        {"swi", "すぃ"},
        {"swu", "すぅ"},
        {"swe", "すぇ"},
        {"swo", "すぉ"},
        {"za", "ざ"},
        {"zi", "ji", "じ"},
        {"zu", "ず"},
        {"ze", "ぜ"},
        {"zo", "ぞ"},
        {"zya", "ja", "jya", "じゃ"},
        {"zyi", "jyi", "じぃ"},
        {"zyu", "ju", "jyu", "じゅ"},
        {"zye", "je", "jye", "じぇ"},
        {"zyo", "jo", "jyo", "じょ"},
        {"ta", "た"},
        {"ti", "chi", "ち"},
        {"tu", "tsu", "つ"},
        {"te", "て"},
        {"to", "と"},
        {"tya", "cha", "cya", "ちゃ"},
        {"tyi", "cyi", "ちぃ"},
        {"tyu", "chu", "cyu", "ちゅ"},
        {"tye", "che", "cye", "ちぇ"},
        {"tyo", "cho", "cyo", "ちょ"},
        {"tsa", "つぁ"},
        {"tsi", "つぃ"},
        {"tse", "つぇ"},
        {"tso", "つぉ"},
        {"tha", "てゃ"},
        {"thi", "てぃ"},
        {"thu", "てゅ"},
        {"the", "てぇ"},
        {"tho", "てょ"},
        {"twa", "とぁ"},
        {"twi", "とぃ"},
        {"twu", "とぅ"},
        {"twe", "とぇ"},
        {"two", "とぉ"},
        {"da", "だ"},
        {"di", "ぢ"},
        {"du", "づ"},
        {"de", "で"},
        {"do", "ど"},
        {"dya", "ぢゃ"},
        {"dyi", "ぢぃ"},
        {"dyu", "ぢゅ"},
        {"dye", "ぢぇ"},
        {"dyo", "ぢょ"},
        {"dha", "でゃ"},
        {"dhi", "でぃ"},
        {"dhu", "でゅ"},
        {"dhe", "でぇ"},
        {"dho", "でょ"},
        {"dwa", "どぁ"},
        {"dwi", "どぃ"},
        {"dwu", "どぅ"},
        {"dwe", "どぇ"},
        {"dwo", "どぉ"},
        {"ltu", "xtu", "ltsu", "っ"},
        {"na", "な"},
        {"ni", "に"},
        {"nu", "ぬ"},
        {"ne", "ね"},
        {"no", "の"},
        {"nya", "にゃ"},
        {"nyi", "にぃ"},
        {"nyu", "にゅ"},
        {"nye", "にぇ"},
        {"nyo", "にょ"},
        {"ha", "は"},
        {"hi", "ひ"},
        {"hu", "fu", "ふ"},
        {"he", "へ"},
        {"ho", "ほ"},
        {"hya", "ひゃ"},
        {"hyi", "ひぃ"},
        {"hyu", "ひゅ"},
        {"hye", "ひぇ"},
        {"hyo", "ひょ"},
        {"fwa", "fa", "ふぁ"},
        {"fwi", "fi", "fyi", "ふぃ"},
        {"fwu", "ふぅ"},
        {"fwe", "fe", "fye", "ふぇ"},
        {"fwo", "fo", "ふぉ"},
        {"fya", "ふゃ"},
        {"fyu", "ふゅ"},
        {"fyo", "ふょ"},
        {"ba", "ば"},
        {"bi", "び"},
        {"bu", "ぶ"},
        {"be", "べ"},
        {"bo", "ぼ"},
        {"bya", "びゃ"},
        {"byi", "びぃ"},
        {"byu", "びゅ"},
        {"bye", "びぇ"},
        {"byo", "びょ"},
        {"va", "ゔぁ"},
        {"vi", "vyi", "ゔぃ"},
        {"vu", "ゔ"},
        {"ve", "vye", "ゔぇ"},
        {"vo", "ゔぉ"},
        {"vya", "ゔゃ"},
        {"vyu", "ゔゅ"},
        {"vyo", "ゔょ"},
        {"pa", "ぱ"},
        {"pi", "ぴ"},
        {"pu", "ぷ"},
        {"pe", "ぺ"},
        {"po", "ぽ"},
        {"pya", "ぴゃ"},
        {"pyi", "ぴぃ"},
        {"pyu", "ぴゅ"},
        {"pye", "ぴぇ"},
        {"pyo", "ぴょ"},
        {"ma", "ま"},
        {"mi", "み"},
        {"mu", "む"},
        {"me", "め"},
        {"mo", "も"},
        {"mya", "みゃ"},
        {"myi", "みぃ"},
        {"myu", "みゅ"},
        {"mye", "みぇ"},
        {"myo", "みょ"},
        {"ya", "や"},
        {"yu", "ゆ"},
        {"yo", "よ"},
        {"lya", "xya", "ゃ"},
        {"lyu", "xyu", "ゅ"},
        {"lyo", "xyo", "ょ"},
        {"ra", "ら"},
        {"ri", "り"},
        {"ru", "る"},
        {"re", "れ"},
        {"ro", "ろ"},
        {"rya", "りゃ"},
        {"ryi", "りぃ"},
        {"ryu", "りゅ"},
        {"rye", "りぇ"},
        {"ryo", "りょ"},
        {"wa", "わ"},
        {"wo", "を"},
        {"n", "nn", "xn", "ん"},
        {"lwa", "xwa", "ゎ"},
    };

    if (2 <= input.size())
    { // 同じ文字が続けられていたら "っ" を追加する
      if (input[0] == input[1] && (input[0] != 'n'))
      {
        ret += "っ";
        input = _string_subtract(input, 1, input.size() - 1);
      }
    }

    for (auto &&item : convert_table)
    { // ノーマル変換
      if (std::find(item.begin(), item.end() - 1, input) != item.end() - 1)
      {
        return ret + item[item.size() - 1];
      }
    }

    if (2 <= input.size() && input[0] == 'n')
    { // N の後に文字が続いていてかつその後に続いている文字が次の結果に影響しない (nu, nyaなどはダメ) 場合、 "ん" に変換する
      char ng_chars[] = {'a', 'i', 'u', 'e', 'o', 'y'};
      for (auto &&ng_char : ng_chars)
      {
        if (input[1] != ng_char && ng_char == 'y')
        {
          return "ん" + _string_subtract(input, 1, input.size() - 1);
        }
      }
    }

    return ret + input;
  }

  std::string jpQwertyOutput = "";
  void JPKeyboardEvent(Keyboard &keyboard, KeyboardEvent &event)
  {
    if (event.type == event.CharacterAdded || event.type == event.CharacterRemoved)
    {
      jpQwertyOutput = "";
      std::string str = keyboard.GetInput();
      while (1)
      {
        size_t len = 1;
        while (1)
        {
          if (len <= str.length())
          {
            if (len == 1 && 2 <= str.length() && str[0] == 'n' && std::string("aiueoy").find(str[1] != std::string::npos))
              len++;
            if (_alphabet_to_hiragana(str.substr(0, len)).back() == str.substr(0, len).back() && len < str.length())
            {
              if (len < 3)
                len++;
              else
              {
                len = 1;
                if (0 < str.length())
                {
                  jpQwertyOutput += str.substr(0, 1);
                  str = str.substr(1);
                }
                else
                  break;
              }
              continue;
            }
          }
          break;
        }
        if (len <= str.length())
        {
          jpQwertyOutput += _alphabet_to_hiragana(str.substr(0, len));
          str = str.substr(len);
        }
        else
          break;
      }
    }
    OSD::GetBottomScreen().DrawRect(20, 20, 280, 25, Color::Black);
    OSD::GetBottomScreen().DrawSysfont(jpQwertyOutput, 23, 23);
  }

  bool JPKeyboard::LoadKanjiList(void)
  {
    u32 count = 0;
    if (File::Exists("kanji.txt"))
    {
      File file("kanji.txt");
      file.Seek(2);
      bool flag = true;
      while (flag)
      {
        std::string hiragana, kanji;
        while (1)
        {
          if (file.GetSize() < count++)
          {
            OSD::Notify("failed load kanji.txt");
            return false;
          }
          u16 buff;
          std::string str_buff;
          file.Read((void *)&buff, sizeof(u16));
          Utils::ConvertUTF16ToUTF8(str_buff, (u16 *)&buff);
          if (buff != 0x2C)
          {
            if (buff == 0x3B)
            {
              flag = false;
              break;
            }
            else if ((buff > 0x80) || (buff < 0xA0))
              hiragana += str_buff.substr(0, 3);
          }
          else
            break;
        }
        while (1)
        {
          if (file.GetSize() < count++)
          {
            OSD::Notify("failed load kanji.txt");
            return false;
          }
          u16 buff;
          std::string str_buff;
          file.Read((void *)&buff, sizeof(u16));
          Utils::ConvertUTF16ToUTF8(str_buff, (u16 *)&buff);
          if (buff != 0xA)
          {
            if (buff == 0x3B)
            {
              flag = false;
              break;
            }
            else if (0x1000 < buff)
              kanji += str_buff.substr(0, 3);
            else
              kanji += str_buff.substr(0, 1);
          }
          else
            break;
        }
        hiragana_kanji.push_back({hiragana, kanji});
      }
      OSD::Notify("kanji.txt loaded");
      return true;
    }
    else
    {
      OSD::Notify("kanji.txt not found.");
      return false;
    }
  }

  void JPKeyboard::SetMaxLength(u32 max)
  {
    _maxLength = max;
  }

  void JPKeyboard::CanSwich(bool canSwich)
  {
    _canSwich = canSwich;
  }
  void JPKeyboard::CanAbort(bool canAbort)
  {
    _canAbort = canAbort;
  }
  void JPKeyboard::CanConvert(bool canConvert)
  {
    _canConvert = canConvert;
  }

  void JPKeyboard::MakeU16Array()
  {
    Process::WriteString((u32)U16_ChrArray, _flick ? KatakanaMode ? FlickKatakana : FlickHiragana : (KatakanaMode ? Katakana : Hiragana), StringFormat::Utf16);
  }

  void JPKeyboard::Komoji(u16 &moji)
  {
    const std::string str1 = "あいうえおアイウエオわワカつツやゆよヤユヨケ";
    const std::string str2 = "ぁぃぅぇぉァィゥェォゎヮヵっッゃゅょャュョヶ";

    // UTF16文字配列
    u16 u16array1[100] = {0}; // 変換前
    u16 u16array2[100] = {0}; // 変換後

    Process::WriteString((u32)u16array1, str1, StringFormat::Utf16);
    Process::WriteString((u32)u16array2, str2, StringFormat::Utf16);
    for (int i = 0; i < 23; i++)
    {
      if (moji == u16array1[i])
        moji = u16array2[i];
      else if (moji == u16array2[i])
        moji = u16array1[i];
    }
  }

  void JPKeyboard::Dakuten(bool handakuten, u16 &moji)
  {

    // 濁点用
    const std::string str1 = "かきくけこさしすせそたちつてとはひふへほカキクケコサシスセソタチツテトハヒフヘホウゝヽ";
    const std::string str2 = "がぎぐげござじずぜぞだぢづでどばびぶべぼガギグゲゴザジズゼゾダヂヅデドバビブベボヴゞヾ";

    // 半濁点用
    const std::string str3 = "はひふへほ";
    const std::string str4 = "ぱぴぷぺぽ";

    // UTF16文字配列
    u16 u16array1[100] = {0}; // 変換前
    u16 u16array2[100] = {0}; // 変換後

    if (handakuten) // 半濁点モード
    {
      Process::WriteString((u32)u16array1, str3, StringFormat::Utf16);
      Process::WriteString((u32)u16array2, str4, StringFormat::Utf16);
    }
    else // 濁点モード
    {
      Process::WriteString((u32)u16array1, str1, StringFormat::Utf16);
      Process::WriteString((u32)u16array2, str2, StringFormat::Utf16);
    }

    // 変換
    for (int i = 0; i < (handakuten ? 5 : 50); i++)
    {
      // ヒットしたら書き換えて終了
      if (u16array1[i] == moji)
        moji = u16array2[i];
      else if (u16array2[i] == moji)
        moji = u16array1[i];
    }
  }

  bool JPKeyboard::DrawKeyboard(const Screen &scr, std::string &out)
  {
    MakeU16Array();

    // 背景
    scr.DrawRect(20, 20, 280, 200, Color::Black);
    scr.DrawRect(22, 22, 276, 196, Color::White, false);

    // キー描画
    std::string key;
    if (_flick)
      for (u8 x = 0; x < 3; x++)
        for (u8 y = 0; y < 4; y++)
        {
          u16 sx = 120 + x * 30;
          u16 sy = 70 + y * 30;
          key.clear();
          Process::ReadString((u32)&U16_ChrArray[(y * 3 + x) * 5], key, 2, StringFormat::Utf16);
          scr.DrawSysfont(key, sx + 5, sy + 3, Color::White);
        }
    else
      for (u8 x = 0; x < 10; x++)
        for (u8 y = 0; y < 5; y++)
        {
          u32 sx = 23 + x * 24;
          u32 sy = 68 + y * 22;

          // 文字
          key.clear();
          Process::ReadString((u32)&U16_ChrArray[y * 10 + x], key, 2, StringFormat::Utf16);
          scr.DrawSysfont(key, sx + 5, sy + 3, Color::White);
        }

    // 入力中の文字列描画
    scr.DrawRect(55, 56, 212, 1, Color::White);
    if (!InputChrs.empty())
    {
      InputStr.clear();
      u16 str[100] = {0};
      for (size_t i = 0; i < InputChrs.size(); i++)
      {
        str[i] = InputChrs.at(i);
      }
      Process::ReadString((u32)str, InputStr, InputChrs.size() * 2, StringFormat::Utf16);
      u8 i = 0;
      u16 width = OSD::GetTextWidth(true, InputStr);
      for (auto &&InputChr : InputChrs)
      {
        if (width <= 208)
          break;

        InputChr < 0x1000 ? i++ : i += 3;

        width = OSD::GetTextWidth(true, InputStr.substr(i, InputStr.length() - i));
      }
      scr.DrawRect(58 + width - selectedIndex * 13, 37, selectedIndex * 13, 17, Color::Blue);
      scr.DrawSysfont(InputStr.substr(i, InputStr.length() - i), 58, 38);
    }

    // 右側のオプション
    const std::string opt[5] = {"削除", "小字", "ー", "”", "゜"};
    for (int i = 0; i < 5; i++)
    {
      if (Controller::IsKeyPressed(Touchpad) && TouchRect(268, 72 + i * 22, 24, 16))
      {
        selectedIndex = 0;
        switch (i)
        {
        case DELETE:
          if (!InputChrs.empty())
            InputChrs.pop_back();
          break;
        case KOMOJI:
          if (!InputChrs.empty())
            Komoji(InputChrs.at(InputChrs.size() - 1));
          break;
        case HYPHEN:
          if (InputChrs.size() < _maxLength)
            InputChrs.emplace_back(0x30FC);
          break;
        case DAKUTEN:
          if (!InputChrs.empty())
            Dakuten(false, InputChrs.at(InputChrs.size() - 1));
          break;
        case HANDAKUTEN:
          if (!InputChrs.empty())
            Dakuten(true, InputChrs.at(InputChrs.size() - 1));
          break;
        }
        scr.DrawRect(263, 68 + i * 22, 34, 22, Color::White);
      }
      scr.DrawSysfont(opt[i], 268, 72 + i * 22);
    }

    // 選択
    if (_canConvert)
    {
      scr.DrawSysfont("<", 35, 35);
      scr.DrawSysfont(">", 277, 35);
      if (Controller::IsKeyPressed(Touchpad) && TouchRect(32, 32, 24, 22))
      {
        scr.DrawRect(32, 35, 17, 17, Color::White);
        std::vector<u16> lastN(InputChrs.end() - selectedIndex - 1, InputChrs.end());
        if (lastN[0] > 0x1000)
        {
          selectedIndex++;
        }
      }
      if (Controller::IsKeyPressed(Touchpad) && TouchRect(274, 32, 24, 22))
      {
        scr.DrawRect(274, 35, 17, 17, Color::White);
        if (selectedIndex != 0)
        {
          selectedIndex--;
        }
      }

      if (Controller::IsKeyPressed(Key::Y) && selectedIndex != 0)
      {
        std::string kanji = Convert::hiraganaToKanji(InputStr.substr(InputStr.length() - selectedIndex * 3, selectedIndex * 3));
        if (!kanji.empty())
        {
          for (int j = 0; j < selectedIndex; j++)
            InputChrs.pop_back();
          if (InputChrs.size() < _maxLength)
          {
            u8 k = 0, i = 0;
            u16 buff_utf16[100] = {0};
            Process::WriteString((u32)buff_utf16, kanji, StringFormat::Utf16);
            i = 0;
            selectedIndex = 0;
            while (1)
            {
              if (buff_utf16[i] == 0)
                break;
              if (buff_utf16[i] < 0x1000)
                selectedIndex = 0;
              else
                selectedIndex++;
              i++;
            }
            for (int j = 0; j < i; j++)
            {
              InputChrs.emplace_back(buff_utf16[j]);
            }
          }
        }
      }
    }

    // とじる
    scr.DrawRect(28, 191, 68, 22, _canAbort ? Color::Gray : Color::BlackGrey);
    scr.DrawRect(28, 191, 68, 22, Color::White, false);
    scr.DrawSysfont("とじる", 42, 194);
    if ((Controller::IsKeyPressed(B) || TouchRect(28, 191, 68, 22)) && _canAbort)
    {
      KeyboardOpened = false;
      return 0;
    }

    // モード変換
    scr.DrawRect(126, 191, 68, 22, _canSwich ? Color::Gray : Color::BlackGrey);
    scr.DrawRect(126, 191, 68, 22, Color::White, false);
    scr.DrawSysfont("レイアウト", 126, 194);
    if (Controller::IsKeyPressed(Touchpad) && TouchRect(126, 191, 68, 22) && _canSwich)
    {
      if (KatakanaMode)
      {
        std::string input;
        if (0 <= Keyboard("ローマ字").Open(input))
          for (size_t i = 0; i < input.length(); i++)
          {
            if (InputChrs.size() > _maxLength)
              break;
            u16 buff;
            Process::WriteString((u32)&buff, input.substr(i, 1), StringFormat::Utf16);
            InputChrs.emplace_back(buff);
            selectedIndex = 0;
          }
        jpQwertyOutput = "";
        Keyboard key("日本語");
        key.OnKeyboardEvent(JPKeyboardEvent);
        if (0 <= key.Open(input))
        {
          Process::WriteString((u32)U16_ChrArray, jpQwertyOutput.substr(0, (_maxLength < 60 ? _maxLength : 60)), StringFormat::Utf16);
          for (int i = 0; i < Convert::getMultiByte(jpQwertyOutput.substr(0, (_maxLength < 60 ? _maxLength : 60))); i++)
          {
            InputChrs.emplace_back(U16_ChrArray[i]);
          }
          selectedIndex = 0;
        }
        _flick = !_flick;
      }
      KatakanaMode = !KatakanaMode;
    }

    // 決定
    scr.DrawRect(224, 191, 68, 22, Color::Gray);
    scr.DrawRect(224, 191, 68, 22, Color::White, false);
    scr.DrawSysfont("決定", 246, 194);
    if (TouchRect(224, 191, 68, 22))
    {
      if (!InputStr.empty())
      {
        out = InputStr;
        KeyboardOpened = false;
        return 1;
      }
    }

    // 入力
    if (Controller::IsKeyPressed(Touchpad))
    {
      UIntVector pos = Touch::GetPosition();
      selectedIndex = 0;
      if (_flick)
      {
        if (pos.x >= 120 && pos.y >= 70 && pos.x <= 200 && pos.y <= 190)
        {
          int wx = pos.x - 121;
          int wy = pos.y - 71;
          wx /= 30;
          wy /= 30;
          UIntVector pos2;
          u8 i = 0;
          while (Touch::IsDown())
          {
            pos2 = Touch::GetPosition();
            Controller::Update();
            u16 sx = 120 + wx * 30;
            u16 sy = 70 + wy * 30;
            int posX = pos.x - pos2.x, posY = pos.y - pos2.y;
            if (posX > 10)
            {
              i = 1;
              scr.DrawRect(120 + wx * 30 - 18, 70 + wy * 30 + 3, 20, 20, Color::White);
              key.clear();
              Process::ReadString((u32)&U16_ChrArray[(wy * 3 + wx) * 5 + 1], key, 2, StringFormat::Utf16);
              scr.DrawSysfont(key, sx - 15, sy + 3, Color::Black);
            }
            else if (posY > 10)
            {
              i = 2;
              scr.DrawRect(120 + wx * 30 + 2, 70 + wy * 30 - 17, 20, 20, Color::White);
              key.clear();
              Process::ReadString((u32)&U16_ChrArray[(wy * 3 + wx) * 5 + 2], key, 2, StringFormat::Utf16);
              scr.DrawSysfont(key, sx + 5, sy - 17, Color::Black);
            }
            else if (posX < -10)
            {
              i = 3;
              scr.DrawRect(120 + wx * 30 + 22, 70 + wy * 30 + 3, 20, 20, Color::White);
              key.clear();
              Process::ReadString((u32)&U16_ChrArray[(wy * 3 + wx) * 5 + 3], key, 2, StringFormat::Utf16);
              scr.DrawSysfont(key, sx + 25, sy + 3, Color::Black);
            }
            else if (posY < -10)
            {
              i = 4;
              scr.DrawRect(120 + wx * 30 + 2, 70 + wy * 30 + 23, 20, 20, Color::White);
              key.clear();
              Process::ReadString((u32)&U16_ChrArray[(wy * 3 + wx) * 5 + 4], key, 2, StringFormat::Utf16);
              scr.DrawSysfont(key, sx + 5, sy + 23, Color::Black);
            }
            else
            {
              i = 0;
              scr.DrawRect(120 + wx * 30 + 2, 70 + wy * 30 + 3, 20, 20, Color::White);
              key.clear();
              Process::ReadString((u32)&U16_ChrArray[(wy * 3 + wx) * 5], key, 2, StringFormat::Utf16);
              scr.DrawSysfont(key, sx + 5, sy + 3, Color::Black);
              scr.DrawRect(120 + wx * 30 - 18, 70 + wy * 30 + 3, 20, 20, Color::White);
              key.clear();
              Process::ReadString((u32)&U16_ChrArray[(wy * 3 + wx) * 5 + 1], key, 2, StringFormat::Utf16);
              scr.DrawSysfont(key, sx - 15, sy + 3, Color::Black);
              scr.DrawRect(120 + wx * 30 + 2, 70 + wy * 30 - 17, 20, 20, Color::White);
              key.clear();
              Process::ReadString((u32)&U16_ChrArray[(wy * 3 + wx) * 5 + 2], key, 2, StringFormat::Utf16);
              scr.DrawSysfont(key, sx + 5, sy - 17, Color::Black);
              scr.DrawRect(120 + wx * 30 + 22, 70 + wy * 30 + 3, 20, 20, Color::White);
              key.clear();
              Process::ReadString((u32)&U16_ChrArray[(wy * 3 + wx) * 5 + 3], key, 2, StringFormat::Utf16);
              scr.DrawSysfont(key, sx + 25, sy + 3, Color::Black);
              scr.DrawRect(120 + wx * 30 + 2, 70 + wy * 30 + 23, 20, 20, Color::White);
              key.clear();
              Process::ReadString((u32)&U16_ChrArray[(wy * 3 + wx) * 5 + 4], key, 2, StringFormat::Utf16);
              scr.DrawSysfont(key, sx + 5, sy + 23, Color::Black);
            }
            OSD::SwapBuffers();
            DrawKeyboard(scr, out);
          }
          if (!InputChrs.empty())
          {
            if (U16_ChrArray[(wy * 3 + wx) * 5 + i] == 0x309B)
              Dakuten(false, InputChrs.at(InputChrs.size() - 1));
            else if (U16_ChrArray[(wy * 3 + wx) * 5 + i] == 0x5C0F)
              Komoji(InputChrs.at(InputChrs.size() - 1));
            else if (U16_ChrArray[(wy * 3 + wx) * 5 + i] == 0x309C)
              Dakuten(true, InputChrs.at(InputChrs.size() - 1));
            else if (U16_ChrArray[(wy * 3 + wx) * 5 + i] == 0x5927)
              Komoji(InputChrs.at(InputChrs.size() - 1));
          }
          if (InputChrs.size() < _maxLength && (U16_ChrArray[(wy * 3 + wx) * 5 + i] != 0x309B && U16_ChrArray[(wy * 3 + wx) * 5 + i] != 0x5C0F && U16_ChrArray[(wy * 3 + wx) * 5 + i] != 0x309C && U16_ChrArray[(wy * 3 + wx) * 5 + i] != 0x5927))
            InputChrs.emplace_back(U16_ChrArray[(wy * 3 + wx) * 5 + i]);
        }
      }
      else if (pos.x >= 23 && pos.y >= 69 && pos.x <= 262 && pos.y <= 178)
      {
        int wx = pos.x - 23;
        int wy = pos.y - 69;
        wx /= 24;
        wy /= 22;
        scr.DrawRect(23 + wx * 24, 68 + wy * 22, 24, 22, Color::White);

        if (InputChrs.size() < _maxLength)
          InputChrs.emplace_back(U16_ChrArray[wy * 10 + wx]);
      }
    }

    return 1;
  }

  bool JPKeyboard::Open(std::string &out)
  {
    if (!Process::IsPaused())
      return 0;

    InputChrs.clear();
    InputStr.clear();

    const Screen &topScr = OSD::GetTopScreen();
    const Screen &scr = OSD::GetBottomScreen();

    KeyboardOpened = true;
    KatakanaMode = false;
    while (KeyboardOpened)
    {
      topScr.DrawRect(30, 20, 340, 200, Color::Black);
      topScr.DrawRect(32, 22, 336, 196, Color::White, false);
      topScr.DrawSysfont(_text, 35, 25);
      Controller::Update();
      if (!DrawKeyboard(scr, out))
        return 0;
      OSD::SwapBuffers();
    }
    return 1;
  }

  bool JPKeyboard::Open(std::string &out, std::string defaultText)
  {
    if (!Process::IsPaused())
      return 0;

    InputChrs.clear();
    InputStr.clear();

    Process::WriteString((u32)U16_ChrArray, defaultText.substr(0, (_maxLength < 60 ? _maxLength : 60)), StringFormat::Utf16);
    for (int i = 0; i < Convert::getMultiByte(defaultText.substr(0, (_maxLength < 60 ? _maxLength : 60))); i++)
    {
      InputChrs.emplace_back(U16_ChrArray[i]);
    }

    const Screen &topScr = OSD::GetTopScreen();
    const Screen &scr = OSD::GetBottomScreen();

    KeyboardOpened = true;
    KatakanaMode = false;
    while (KeyboardOpened)
    {
      topScr.DrawRect(30, 20, 340, 200, Color::Black);
      topScr.DrawRect(32, 22, 336, 196, Color::White, false);
      topScr.DrawSysfont(_text, 35, 25);
      Controller::Update();
      if (!DrawKeyboard(scr, out))
        return 0;
      OSD::SwapBuffers();
    }
    return 1;
  }
}