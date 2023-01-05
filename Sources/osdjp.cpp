#include "osdjp.hpp"

namespace CTRPluginFramework
{
  std::vector<Sys_OSD> OSDJPlist;

  bool OSDJP(const Screen &screen)
  {
    int i = 0;
    if (!screen.IsTop)
      return false;
    while (i < OSDJPlist.size())
    {
      if (OSDJPlist[i].time.GetElapsedTime() <= Seconds(5))
      {
        int Leaght = OSD::GetTextWidth(true, OSDJPlist[i].name);
        screen.DrawRect(380 - (Leaght + 2), 220 - (i * 20), Leaght + 4, 16 + 2, Color(OSDJPlist[i].bg.ToU32()));
        screen.DrawSysfont(OSDJPlist[i].name, 380 - Leaght, 220 - (i * 20), Color(OSDJPlist[i].fg.ToU32()));
        if (++i == 12)
          break;
      }
      else
      {
        OSDJPlist.erase(OSDJPlist.begin() + i);
      }
    }
    return true;
  }

  bool OSDJPNotify(const std::string &str, const Color &fg, const Color &bg)
  {
    OSD::Run(OSDJP);
    if (OSDJPlist.size() >= 20)
      return false;
    OSDJPlist.push_back({str, Color(fg.ToU32()), Color(bg.ToU32()), Clock()});
    return true;
  }
}