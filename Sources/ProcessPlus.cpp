#include "MyFunctions/ProcessPlus.hpp"

namespace CTRPluginFramework
{
    template <typename T>
    void ValueBase(const std::vector<u32> &addresses, const std::vector<T> &values, MenuEntry *entry)
    {
        static std::vector<T> saveValues;
        if (entry->WasJustActivated()) {
            saveValues.clear();
            for (int i = 0; i < static_cast<int>(addresses.size()); i++) {
                saveValues.emplace_back(ReadValue<T>(addresses.at(i)));
                WriteValue<T>(addresses.at(i), values.at(i));
            }
        }
        else if (!entry->IsActivated()) {
            for (int i = 0; i < static_cast<int>(saveValues.size()); i++)
                WriteValue<T>(addresses.at(i), saveValues.at(i));
        }
    }

    template <typename T>
    void PointerBase(const u32 &baseAddress, const std::vector<u32> &offsets, const std::vector<T> &values, MenuEntry *entry)
    {
        static std::vector<T> saveValues;
        u32 address = ReadValue<u32>(baseAddress);
        if (Process::CheckAddress(address, MEMPERM_WRITE) && address) {
            if (entry->WasJustActivated()) {
                saveValues.clear();
                for (int i = 0; i < static_cast<int>(offsets.size()); i++) {
                    saveValues.emplace_back(ReadValue<T>(address + offsets.at(i)));
                    WriteValue<T>(address + offsets.at(i), values.at(i));
                }
            }
            else if (!entry->IsActivated()) {
                for (int i = 0; i < static_cast<int>(saveValues.size()); i++)
                    WriteValue<T>(address + offsets.at(i), saveValues.at(i));
            }
        }
    }

    void ProcessPlus::Write32(const std::vector<u32> &addresses, const std::vector<u32> &values, MenuEntry *entry)
    {
        ValueBase<u32>(addresses, values, entry);
    }

    void ProcessPlus::Write32(const u32 &baseAddress, const std::vector<u32> &offsets, const std::vector<u32> &values, MenuEntry *entry)
    {
        PointerBase<u32>(baseAddress, offsets, values, entry);
    }

    void ProcessPlus::Write16(const std::vector<u32> &addresses, const std::vector<u16> &values, MenuEntry *entry)
    {
        ValueBase<u16>(addresses, values, entry);
    }

    void ProcessPlus::Write16(const u32 &baseAddress, const std::vector<u32> &offsets, const std::vector<u16> &values, MenuEntry *entry)
    {
        PointerBase<u16>(baseAddress, offsets, values, entry);
    }

    void ProcessPlus::Write8(const std::vector<u32> &addresses, const std::vector<u8> &values, MenuEntry *entry)
    {
        ValueBase<u8>(addresses, values, entry);
    }

    void ProcessPlus::Write8(const u32 &baseAddress, const std::vector<u32> &offsets, const std::vector<u8> &values, MenuEntry *entry)
    {
        PointerBase<u8>(baseAddress, offsets, values, entry);
    }

    void ProcessPlus::WriteFloat(const std::vector<u32> &addresses, const std::vector<float> &values, MenuEntry *entry)
    {
        ValueBase<float>(addresses, values, entry);
    }

    void ProcessPlus::WriteFloat(const u32 &baseAddress, const std::vector<u32> &offsets, const std::vector<float> &values, MenuEntry *entry)
    {
        PointerBase<float>(baseAddress, offsets, values, entry);
    }

    void ProcessPlus::WriteString(const u32 &address, const std::string &input)
    {
        std::vector<u8> SJIS;
        std::vector<u16> values = Convert::StrToSJIS(input);
        for (int i = 0; i < static_cast<int>(values.size()); i++) {
            u8 upBit = (values.at(i) >> 8) & 0xFF;
            if (upBit)
                SJIS.emplace_back(upBit);
            
            SJIS.emplace_back(values.at(i) & 0xFF);
        }
        for (int i = 0; i < static_cast<int>(SJIS.size()); i++)
            Process::Write8(address + i, SJIS.at(i));
    }

    void ProcessPlus::WriteString(const u32 &address, const std::string &input, MenuEntry *entry)
    {
        static std::vector<u8> saveValues;
        if (entry->WasJustActivated()) {
            std::vector<u8> SJIS;
            std::vector<u16> values = Convert::StrToSJIS(input);
            for (int i = 0; i < static_cast<int>(values.size()); i++) {
                u8 upBit = (values.at(i) >> 8) & 0xFF;
                if (upBit)
                    SJIS.emplace_back(upBit);
                
                SJIS.emplace_back(values.at(i) & 0xFF);
            }
            for (int i = 0; i < static_cast<int>(SJIS.size()); i++) {
                saveValues.emplace_back(ReadValue<u8>(address + i));
                Process::Write8(address + i, SJIS.at(i));
            }
        }
        else if (!entry->IsActivated()) {
            for (int i = 0; i < static_cast<int>(saveValues.size()); i++)
                Process::Write8(address + i, saveValues.at(i));
        }
    }
}
