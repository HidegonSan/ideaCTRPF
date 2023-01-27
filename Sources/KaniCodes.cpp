#include "KaniCodes.hpp"
#include "Convert.hpp"
#include <locale.h>
#include <stdlib.h>

namespace CTRPluginFramework
{
  /*****************************************************/
  /*                                                   */
  /*             Generated by OSD Designer             */
  /*                  Made by Hidegon                  */
  /*  OSD Designer: https://HidegonSan.github.io/OSD/  */
  /*                                                   */
  /*****************************************************/
  void DrawLine(const Screen &scr, int srcX, int srcY, int dstX, int dstY, const Color &color)
  {
    float x, y, dx, dy, step;
    int i;

    dx = dstX - srcX;
    dy = dstY - srcY;

    step = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

    dx /= step;
    dy /= step;
    x = srcX;
    y = srcY;
    i = 1;

    while (i <= step)
    {
      scr.DrawPixel(x, y, color);
      x += dx;
      y += dy;
      i++;
    }
  }

  void DrawPlus(const Screen &scr, const std::string &str, u32 posX, u32 posY, u32 borderWidth, u32 padding, const Color &foreground, const Color &background, const Color &border, int fontAlign)
  {
    int bgWidth = OSD::GetTextWidth(false, str);
    int height = 10 + padding * 2;

    scr.DrawRect(posX, posY, bgWidth + (borderWidth * 2) + (padding * 2), borderWidth, border);
    scr.DrawRect(posX + borderWidth + padding + bgWidth + padding, posY + borderWidth, borderWidth, height, border);
    scr.DrawRect(posX, posY + borderWidth + height, bgWidth + (borderWidth * 2) + (padding)*2, borderWidth, border);
    scr.DrawRect(posX, posY + borderWidth, borderWidth, height, border);

    scr.DrawRect(posX + borderWidth, posY + borderWidth, bgWidth + padding * 2, 10 + padding * 2, background);

    u32 strX = 0;
    u32 strY = 0;

    // TODO : enumに置き換える
    switch (fontAlign)
    {
    case 0:
      strX = posX + borderWidth;
      strY = posY + borderWidth;
      break;
    case 1:
      strX = posX + borderWidth + padding;
      strY = posY + borderWidth;
      break;
    case 2:
      strX = posX + borderWidth + padding * 2;
      strY = posY + borderWidth;
      break;
    case 3:
      strX = posX + borderWidth + padding * 2;
      strY = posY + borderWidth + padding;
      break;
    case 4:
      strX = posX + borderWidth + padding * 2;
      strY = posY + borderWidth + padding * 2;
      break;
    case 5:
      strX = posX + borderWidth + padding;
      strY = posY + borderWidth + padding * 2;
      break;
    case 6:
      strX = posX + borderWidth;
      strY = posY + borderWidth + padding * 2;
      break;
    case 7:
      strX = posX + borderWidth;
      strY = posY + borderWidth + padding;
      break;
    case 8:
      strX = posX + borderWidth + padding;
      strY = posY + borderWidth + padding;
      break;
    }

    scr.Draw(str, strX, strY, foreground, background);
  }

