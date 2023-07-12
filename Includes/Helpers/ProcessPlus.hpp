#ifndef MYFUNCTIONS_PROCESSPLUS_HPP
#define MYFUNCTIONS_PROCESSPLUS_HPP

#include "CTRPluginFramework.hpp"
#include "Convert.hpp"

namespace CTRPluginFramework
{
    template <typename T>
    T ReadValue(u32 address)
    {
        if (!Process::CheckAddress(address, MEMPERM_READ))
            return -1;
        
        return *reinterpret_cast<T *>(address);
    }

    template <typename T>
    void WriteValue(u32 address, T value)
    {
        if (!Process::CheckAddress(address, MEMPERM_WRITE))
            return;
        
        *reinterpret_cast<T *>(address) = value;
    }

    class ProcessPlus
    {
    public:

        static void Write32(const std::vector<u32> &addresses, const std::vector<u32> &values, MenuEntry *entry);
        static void Write32(const u32 &baseAddress, const std::vector<u32> &offsets, const std::vector<u32> &values, MenuEntry *entry);

        static void Write16(const std::vector<u32> &addresses, const std::vector<u16> &values, MenuEntry *entry);
        static void Write16(const u32 &baseAddress, const std::vector<u32> &offsets, const std::vector<u16> &values, MenuEntry *entry);

        static void Write8(const std::vector<u32> &addresses, const std::vector<u8> &values, MenuEntry *entry);
        static void Write8(const u32 &baseAddress, const std::vector<u32> &offsets, const std::vector<u8> &values, MenuEntry *entry);
        
        static void WriteFloat(const std::vector<u32> &addresses, const std::vector<float> &values, MenuEntry *entry);
        static void WriteFloat(const u32 &baseAddress, const std::vector<u32> &offsets, const std::vector<float> &values, MenuEntry *entry);

        static void WriteString(const u32 &address, const std::string &input);
        static void WriteString(const u32 &address, const std::string &input, MenuEntry *entry);
    };
}

#endif
