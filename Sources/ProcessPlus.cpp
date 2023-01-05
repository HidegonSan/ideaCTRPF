#include "Helpers.hpp"

namespace CTRPluginFramework
{
  void ProcessPlus::PointerWrite8(u32 Pointer, u32 Offset, u8 Value)
  {
    u32 Address;

    if (Process::Read32(Pointer, Address))
    {
      Process::Write8(Address + Offset, Value);
    }
  }

  void ProcessPlus::PointerWrite16(u32 Pointer, u32 Offset, u16 Value)
  {
    u32 Address;

    if (Process::Read32(Pointer, Address))
    {
      Process::Write16(Address + Offset, Value);
    }
  }

  void ProcessPlus::PointerWrite32(u32 Pointer, u32 Offset, u32 Value)
  {
    u32 Address;

    if (Process::Read32(Pointer, Address))
    {
      Process::Write32(Address + Offset, Value);
    }
  }

  // Max: 100
  u8 SaveValue_8[100];
  u16 SaveValue_16[100];
  u32 SaveValue_32[100];

  void ProcessPlus::MainWrite8(u32 Address, u8 Value, MenuEntry *entry)
  {
    static u8 SaveValue;
    static bool Check = false;

    if (entry->WasJustActivated())
    {
      if (!Check)
      {
        SaveValue = *(u8 *)(Address);
        Check = true;
      }

      Process::Write8(Address, Value);
    }
    else if (!entry->IsActivated())
    {
      if (Check)
      {
        Process::Write8(Address, SaveValue);

        SaveValue = 0;
        Check = false;
      }
    }
  }

  void ProcessPlus::MainWrite8(const std::vector<u32> &Addresses, const std::vector<u8> &Values, MenuEntry *entry)
  {
    int size = Addresses.size();
    u8 SaveValue_8[size];
    static bool Check = false;

    if (entry->WasJustActivated())
    {
      if (!Check)
      {
        for (int i = 0; i < size; i++)
        {
          SaveValue_8[i] = *(u8 *)(Addresses[i]);
        }
        Check = true;
      }

      for (int i = 0; i < size; i++)
      {
        Process::Write8(Addresses[i], Values[i]);
      }
    }
    else if (!entry->IsActivated())
    {
      if (Check)
      {
        for (int i = 0; i < size; i++)
        {
          Process::Write8(Addresses[i], SaveValue_8[i]);
          SaveValue_8[i] = 0;
        }

        Check = false;
      }
    }
  }

  void ProcessPlus::MainWrite16(u32 Address, u16 Value, MenuEntry *entry)
  {
    static u16 SaveValue;
    static bool Check = false;

    if (entry->WasJustActivated())
    {
      if (!Check)
      {
        SaveValue = *(u16 *)(Address);
        Check = true;
      }

      Process::Write16(Address, Value);
    }
    else if (!entry->IsActivated())
    {
      if (Check)
      {
        Process::Write16(Address, SaveValue);

        SaveValue = 0;
        Check = false;
      }
    }
  }

  void ProcessPlus::MainWrite16(const std::vector<u32> &Addresses, const std::vector<u16> &Values, MenuEntry *entry)
  {
    int size = Addresses.size();
    u16 SaveValue_16[size];
    static bool Check = false;

    if (entry->WasJustActivated())
    {
      if (!Check)
      {
        for (int i = 0; i < size; i++)
        {
          SaveValue_16[i] = *(u16 *)(Addresses[i]);
        }
        Check = true;
      }

      for (int i = 0; i < size; i++)
      {
        Process::Write16(Addresses[i], Values[i]);
      }
    }
    else if (!entry->IsActivated())
    {
      if (Check)
      {
        for (int i = 0; i < size; i++)
        {
          Process::Write16(Addresses[i], SaveValue_16[i]);
          SaveValue_16[i] = 0;
        }

        Check = false;
      }
    }
  }

  void ProcessPlus::MainWrite32(u32 Address, u32 Value, MenuEntry *entry)
  {
    static u32 SaveValue;
    static bool Check = false;

    if (entry->WasJustActivated())
    {
      if (!Check)
      {
        SaveValue = *(u32 *)(Address);
        Check = true;
      }

      Process::Write32(Address, Value);
    }
    else if (!entry->IsActivated())
    {
      if (Check)
      {
        Process::Write32(Address, SaveValue);

        SaveValue = 0;
        Check = false;
      }
    }
  }

  void ProcessPlus::MainWrite32(const std::vector<u32> &Addresses, const std::vector<u32> &Values, MenuEntry *entry)
  {
    int size = Addresses.size();
    static bool Check = false;

    if (entry->WasJustActivated())
    {
      if (!Check)
      {
        for (int i = 0; i < size; i++)
        {
          SaveValue_32[i] = *(u32 *)(Addresses[i]);
        }
        Check = true;
      }

      for (int i = 0; i < size; i++)
      {
        Process::Write32(Addresses[i], Values[i]);
      }
    }
    else if (!entry->IsActivated())
    {
      if (Check)
      {
        for (int i = 0; i < size; i++)
        {
          Process::Write32(Addresses[i], SaveValue_32[i]);
          SaveValue_32[i] = 0;
        }

        Check = false;
      }
    }
  }
}