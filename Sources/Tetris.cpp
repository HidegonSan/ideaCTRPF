#include "Tetris.hpp"

namespace CTRPluginFramework
{
  u8 Tetris::GenerateNextMino(void)
  {
    if (_srcNexts.empty())
    {
      _srcNexts = {0, 1, 2, 3, 4, 5, 6};

      for (u8 i = 0; i < MINO_KINDS_COUNT; i++)
      {
        u8 r = (u8)Utils::Random(i, 6);
        std::swap(_srcNexts[i], _srcNexts[r]);
      }
    }

    u8 next = _srcNexts[0];
    _srcNexts.erase(_srcNexts.begin());

    return next;
  }

  void Tetris::NextMino(void)
  {
    _mino.kind = _nexts[0];
    _nexts.erase(_nexts.begin());
    _nexts.emplace_back(GenerateNextMino());
    _mino.turn = 0;
    _mino.dropClock.Restart();
    _mino.blocks = _mino_templates[0][_mino.kind];
    _mino.IsHeld = false;
    for (auto &&block : _mino.blocks)
      block.x += FIELD_WIDTH / 2 - 2;
  }

  void Tetris::GameOver(void)
  {
    MessageBox(Utils::Format("Game Over\nyour score is %d", _score))();
    _score = 0;
    _level = 0;
    _mino.heldKind = 9;
    _field = std::vector<std::vector<u8>>(FIELD_WIDTH, std::vector<u8>(FIELD_HEIGHT, 0));
  }

  bool Tetris::Restart(void)
  {
    for (auto &&block : _mino.blocks)
    {
      if (block.y == 1 || _field[block.x][block.y])
      {
        GameOver();
        return false;
      }

      _field[block.x][block.y] = _mino.kind + 1;
    }

    NextMino();
    return true;
  }

  void Tetris::TurnBlock(bool turn_right)
  {
    IntVector pos = {(int)((int)_mino.blocks[0].x - (int)_mino_templates[_mino.turn][_mino.kind][0].x), (int)((int)_mino.blocks[0].y - (int)_mino_templates[_mino.turn][_mino.kind][0].y)};
    if (turn_right)
    {
      if (_mino.turn == 3)
        _mino.turn = 0;
      else
        _mino.turn++;
    }
    else
    {
      if (_mino.turn == 0)
        _mino.turn = 3;
      else
        _mino.turn--;
    }
    for (auto &&block : _mino_templates[_mino.turn][_mino.kind])
    {
      if ((int)(block.x) + pos.x < 0)
      {
        pos.x += abs((int)(block.x) + pos.x);
        if (_field[block.x + pos.x][block.y + pos.y])
          goto RESET_TURN;
      }
      if (_field.size() - 1 < block.x + pos.x)
      {
        pos.x -= block.x + pos.x - _field.size() + 1;
        if (_field[block.x + pos.x][block.y + pos.y])
          goto RESET_TURN;
      }
      if ((int)(block.y) + pos.y < 0)
        pos.y += abs((int)(block.y) + pos.y);
      if (_field[0].size() - 1 < block.y + pos.y)
        goto RESET_TURN;
      if (_field[block.x + pos.x][block.y + pos.y])
        goto RESET_TURN;
    }
    _mino.blocks = _mino_templates[_mino.turn][_mino.kind];
    for (auto &&block : _mino.blocks)
    {
      block.x += pos.x;
      block.y += pos.y;
    }
    return;
  RESET_TURN:
    if (turn_right)
    {
      if (_mino.turn == 0)
        _mino.turn = 3;
      else
        _mino.turn--;
    }
    else
    {
      if (_mino.turn == 3)
        _mino.turn = 0;
      else
        _mino.turn++;
    }
    pos = {(int)(_mino.blocks[0].x - _mino_templates[_mino.turn][_mino.kind][0].x), (int)(_mino.blocks[0].y - _mino_templates[_mino.turn][_mino.kind][0].y)};
    _mino.blocks = _mino_templates[_mino.turn][_mino.kind];
    for (auto &&block : _mino.blocks)
    {
      block.x += pos.x;
      block.y += pos.y;
    }
  }

  void Tetris::MoveMino(int moveX, int moveY)
  {
    for (auto &&block : _mino.blocks)
    {
      block.x += moveX;
      block.y += moveY;
    }
  }