  void DrawSysfontPlus(const Screen &scr, const std::string &str, u32 posX, u32 posY, u32 borderWidth, u32 padding, const Color &foreground, const Color &background, const Color &border, bool fillBackground, bool rightAligned, int fontAlign)
  {
    int bgWidth = OSD::GetTextWidth(true, str);
    int height = 16 + padding * 2;
    if (rightAligned)
    {
      scr.DrawRect(posX - bgWidth - (borderWidth * 2) - (padding), posY, bgWidth + (borderWidth * 2) + (padding * 2), borderWidth, border);
      scr.DrawRect(posX - borderWidth, posY + borderWidth, borderWidth, height, border);
      scr.DrawRect(posX - bgWidth - (borderWidth * 2) - (padding), posY + borderWidth + height, bgWidth + (borderWidth * 2) + (padding)*2, borderWidth, border);
      scr.DrawRect(posX - bgWidth - (borderWidth * 2) - (padding), posY + borderWidth, borderWidth, height, border);
      if (fillBackground)
      {
        scr.DrawRect(posX + borderWidth - bgWidth, posY + borderWidth, bgWidth + padding * 2, 16 + padding * 2, background);
      }
      scr.DrawSysfont(str, posX + borderWidth + padding - bgWidth, posY + borderWidth + padding, foreground);
    }
    else
    {
      scr.DrawRect(posX, posY, bgWidth + (borderWidth * 2) + (padding * 2), borderWidth, border);
      scr.DrawRect(posX + borderWidth + padding + bgWidth + padding, posY + borderWidth, borderWidth, height, border);
      scr.DrawRect(posX, posY + borderWidth + height, bgWidth + (borderWidth * 2) + (padding)*2, borderWidth, border);
      scr.DrawRect(posX, posY + borderWidth, borderWidth, height, border);

      if (fillBackground)
      {
        scr.DrawRect(posX + borderWidth, posY + borderWidth, bgWidth + padding * 2, 16 + padding * 2, background);
      }

      u32 strX = 0;
      u32 strY = 0;

      // TODO : enumに置き換える
      switch (fontAlign)
      {
      case 0:
        strX = posX + borderWidth;
        strY = posY + borderWidth;
        break;
      case 1:
        strX = posX + borderWidth + padding;
        strY = posY + borderWidth;
        break;
      case 2:
        strX = posX + borderWidth + padding * 2;
        strY = posY + borderWidth;
        break;
      case 3:
        strX = posX + borderWidth + padding * 2;
        strY = posY + borderWidth + padding;
        break;
      case 4:
        strX = posX + borderWidth + padding * 2;
        strY = posY + borderWidth + padding * 2;
        break;
      case 5:
        strX = posX + borderWidth + padding;
        strY = posY + borderWidth + padding * 2;
        break;
      case 6:
        strX = posX + borderWidth;
        strY = posY + borderWidth + padding * 2;
        break;
      case 7:
        strX = posX + borderWidth;
        strY = posY + borderWidth + padding;
        break;
      case 8:
        strX = posX + borderWidth + padding;
        strY = posY + borderWidth + padding;
        break;
      }

      scr.DrawSysfont(str, strX, strY, foreground);
    }
  }

  float DegreeToRadian(float degree)
  {
    return degree * (M_PI / 180);
  }

  void DrawCircle(const Screen &scr, u32 x, u32 y, u32 radiusStart, u32 radiusEnd, int start, int end, const Color &color, int origin)
  {
    u32 rectLength = (radiusEnd * 2) / 1.41421356237;
    u32 miniRadius = rectLength / 2;

    u32 rectX = x - miniRadius;
    u32 rectY = y - miniRadius;

    if (start == 0 && end == 360 && radiusStart == 0)
    {
      scr.DrawRect(rectX, rectY, rectLength, rectLength, color);
    }
    else
    {
      miniRadius = radiusStart;
    }

    // TODO : enumに置き換える
    switch (origin)
    {
    case 0:
      x += radiusEnd;
      y += radiusEnd;
      break;
    case 1:
      y += radiusEnd;
      break;
    case 2:
      x -= radiusEnd;
      y += radiusEnd;
      break;
    case 3:
      x -= radiusEnd;
      break;
    case 4:
      x -= radiusEnd;
      y -= radiusEnd;
      break;
    case 5:
      y -= radiusEnd;
      break;
    case 6:
      x += radiusEnd;
      y -= radiusEnd;
      break;
    case 7:
      x += radiusEnd;
      break;
    case 8:
      break;
    }

    for (u32 r = miniRadius; r < radiusEnd; r++)
    {
      for (int angle = start; angle < end; angle++)
      {
        scr.DrawPixel(x + cos(DegreeToRadian(angle)) * r, y + sin(DegreeToRadian(angle)) * r, color);
      }
    }
  }

