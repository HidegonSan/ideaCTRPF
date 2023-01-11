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
    void Tetris(MenuEntry *entry);
    void SetTetrisSetting(MenuEntry *entry);

    std::vector<u64> getFrame(int &frame);
}
#endif