  void Tetris::HoldMino(void)
  {
    if (_mino.heldKind == 9)
    {
      _mino.heldKind = _mino.kind;
      NextMino();
    }
    else
    {
      _mino.turn = 0;
      _mino.dropClock.Restart();
      _mino.blocks = _mino_templates[0][_mino.heldKind];
      std::swap(_mino.heldKind, _mino.kind);
      _mino.IsHeld = true;
      for (auto &&block : _mino.blocks)
        block.x += FIELD_WIDTH / 2 - 2;
    }
  }

  bool Tetris::CheckSpace(Direction dir)
  {
    for (auto &&block : _mino.blocks)
    {
      if (dir == Direction::Under && block.x == 0)
        return false;
      else if (dir == Direction::Left && (block.x <= 0 || _field[block.x - 1][block.y]))
        return false;
      else if (dir == Direction::Right && (_field.size() - 1 <= block.x || _field[block.x + 1][block.y]))
        return false;
    }
    return true;
  }

  void Tetris::FillScreen(const Screen &scr, const Color &color)
  {
    scr.DrawRect(0, 0, scr.IsTop ? 400 : 320, 240, color);
    OSD::SwapBuffers();
    scr.DrawRect(0, 0, scr.IsTop ? 400 : 320, 240, color);
  }

  void Tetris::Update(HotkeyManager Hotkeys)
  {
    bool isOpened = true;
    const Screen &topScr = OSD::GetTopScreen();
    const Screen &btmScr = OSD::GetBottomScreen();

    _nexts.clear();
    for (u8 i = 0; i < NEXT_COUNT; i++)
    {
      _nexts.emplace_back(GenerateNextMino());
    }

    NextMino();

    FillScreen(topScr, Color::Gray);
    FillScreen(btmScr, Color::Gray);

    while (isOpened)
    {
      Controller::Update();

      topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2, 9, FIELD_WIDTH * BLOCK_WIDTH + 6, 222, Color::Black);
      topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 + 2, 11, FIELD_WIDTH * BLOCK_WIDTH + 6 - 4, 218, Color::White, false);

      // 左移動
      if (Hotkeys[0].IsDown() && CheckSpace(Direction::Left))
      {
        if (Hotkeys[0].IsPressed())
        {
          MoveMino(-1, 0);
          _mino.moveClock.Restart();
        }
        else if (_mino.moveClock.HasTimePassed(Seconds(0.2)))
        {
          MoveMino(-1, 0);
        }
      }

      // 右移動
      else if (Hotkeys[1].IsDown() && CheckSpace(Direction::Right))
      {
        if (Hotkeys[1].IsPressed())
        {
          MoveMino(1, 0);
          _mino.moveClock.Restart();
        }
        else if (_mino.moveClock.HasTimePassed(Seconds(0.2)))
        {
          MoveMino(1, 0);
        }
      }

      // ソフトドロップ
      else if (Hotkeys[2].IsDown() && CheckSpace(Direction::Under) && _mino.softdropClock.HasTimePassed(Seconds(0.08)))
      {
        MoveMino(0, 1);
        _mino.softdropClock.Restart();
      }

      // ハードドロップ
      else if (Hotkeys[3].IsPressed())
      {
        bool dropped = false;

        for (size_t i = 0; i < _field[0].size(); i++)
        {
          for (auto &&block : _mino.blocks)
          {
            if (block.y + i >= _field[0].size() - 1 || _field[block.x][block.y + 1 + i])
            {
              for (auto &&block : _mino.blocks)
                block.y += i;
              isOpened = Restart();
              dropped = true;
              break;
            }
          }
          if (dropped) break;
        }
      }

      // 右回転
      if (Hotkeys[4].IsPressed())
        TurnBlock(false);
    
      // 左回転
      if (Hotkeys[5].IsPressed())
        TurnBlock(true);

      // ホールド
      if (Hotkeys[6].IsPressed() && !_mino.IsHeld)
        HoldMino();

      // ポーズ
      if (Hotkeys[7].IsPressed())
      {
        s8 ans;
        if (0 <= (ans = Keyboard("Paused", {"continue", "settings", "quit"}).Open()))
        {
          if (ans == 1)
          {
            s8 answer;
            u8 width;
            if (0 <= (answer = Keyboard("tetris color", {"monochrome", "colorful"}).Open()))
              SetColorful(answer);
            if (0 <= (answer = Keyboard("level", {"easy", "normal", "difficult"}).Open()))
              SetLevel(answer * 5);
            Keyboard key("input field's width\ndefault is 10\n4以上23以下でよろ");
            key.IsHexadecimal(false);
            if (0 <= key.Open(width))
              SetFieldWidth(width);
          }
          else if (ans == 2)
            isOpened = false;
        }
        FillScreen(topScr, Color::Gray);
        FillScreen(btmScr, Color::Gray);
      }