  void DrawRectPlus(const Screen &scr, u32 posX, u32 posY, u32 width, u32 height, const Color &color, bool filled, int origin)
  {
    // TODO : enumに置き換える
    switch (origin)
    {
    case 0:
      break;
    case 1:
      posX -= width / 2;
      break;
    case 2:
      posX -= width;
      break;
    case 3:
      posX -= width;
      posY -= height / 2;
      break;
    case 4:
      posX -= width;
      posY -= height;
      break;
    case 5:
      posX -= width / 2;
      posY -= height;
      break;
    case 6:
      posY -= height;
      break;
    case 7:
      posY -= height / 2;
      break;
    case 8:
      posX -= width / 2;
      posY -= height / 2;
      break;
    }

    scr.DrawRect(posX, posY, width, height, color, filled);
  }

  bool TouchRect(u32 x, u32 y, u32 w, u32 h)
  {
    if (Touch::IsDown())
    {
      UIntVector pos = Touch::GetPosition();
      if (pos.x >= x && pos.y >= y && pos.x <= (x + w) && pos.y <= (y + h))
        return true;
    }
    return false;
  }

  bool TouchCircle(u32 x, u32 y, u8 size)
  {
    u32 rectLength = (size * 2) / 1.41421356237;
    u32 miniRadius = rectLength / 2;

    u32 rectX = x - miniRadius;
    u32 rectY = y - miniRadius;
    if (TouchRect(rectX, rectY, rectLength, rectLength))
      return true;

    UIntVector pos = Touch::GetPosition();
    for (int r = miniRadius; r < size; r++)
    {
      for (int angle = 0; angle < 360; angle++)
      {
        if (pos.x == x + cos(DegreeToRadian(angle)) * r && pos.y == y + sin(DegreeToRadian(angle)) * r)
          return true;
      }
    }
    return false;
  }

  bool flagShowScreenBuffer = false;
  std::vector<std::vector<Color>> screenBuffer(400, std::vector<Color>(240, Color(0, 0, 0, 0)));

  void setFlagShowScreenBuffer(bool flag)
  {
    flagShowScreenBuffer = flag;
  }

  void setScreenBuffer(u16 x, u16 y, Color color)
  {
    screenBuffer[x][y] = color;
  }

  void fillScreenBuffer(Color color)
  {
    screenBuffer = std::vector<std::vector<Color>>(400, std::vector<Color>(240, color));
  }

  bool ShowScreenBuffer(const Screen &screen)
  {
    if (!screen.IsTop)
      return false;
    for (int i = 0; i < 400; i++)
    {
      for (int j = 0; j < 240; j++)
      {
        if (screenBuffer[i][j].a)
        {
          screen.DrawPixel(i, j, screenBuffer[i][j]);
        }
      }
    }
    if (!flagShowScreenBuffer)
      return true;

    return false;
  }

  std::string
  ReadSJIS(u32 Address)
  {
    std::string out = "";
    u8 num = 0;
    while (1)
    {
      if (*(u8 *)(Address + num) == 0)
        return out;
      if ((*(u8 *)(Address + num) < 0x80) || (*(u8 *)(Address + num) > 0xA0))
      {
        u16 buff = Convert::sjisToUtf16(*(u8 *)(Address + num));
        std::string buff_str;
        Utils::ConvertUTF16ToUTF8(buff_str, &buff);
        out += buff_str.substr(0, 1);
        num++;
      }
      else
      {
        u16 buff = Convert::sjisToUtf16(((*(u8 *)(Address + num)) * 0x100) + *(u8 *)(Address + num + 1));
        std::string buff_str;
        Utils::ConvertUTF16ToUTF8(buff_str, &buff);
        out += buff_str.substr(0, 3);
        num += 2;
      }
    }
    return out;
  }

  bool colorPickerOpened = false;
  size_t barLength = 24;
  UIntVector pickerPos = {126, 27};
  Color barColor = Color::Red, selectedColor;
  std::vector<Color> barColorVector;
  std::vector<std::vector<Color>> pickerColorVector;

