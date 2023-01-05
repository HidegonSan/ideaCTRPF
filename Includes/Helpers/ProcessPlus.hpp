#ifndef PROCESSPLUS_HPP
#define PROCESSPLUS_HPP

#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework
{
    class ProcessPlus
    {
    public:
        static void PointerWrite8(u32 Pointer, u32 Offset, u8 Value);
        static void PointerWrite16(u32 Pointer, u32 Offset, u16 Value);
        static void PointerWrite32(u32 Pointer, u32 Offset, u32 Value);
        static void MainWrite8(u32 Address, u8 Value, MenuEntry *entry);
        static void MainWrite8(const std::vector<u32> &Addresses, const std::vector<u8> &Values, MenuEntry *entry);
        static void MainWrite16(u32 Address, u16 Value, MenuEntry *entry);
        static void MainWrite16(const std::vector<u32> &Addresses, const std::vector<u16> &Values, MenuEntry *entry);
        static void MainWrite32(u32 Address, u32 Value, MenuEntry *entry);
        static void MainWrite32(const std::vector<u32> &Addresses, const std::vector<u32> &Values, MenuEntry *entry);
    };
}

#endif