      // ミノ落下
      if (_mino.dropClock.HasTimePassed(Milliseconds(1000 - _level * 30)))
      {
        for (auto &&block : _mino.blocks)
          if (block.y >= _field[0].size() - 1 || _field[block.x][block.y + 1])
          {
            isOpened = Restart();
            break;
          }
        for (auto &&block : _mino.blocks)
          block.y++;
        _mino.dropClock.Restart();
      }

      // フィールド上のブロック描画
      for (size_t i = 0; i < _field.size(); i++)
        for (size_t j = 0; j < _field[i].size(); j++)
          if (_field[i][j])
            topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 + 3 + i * BLOCK_WIDTH + 1, BLOCK_WIDTH + j * BLOCK_WIDTH + 1, BLOCK_WIDTH - 2, BLOCK_WIDTH - 2, _colorfulMode ? _mino_colors[_field[i][j]] : _mino_colors[0]);

      // 落下中のミノ描画
      for (auto &&block : _mino.blocks)
        topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 + 3 + block.x * BLOCK_WIDTH + 1, BLOCK_WIDTH + block.y * BLOCK_WIDTH + 1, BLOCK_WIDTH - 2, BLOCK_WIDTH - 2, _colorfulMode ? _mino_colors[_mino.kind + 1] : _mino_colors[0]);

      // Next
      for (u8 i = 0; i < NEXT_COUNT; i++)
      {
        u8 next = _nexts[i];
        u32 posY = 12 + 40 * i;
        topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 + FIELD_WIDTH * BLOCK_WIDTH + 6 + 2, posY, 55, 40, Color::White, false);
        topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 + FIELD_WIDTH * BLOCK_WIDTH + 6 + 3, posY + 1, 53, 38, Color::Black);
        for (auto &&block : _mino_templates[0][next])
          topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 + FIELD_WIDTH * BLOCK_WIDTH + 6 + 7 + block.x * BLOCK_WIDTH + 1, posY + block.y * BLOCK_WIDTH + 3, BLOCK_WIDTH - 2, BLOCK_WIDTH - 2, _colorfulMode ? _mino_colors[next + 1] : _mino_colors[0]);
      }

      // Hold
      topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 - 58, 12, 55, 40, Color::White, false);
      topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 - 57, 12 + 1, 53, 38, Color::Black);
      if (_mino.heldKind != 9)
      {
        for (auto &&block : _mino_templates[0][_mino.heldKind])
        {
          topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 - 53 + block.x * BLOCK_WIDTH + 1, 12 + block.y * BLOCK_WIDTH + 3, BLOCK_WIDTH - 2, BLOCK_WIDTH - 2, _colorfulMode ? _mino_colors[_mino.heldKind + 1] : _mino_colors[0]);
        }
      }

      // 落下位置描画
      bool drawn = false;
      for (int i = 0; i < FIELD_HEIGHT; i++)
      {
        for (auto &&block : _mino.blocks)
        {
          if (block.y + i >= FIELD_HEIGHT - 1 || _field[block.x][block.y + 1 + i])
          {
            for (auto &&block : _mino.blocks)
            {
              u32 y = block.y + i;
              topScr.DrawRect(200 - (FIELD_WIDTH * BLOCK_WIDTH + 6) / 2 + 3 + block.x * BLOCK_WIDTH + 1, BLOCK_WIDTH + y * BLOCK_WIDTH + 1, BLOCK_WIDTH - 2, BLOCK_WIDTH - 2, _colorfulMode ? _mino_colors[_mino.kind + 1] : _mino_colors[0], false);
            }
            drawn = true;
            break;
          }
        }
        if (drawn) break;
      }

      for (int i = 0; i < FIELD_HEIGHT; i++)
      {
        u8 count = 0;
        for (int j = 0; j < FIELD_WIDTH; j++)
          count += _field[j][i] ? 1 : 0;
        if (count == FIELD_WIDTH)
        {
          _score++;
          // 10ごとにレベルを上げる
          if (!(_score % 10) && _level < 11)
            _level++;

          for (int k = i - 1; k >= 0; k--)
            for (int j = 0; j < FIELD_WIDTH; j++)
              _field[j][k + 1] = _field[j][k];
        }
      }
      OSD::SwapBuffers();
    }
  }
}