  std::vector<Color>
  drawVerticalGradation(u16 x, u16 w, u16 y, u16 h, Color start, Color end, Screen scr)
  {
    std::vector<Color> color_vector;
    double d, dd;
    h += y;
    u16 yy = y;
    for (; y < h; y++)
    {
      d = (double)(y - yy) / (double)(h - 1 - yy);
      dd = 1.0 - d;

      u8 r = (int)(end.r * d + start.r * dd);
      u8 g = (int)(end.g * d + start.g * dd);
      u8 b = (int)(end.b * d + start.b * dd);
      if (r == 254)
        r = 255;
      if (g == 254)
        g = 255;
      if (b == 254)
        b = 255;
      color_vector.push_back(Color(r, g, b));
      scr.DrawRect(x, y, w, 1, Color(r, g, b));
    }
    return color_vector;
  }

  std::vector<std::vector<Color>>
  DrawPicker(Color color, Screen scr)
  {
    std::vector<std::vector<Color>> color_vector;
    scr.DrawRect(26, 26, 102, 102, Color::White, false);
    for (u16 x = 27; x < 127; x++)
    {
      double d = (double)(x - 27) / (double)(127 - 1 - 27);
      double dd = 1.0 - d;

      u8 r = (int)(color.r * d + 255.0 * dd);
      u8 g = (int)(color.g * d + 255.0 * dd);
      u8 b = (int)(color.b * d + 255.0 * dd);

      color_vector.push_back(drawVerticalGradation(x, 1, 27, 100, Color(r, g, b), Color::Black, scr));
    }
    return color_vector;
  }

