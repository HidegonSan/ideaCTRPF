#pragma once

#include <CTRPluginFramework.hpp>
#include <bitset>
#include "types.h"

namespace CTRPluginFramework
{
  class LifeGame_Class
  {
  public:
    void LifeGame_Loop(void);
    static LifeGame_Class *GetInstance(void)
    {
      if (!_instance)
        _instance = new LifeGame_Class();
      return _instance;
    }

  private:
    static constexpr u8 FIELD_WIDTH = 100;
    static constexpr u8 FIELD_HEIGHT = 60;
    static constexpr u8 BLOCK_WIDTH = 16;

    UIntVector _selector = {4, 6};
    UIntVector _btmPos = {3, 5};
    bool _isLoopingGen = false;
    int _speed = 200;
    bool _isTorus = true;

    std::bitset<FIELD_HEIGHT * FIELD_WIDTH> _field;

    static LifeGame_Class *_instance;

    const Screen &topScr = OSD::GetTopScreen();
    const Screen &btmScr = OSD::GetBottomScreen();

    void DrawField(void);
    void NextGen(void);
    s8 LivesAround(s16 x, s16 y);

    LifeGame_Class();
    ~LifeGame_Class();
  };
}