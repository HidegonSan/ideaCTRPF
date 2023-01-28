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
    Process::WriteString((u32)U16_ChrArray, _flick ? KatakanaMode ? FlickKatakana : FlickHiragana : KatakanaMode ? Katakana
                                                                                                                 : Hiragana,
                         StringFormat::Utf16);
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
        str[i] = InputChrs[i];
      }
      Process::ReadString((u32)str, InputStr, InputChrs.size() * 2, StringFormat::Utf16);
      u8 i = 0;
      u8 j = 0;
      u16 width = OSD::GetTextWidth(true, InputStr);
      while (1)
      {
        if (width <= 208)
          break;
        
        InputChrs[j] < 0x1000 ? i++ : i += 3;

        j++;
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
            Komoji(InputChrs[InputChrs.size() - 1]);
          break;
        case HYPHEN:
          if (InputChrs.size() < _maxLength)
            InputChrs.push_back(0x30FC);
          break;
        case DAKUTEN:
          if (!InputChrs.empty())
            Dakuten(false, InputChrs[InputChrs.size() - 1]);
          break;
        case HANDAKUTEN:
          if (!InputChrs.empty())
            Dakuten(true, InputChrs[InputChrs.size() - 1]);
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
              InputChrs.push_back(buff_utf16[j]);
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
        if (0 <= Keyboard("").Open(input))
          for (size_t i = 0; i < input.length(); i++)
          {
            if (InputChrs.size() > _maxLength)
              break;
            u16 buff;
            Process::WriteString((u32)&buff, input.substr(i, 1), StringFormat::Utf16);
            InputChrs.push_back(buff);
            selectedIndex = 0;
          }
        _flick = !_flick;
      }
      KatakanaMode = KatakanaMode ? false : true;
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
        if (pos.x >= 120 && pos.y >= 70 && pos.x <= 210 && pos.y <= 190)
        {
          int wx = pos.x - 120;
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
              Dakuten(false, InputChrs[InputChrs.size() - 1]);
            else if (U16_ChrArray[(wy * 3 + wx) * 5 + i] == 0x5C0F)
              Komoji(InputChrs[InputChrs.size() - 1]);
            else if (U16_ChrArray[(wy * 3 + wx) * 5 + i] == 0x309C)
              Dakuten(true, InputChrs[InputChrs.size() - 1]);
            else if (U16_ChrArray[(wy * 3 + wx) * 5 + i] == 0x5927)
              Komoji(InputChrs[InputChrs.size() - 1]);
          }
          if (InputChrs.size() < _maxLength && (U16_ChrArray[(wy * 3 + wx) * 5 + i] != 0x309B && U16_ChrArray[(wy * 3 + wx) * 5 + i] != 0x5C0F && U16_ChrArray[(wy * 3 + wx) * 5 + i] != 0x309C && U16_ChrArray[(wy * 3 + wx) * 5 + i] != 0x5927))
            InputChrs.push_back(U16_ChrArray[(wy * 3 + wx) * 5 + i]);
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
          InputChrs.push_back(U16_ChrArray[wy * 10 + wx]);
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

    Process::WriteString((u32)U16_ChrArray, defaultText, StringFormat::Utf16);
    for (int i = 0; i < Convert::getMultiByte(defaultText); i++)
    {
      InputChrs.push_back(U16_ChrArray[i]);
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