  bool DrawColorPicker(const Screen &scr, Color &out)
  {
    Screen topScr = OSD::GetTopScreen();
    topScr.DrawRect(30, 20, 340, 200, Color::Black);
    topScr.DrawRect(32, 22, 336, 196, Color::White, false);
    topScr.DrawSysfont("CPad,CStickで動かせます", 35, 25);
    scr.DrawRect(20, 20, 280, 200, Color::Black);
    scr.DrawRect(22, 22, 276, 196, Color::White, false);
    barColorVector.clear();
    std::vector<Color> buff_vector;
    buff_vector = drawVerticalGradation(260, 15, 24, 32, Color(255, 0, 0), Color(255, 255, 0), scr);
    barColorVector.insert(barColorVector.end(), buff_vector.begin(), buff_vector.end());
    buff_vector = drawVerticalGradation(260, 15, 56, 32, Color(255, 255, 0), Color(0, 255, 0), scr);
    barColorVector.insert(barColorVector.end(), buff_vector.begin(), buff_vector.end());
    buff_vector = drawVerticalGradation(260, 15, 88, 32, Color(0, 255, 0), Color(0, 255, 255), scr);
    barColorVector.insert(barColorVector.end(), buff_vector.begin(), buff_vector.end());
    buff_vector = drawVerticalGradation(260, 15, 120, 32, Color(0, 255, 255), Color(0, 0, 255), scr);
    barColorVector.insert(barColorVector.end(), buff_vector.begin(), buff_vector.end());
    buff_vector = drawVerticalGradation(260, 15, 152, 32, Color(0, 0, 255), Color(255, 0, 255), scr);
    barColorVector.insert(barColorVector.end(), buff_vector.begin(), buff_vector.end());
    buff_vector = drawVerticalGradation(260, 15, 184, 32, Color(255, 0, 255), Color(255, 0, 0), scr);
    barColorVector.insert(barColorVector.end(), buff_vector.begin(), buff_vector.end());

    if (Controller::IsKeyDown(Key::CPadLeft) && 27 < pickerPos.x)
      pickerPos.x -= 1;
    if (Controller::IsKeyDown(Key::CPadRight) && pickerPos.x < 126)
      pickerPos.x += 1;
    if (Controller::IsKeyDown(Key::CPadUp) && 27 < pickerPos.y)
      pickerPos.y -= 1;
    if (Controller::IsKeyDown(Key::CPadDown) && pickerPos.y < 126)
      pickerPos.y += 1;
    if (Controller::IsKeyDown(Key::CStickUp) && 24 < barLength)
      barLength -= 1;
    if (Controller::IsKeyDown(Key::CStickDown) && barLength < 216)
      barLength += 1;

    if (TouchRect(260, 24, 40, 192))
      barLength = Touch::GetPosition().y;
    if (barLength - 24 < barColorVector.size())
      barColor = barColorVector[barLength - 24];
    scr.DrawRect(261, barLength, 20, 1, Color::White);
    scr.DrawRect(281, barLength - 2, 5, 5, barColor);
    pickerColorVector = DrawPicker(barColor, scr);

    if (TouchRect(27, 27, 99, 99))
      pickerPos = {Touch::GetPosition().x, Touch::GetPosition().y};
    if ((pickerPos.x - 27 < pickerColorVector.size()) && (pickerPos.y - 27 < pickerColorVector[0].size()))
      selectedColor = pickerColorVector[pickerPos.x - 27][pickerPos.y - 27];

    DrawRectPlus(scr, 160, 30, 70, 19, Color::White, true, 0);
    DrawSysfontPlus(scr, "H", 150, 30, 0, 0, Color::White, Color::Black, Color::Red, true, 8, 0);
    DrawSysfontPlus(scr, Utils::Format("%d", (int)((double)(barLength - 24) / 192.0 * 360.0)), 230, 30, 0, 0, Color::Black, Color::Black, Color::Red, false, true, 8);
    DrawRectPlus(scr, 160, 50, 70, 19, Color::White, true, 0);
    DrawSysfontPlus(scr, "S", 150, 50, 0, 0, Color::White, Color::Black, Color::Red, true, 8, 0);
    DrawSysfontPlus(scr, Utils::Format("%d", pickerPos.x - 26), 230, 50, 0, 0, Color::Black, Color::Black, Color::Red, false, true, 8);
    DrawRectPlus(scr, 160, 70, 70, 19, Color::White, true, 0);
    DrawSysfontPlus(scr, "V", 150, 70, 0, 0, Color::White, Color::Black, Color::Red, true, 8, 0);
    DrawSysfontPlus(scr, Utils::Format("%d", pickerPos.y - 26), 230, 70, 0, 0, Color::Black, Color::Black, Color::Red, false, true, 8);
    DrawRectPlus(scr, 160, 120, 70, 19, Color::White, true, 0);
    DrawSysfontPlus(scr, "R", 150, 120, 0, 0, Color::White, Color::Black, Color::Red, true, 8, 0);
    DrawSysfontPlus(scr, Utils::Format("%d", barColor.r), 230, 120, 0, 0, Color::Black, Color::Black, Color::Red, false, true, 8);
    DrawRectPlus(scr, 160, 140, 70, 19, Color::White, true, 0);
    DrawSysfontPlus(scr, "G", 150, 140, 0, 0, Color::White, Color::Black, Color::Red, true, 8, 0);
    DrawSysfontPlus(scr, Utils::Format("%d", barColor.g), 230, 140, 0, 0, Color::Black, Color::Black, Color::Red, false, true, 8);
    DrawRectPlus(scr, 160, 160, 70, 19, Color::White, true, 0);
    DrawSysfontPlus(scr, "B", 150, 160, 0, 0, Color::White, Color::Black, Color::Red, true, 8, 0);
    DrawSysfontPlus(scr, Utils::Format("%d", barColor.b), 230, 160, 0, 0, Color::Black, Color::Black, Color::Red, false, true, 8);

    // cross
    scr.DrawRect(pickerPos.x - 6, pickerPos.y, 5, 1, Color::Gray);
    scr.DrawRect(pickerPos.x + 1, pickerPos.y, 5, 1, Color::Gray);
    scr.DrawRect(pickerPos.x, pickerPos.y - 6, 1, 5, Color::Gray);
    scr.DrawRect(pickerPos.x, pickerPos.y + 1, 1, 5, Color::Gray);

    scr.DrawSysfont(Utils::Format("#%02X%02X%02X", selectedColor.r, selectedColor.g, selectedColor.b), 30, 195);
    scr.DrawRect(100, 195, 15, 15, selectedColor);

    if (TouchRect(160, 30, 70, 19))
    {
      Sleep(Milliseconds(300));
      Keyboard key("input hue\n0<=hue<=360\n誤差があります");
      key.IsHexadecimal(false);
      u16 hue;
      if (0 <= key.Open(hue))
        barLength = (int)((double)(hue) / 360.0 * 192.0 + 24);
    }
    else if (TouchRect(160, 50, 70, 19))
    {
      Sleep(Milliseconds(300));
      Keyboard key("input Saturation\n1<=Saturation<=100");
      key.IsHexadecimal(false);
      u8 Saturation;
      if (0 <= key.Open(Saturation))
        if (0 < Saturation && Saturation <= 100)
          pickerPos.x = Saturation + 26;
    }
    else if (TouchRect(160, 70, 70, 19))
    {
      Sleep(Milliseconds(300));
      Keyboard key("input Value\n1<=Value<=100");
      key.IsHexadecimal(false);
      u8 Value;
      if (0 <= key.Open(Value))
        if (0 < Value && Value <= 100)
          pickerPos.y = Value + 26;
    }
    else if (TouchRect(160, 120, 70, 19))
    {
      Sleep(Milliseconds(300));
      Keyboard key("input Red");
      key.IsHexadecimal(false);
      u8 Value;
      if (0 <= key.Open(Value))
      {
        barColor.r = Value;
        if (Value == 254)
          Value = 255;
        for (size_t i = 0; i < barColorVector.size(); i++)
        {
          if ((Value == barColorVector[i].r) && (barColor.g == barColorVector[i].g) && (barColor.b == barColorVector[i].b))
          {
            barLength = i + 24;
          }
        }
      }
    }
    else if (TouchRect(160, 140, 70, 19))
    {
      Sleep(Milliseconds(300));
      Keyboard key("input Green");
      key.IsHexadecimal(false);
      u8 Value;
      if (0 <= key.Open(Value))
      {
        barColor.g = Value;
        if (Value == 254)
          Value = 255;
        for (size_t i = 0; i < barColorVector.size(); i++)
        {
          if ((barColor.r == barColorVector[i].r) && (Value == barColorVector[i].g) && (barColor.b == barColorVector[i].b))
          {
            barLength = i + 24;
          }
        }
      }
    }
    else if (TouchRect(160, 160, 70, 19))
    {
      Sleep(Milliseconds(300));
      Keyboard key("input Blue");
      key.IsHexadecimal(false);
      u8 Value;
      if (0 <= key.Open(Value))
      {
        barColor.b = Value;
        if (Value == 254)
          Value = 255;
        for (size_t i = 0; i < barColorVector.size(); i++)
        {
          if ((barColor.r == barColorVector[i].r) && (barColor.g == barColorVector[i].g) && (Value == barColorVector[i].b))
          {
            barLength = i + 24;
          }
        }
      }
    }

    scr.DrawRect(130, 191, 50, 22, Color::Gray);
    scr.DrawRect(130, 191, 50, 22, Color::White, false);
    scr.DrawSysfont("cancel", 132, 194);
    scr.DrawRect(190, 191, 50, 22, Color::Gray);
    scr.DrawRect(190, 191, 50, 22, Color::White, false);
    scr.DrawSysfont("OK", 202, 194);
    if (TouchRect(130, 191, 50, 22))
    {
      colorPickerOpened = false;
      return 0;
    }
    if (TouchRect(190, 191, 50, 22))
    {
      out = selectedColor;
      colorPickerOpened = false;
      return 1;
    }

    if (Controller::IsKeyPressed(Key::B))
    {
      colorPickerOpened = false;
      return 0;
    }
    return 1;
  }

  bool colorPicker(Color &out)
  {
    if (!Process::IsPaused())
      return 0;

    const Screen &scr = OSD::GetBottomScreen();
    colorPickerOpened = true;
    while (colorPickerOpened)
    {
      Controller::Update();
      if (!DrawColorPicker(scr, out))
        return 0;
      OSD::SwapBuffers();
    }
    return 1;
  }
} // namespace CTRPluginFramework