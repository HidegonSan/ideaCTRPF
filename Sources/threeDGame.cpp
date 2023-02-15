#include "threeDGame.hpp"
#include "KaniCodes.hpp"

#include <sstream>

namespace CTRPluginFramework
{
  void threeDGame::threeDGameLoop(void)
  {
    _isOpened = true;
    while (_isOpened)
    {
      Controller::Update();
      if (Controller::IsKeyPressed(Key::B))
        _isOpened = false;
      if (Touch::IsDown())
      {
        u8 btmBlockWidth = std::min(320 / _field_width, 240 / _field_height);
        for (u8 i = 0; i < _field_width; i++)
        {
          for (u8 j = 0; j < _field_height; j++)
          {
            if (TouchRect((320 - _field_width * btmBlockWidth) / 2 + i * btmBlockWidth, (240 - _field_height * btmBlockWidth) + j * btmBlockWidth, btmBlockWidth, btmBlockWidth))
            {
              _field[i + j * _field_width]++;
              if (_block_height < _field[i + j * _field_width])
                _field[i + j * _field_width] = 0;
              break;
            }
          }
        }
      }
      if (Controller::GetKeysDown())
      {
        DrawScreen();
        OSD::SwapBuffers();
        DrawScreen();
      }
    }
  }

  void threeDGame::DrawScreen(void)
  {
    const Screen &top = OSD::GetTopScreen();
    const Screen &btm = OSD::GetBottomScreen();

    top.DrawRect(0, 0, 400, 240, Color::Gray);
    btm.DrawRect(0, 0, 320, 240, Color::Gray);

    u8 btmBlockWidth = std::min(320 / _field_width, 240 / _field_height);
    for (u8 i = 0; i < _field_width; i++)
    {
      for (u8 j = 0; j < _field_height; j++)
      {
        DrawDiamond(top, 200 - _block_width / 2 * 1.7 * (j + 1) + _block_width / 2 * 1.7 * i, (240 - _block_width * _field_height - _block_width / 2 * _block_height) / 2 + _block_width / 2 * (i + 2) + _block_width / 2 * j + _block_width / 2 * (_block_height - _field[i + j * _field_width]), 200 - _block_width / 2 * 1.7 * j + _block_width / 2 * 1.7 * i, (240 - _block_width * _field_height - _block_width / 2 * _block_height) / 2 + _block_width / 2 * i + _block_width / 2 * j + _block_width / 2 * (_block_height - _field[i + j * _field_width]), _blockColors[_field[i + j * _field_width]] - Color(20, 20, 20), true);
        DrawDiamond(top, 200 - _block_width / 2 * 1.7 * (j + 1) + _block_width / 2 * 1.7 * i, (240 - _block_width * _field_height - _block_width / 2 * _block_height) / 2 + _block_width / 2 * (i + 1) + _block_width / 2 * j + _block_width / 2 * (_block_height - _field[i + j * _field_width]), 200 - _block_width / 2 * 1.7 * j + _block_width / 2 * 1.7 * i, (240 - _block_width * _field_height - _block_width / 2 * _block_height) / 2 + _block_width / 2 * i + _block_width / 2 * j + _block_width / 2 * (_block_height - _field[i + j * _field_width]), _blockColors[_field[i + j * _field_width]], true);
        btm.DrawRect((320 - _field_width * btmBlockWidth) / 2 + i * btmBlockWidth, (240 - _field_height * btmBlockWidth) + j * btmBlockWidth, btmBlockWidth, btmBlockWidth, _blockColors[_field[i + j * _field_width]]);
      }
    }
  }
}