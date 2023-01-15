#ifndef CHEATS_H
#define CHEATS_H

#include <CTRPluginFramework.hpp>
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
  using StringVector = std::vector<std::string>;

  void Test1(MenuEntry *entry);
  void Pipes(MenuEntry *entry);
  void Search(MenuEntry *entry);
  void Cube(MenuEntry *entry);
  void BadApple(MenuEntry *entry);
  void JPNotify(MenuEntry *entry);
  void ChangeBackGround(MenuEntry *entry);
  void PlayMusic(MenuEntry *entry);
  void Command(MenuEntry *entry);
  void ColorPicker(MenuEntry *entry);

  class Tetris_Class
  {
  public:
    void Tetris_Loop(HotkeyManager Hotkeys);

    void SetLevel(u8 level)
    {
      GetInstance()->_level = level;
    }

    void SetColorful(bool colorful)
    {
      GetInstance()->_colorfulMode = colorful;
    }

    static Tetris_Class *GetInstance()
    {
      if(!_instance)
        _instance = new Tetris_Class();
      return _instance;
    }

    ~Tetris_Class()
    {
      _instance = nullptr;
    }

  private:
    static constexpr u8 MINO_KINDS_COUNT = 7;
    static constexpr u8 FIELD_WIDTH = 10;
    static constexpr u8 FIELD_HEIGHT = 18;
    static constexpr u8 BLOCK_WIDTH = 12;

    const std::vector<Color> _mino_colors = {Color::White, Color::SkyBlue, Color::Blue, Color::Orange, Color::Yellow, Color::LimeGreen, Color::Purple, Color::Red};
    const std::vector<std::vector<std::vector<UIntVector>>> _mino_templates = {{{{1, 0}, {1, 1}, {1, 2}, {1, 3}}, {{2, 0}, {2, 1}, {1, 2}, {2, 2}}, {{1, 0}, {1, 1}, {1, 2}, {2, 2}}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {{1, 1}, {2, 1}, {0, 2}, {1, 2}}, {{0, 1}, {1, 1}, {2, 1}, {1, 2}}, {{0, 1}, {1, 1}, {1, 2}, {2, 2}}}, {{{0, 2}, {1, 2}, {2, 2}, {3, 2}}, {{0, 1}, {1, 1}, {2, 1}, {2, 2}}, {{2, 1}, {0, 2}, {1, 2}, {2, 2}}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {{1, 0}, {1, 1}, {2, 1}, {2, 2}}, {{1, 0}, {1, 1}, {2, 1}, {1, 2}}, {{2, 1}, {1, 2}, {2, 2}, {1, 3}}}, {{{2, 0}, {2, 1}, {2, 2}, {2, 3}}, {{1, 1}, {2, 1}, {1, 2}, {1, 3}}, {{1, 1}, {2, 1}, {2, 2}, {2, 3}}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {{1, 1}, {2, 1}, {0, 2}, {1, 2}}, {{1, 0}, {0, 1}, {1, 1}, {2, 1}}, {{1, 1}, {2, 1}, {2, 2}, {3, 2}}}, {{{0, 2}, {1, 2}, {2, 2}, {3, 2}}, {{1, 1}, {1, 2}, {2, 2}, {3, 2}}, {{1, 1}, {2, 1}, {3, 1}, {1, 2}}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {{1, 1}, {1, 2}, {2, 2}, {2, 3}}, {{1, 0}, {0, 1}, {1, 1}, {1, 2}}, {{2, 0}, {1, 1}, {2, 1}, {1, 2}}}};
    std::vector<std::vector<u8>> _field = std::vector<std::vector<u8>>(FIELD_WIDTH, std::vector<u8>(FIELD_HEIGHT, 0));

    static Tetris_Class *_instance;

    // 落下中のミノ
    struct {
      u8 kind = 0;
      u8 turn = 0;
      Clock dropClock;
      Clock moveClock;
      Clock softdropClock;
      std::vector<UIntVector> blocks = {{4, 0}, {4, 1}, {4, 2}, {4, 3}};
    } _mino;

    u8 _score = 0;
    u8 _level = 0;
    bool _colorfulMode = false;

    void Restart(void);
    void GameOver(void);
    void TurnBlock(bool turn_right);
    void MoveMino(int moveX, int moveY);
    Tetris_Class()
    {
      _instance = this;
    }
  };

  void Tetris(MenuEntry *entry);
  void SetTetrisSetting(MenuEntry *entry);

  std::vector<u64> getFrame(int &frame);
}
#endif