#include "cheats.hpp"
#include "osdjp.hpp"
#include "AliceCodes.hpp"
#include "KaniCodes.hpp"
#include "../libctrpf/include/CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "../libctrpf/include/CTRPluginFrameworkImpl/Menu/KeyboardImpl.hpp"

namespace CTRPluginFramework
{
  void Test1(MenuEntry *entry)
  {
    Color out;
    colorPicker(out);
  }

  void JPNotify(MenuEntry *entry)
  {
    StringVector words = {"コ", "ン", "ニ", "チ", "ワ"};
    if (Controller::IsKeyDown(Key::Start) && Controller::IsKeyDown(Key::Y))
      words = {"オ", "チ", "ン", "チ", "ン"};
    if (Controller::IsKeyPressed(Key::Start))
    {
      std::string buff;
      for (int j = 0; j < 5; j++)
      {
        buff += words[Utils::Random(0, 4)];
      }
      u8 red = Utils::Random(0, 255), green = Utils::Random(0, 255), blue = Utils::Random(0, 255);
      u16 sum = std::max({red, green, blue}) + std::min({red, green, blue});
      OSDJPNotify(buff, Color(red, green, blue), Color(sum - red, sum - green, sum - blue));
    }
  }

  u32 xPos = 150, yPos = 120;
  u8 direct = 3, temp_direct, buff_direct, i = 0, len, j;
  Color colorList[7] = {Color::Red, Color::Blue, Color::Cyan, Color::Green, Color::Yellow, Color::Orange, Color::White};
  bool isReset = false;
  // たまにクラッシュするからだれか直してくれ
  void Pipes(MenuEntry *entry)
  {
    if (entry->WasJustActivated())
    {
      fillScreenBuffer(Color::Black);
      setFlagShowScreenBuffer(true);
      OSD::Run(ShowScreenBuffer);
    }
    if (!entry->IsActivated())
    {
      setFlagShowScreenBuffer(false);
      OSD::Stop(ShowScreenBuffer);
    }
    if (isReset)
    {
      switch (direct)
      {
      case 0:
        xPos = Utils::Random(2, 397);
        yPos = 2;
        break;
      case 1:
        xPos = 2;
        yPos = Utils::Random(2, 237);
        break;
      case 2:
        xPos = 398;
        yPos = Utils::Random(2, 237);
        break;
      default:
        xPos = Utils::Random(2, 397);
        yPos = 238;
        break;
      }
      isReset = false;
      j = 0;
      if (i < 6)
        i++;
      else
        i = 0;
    }
    if (len > 0)
    {
      if (xPos == 1 || xPos == 399 || yPos == 1 || yPos == 239)
      {
        isReset = true;
      }
      else
      {
        for (int k = -1; k < 2; k++)
        {
          setScreenBuffer(xPos + k, yPos + k, colorList[i]);
        }
        switch (direct)
        {
        case 0:
          if (j == 0)
          {
            if (buff_direct == 1)
              setScreenBuffer(xPos + 2, yPos, Color::Black);
            else
              setScreenBuffer(xPos - 2, yPos, Color::Black);
            yPos++;
            break;
          }
          setScreenBuffer(xPos + 2, yPos, Color::Black);
          setScreenBuffer(xPos - 2, yPos, Color::Black);
          yPos++;
          break;
        case 1:
          if (j == 0)
          {
            if (buff_direct == 0)
              setScreenBuffer(xPos, yPos + 2, Color::Black);
            else
              setScreenBuffer(xPos, yPos - 2, Color::Black);
            xPos++;
            break;
          }
          setScreenBuffer(xPos, yPos + 2, Color::Black);
          setScreenBuffer(xPos, yPos - 2, Color::Black);
          xPos++;
          break;
        case 2:
          if (j == 0)
          {
            if (buff_direct == 0)
              setScreenBuffer(xPos, yPos + 2, Color::Black);
            else
              setScreenBuffer(xPos, yPos - 2, Color::Black);
            xPos--;
            break;
          }
          setScreenBuffer(xPos, yPos + 2, Color::Black);
          setScreenBuffer(xPos, yPos - 2, Color::Black);
          xPos--;
          break;
        default:
          if (j == 0)
          {
            if (buff_direct == 1)
              setScreenBuffer(xPos + 2, yPos, Color::Black);
            else
              setScreenBuffer(xPos - 2, yPos, Color::Black);
            yPos--;
            break;
          }
          setScreenBuffer(xPos + 2, yPos, Color::Black);
          setScreenBuffer(xPos - 2, yPos, Color::Black);
          yPos--;
          break;
        }
        len--;
      }
    }
    else
    {
      len = Utils::Random(3, 30);
      j = 0;
      while (1)
      {
        temp_direct = Utils::Random(0, 3);
        if ((temp_direct == 0 && direct != 3) || (temp_direct == 1 && direct != 2) || (temp_direct == 2 && direct != 1) || (temp_direct == 3 && direct != 0))
          break;
      }
      buff_direct = direct;
      direct = temp_direct;
    }
    if (Controller::IsKeyPressed(Key::Start))
    {
      fillScreenBuffer(Color::Black);
      isReset = true;
    }
  }

  void addSearch(MenuFolder *folder, MenuFolder *SearchFolder, std::string input)
  {
    if (folder->Name() == "Search")
      return;
    std::vector<CTRPluginFramework::MenuEntry *> entries = folder->GetEntryList();
    std::vector<MenuFolder *> folders = folder->GetFolderList();
    for (auto folder1 : folders)
    {
      addSearch(folder1, SearchFolder, input);
    }
    for (auto entry : entries)
    {
      if ((Convert::toLower(entry->Name()).find(input) != std::string::npos) || (Convert::hiraganaToKatakana(entry->Note()).find(input) != std::string::npos) || (Convert::katakanaToHiragana(entry->Note()).find(input) != std::string::npos))
      {
        *SearchFolder += new MenuEntry(entry->Name(), entry->GetGameFunc(), entry->GetMenuFunc(), entry->Note());
      }
    }
  }

  void Search(MenuEntry *entry)
  {
    std::string input;
    PluginMenu *menu = PluginMenu::GetRunningInstance();
    japKey(input, "エントリー名を入力してください");
    if (input.empty())
      return;

    input = Convert::hiraganaToKatakana(Convert::toLower(input));
    std::vector<MenuFolder *> folders = menu->GetFolderList();
    MenuFolder *SearchFolder;
    for (auto folder : folders)
    {
      if (folder->Name() == "Search")
      {
        folder->Clear();
        *folder += new MenuEntry("Search", nullptr, Search);
        SearchFolder = folder;
      }
    }
    std::vector<CTRPluginFramework::MenuEntry *> entries = menu->GetEntryList();
    for (auto menu_entry : entries)
    {
      if ((Convert::toLower(menu_entry->Name()).find(input) != std::string::npos) || (Convert::toLower(menu_entry->Note()).find(input) != std::string::npos))
        *SearchFolder += new MenuEntry(menu_entry->Name(), menu_entry->GetGameFunc(), menu_entry->GetMenuFunc(), menu_entry->Note());
    }

    for (auto folder : folders)
      addSearch(folder, SearchFolder, input);
  }


  float AA, BB, CC;

  float cubeWidth = 20;
  int width = 400, height = 240;
  float zBuffer[400 * 240];
  char buffer[400 * 240];
  int distanceFromCam = 100;
  float horizontalOffset;
  float K1 = 40;

  float incrementSpeed = 0.6;

  float x, y, z;
  float ooz;
  int xp, yp;
  int idx;

  float calculateX(int i, int j, int k)
  {
    return j * sin(AA) * sin(BB) * cos(CC) - k * cos(AA) * sin(BB) * cos(CC) +
           j * cos(AA) * sin(CC) + k * sin(AA) * sin(CC) + i * cos(BB) * cos(CC);
  }

  float calculateY(int i, int j, int k)
  {
    return j * cos(AA) * cos(CC) + k * sin(AA) * cos(CC) -
           j * sin(AA) * sin(BB) * sin(CC) + k * cos(AA) * sin(BB) * sin(CC) -
           i * cos(BB) * sin(CC);
  }

  float calculateZ(int i, int j, int k)
  {
    return k * cos(AA) * cos(BB) - j * sin(AA) * cos(BB) + i * sin(BB);
  }

  void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch)
  {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1 / z;

    xp = (int)(width / 2 + horizontalOffset + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height)
    {
      if (ooz > zBuffer[idx])
      {
        zBuffer[idx] = ooz;
        buffer[idx] = ch;
      }
    }
  }
  void rotateCube(void)
  {
    memset(buffer, 10, sizeof(buffer));
    memset(zBuffer, 0, width * height * 4);
    horizontalOffset = 1 * cubeWidth;
    // second cube
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
    {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
      {
        calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
        calculateForSurface(cubeWidth, cubeY, cubeX, '$');
        calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
        calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
        calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
        calculateForSurface(cubeX, cubeWidth, cubeY, '+');
      }
    }
    fillScreenBuffer(Color::Black);
    for (int k = 0; k < width * height; k++)
    {
      switch (buffer[k])
      {
      case 64:
        setScreenBuffer(k % width, k / width, Color::Red);
        break;
      case 36:
        setScreenBuffer(k % width, k / width, Color::Blue);
        break;
      case 126:
        setScreenBuffer(k % width, k / width, Color::Orange);
        break;
      case 35:
        setScreenBuffer(k % width, k / width, Color::Green);
        break;
      case 59:
        setScreenBuffer(k % width, k / width, Color::White);
        break;
      case 43:
        setScreenBuffer(k % width, k / width, Color::Yellow);
        break;
      }
    }
  }

  void Cube(MenuEntry *entry)
  {
    if (entry->WasJustActivated())
    {
      setFlagShowScreenBuffer(true);
      OSD::Run(ShowScreenBuffer);
    }
    if (!entry->IsActivated())
    {
      setFlagShowScreenBuffer(false);
      OSD::Stop(ShowScreenBuffer);
    }
    rotateCube();
    if (Controller::IsKeyDown(Key::CPadLeft))
      BB -= 0.05;
    if (Controller::IsKeyDown(Key::CPadRight))
      BB += 0.05;
    if (Controller::IsKeyDown(Key::CPadUp))
      AA += 0.05;
    if (Controller::IsKeyDown(Key::CPadDown))
      AA -= 0.05;
    if (Controller::IsKeyDown(Key::CStickUp))
      CC -= 0.05;
    if (Controller::IsKeyDown(Key::CStickDown))
      CC += 0.05;
    if (Controller::IsKeyDown(Key::CStickLeft))
      distanceFromCam -= 5;
    if (Controller::IsKeyDown(Key::CStickRight))
      distanceFromCam += 5;
  }

  int frame_num = 0;
  void BadApple(MenuEntry *entry)
  {
    if (!entry->IsActivated())
    {
      ProcessImpl::Play(true);
      return;
    }
    ProcessImpl::Pause(false);
    if (entry->WasJustActivated())
      frame_num = 0;

    std::vector<u64> frame = getFrame(frame_num);

    const Screen &screen = OSD::GetTopScreen();
    for (int i = 0; i < frame.size(); i++)
    {
      u8 index = 0;
      for (u64 j = 1; j < 0x1000000000000000; j *= 2)
      {
        if (frame[i] & j)
          screen.DrawRect(380 - index * 6, 10 + i * 10, 6, 10, Color::White);
        else
          screen.DrawRect(380 - index * 6, 10 + i * 10, 6, 10, Color::Black);
        index++;
      }
    }
    Sleep(Milliseconds(100));
    frame_num++;
    OSD::SwapBuffers();
  }

  void ChangeBackGround(MenuEntry *entry)
  {
    StringVector files_name = {};
    Directory("BMP", true).ListFiles(files_name);
    if (!(files_name.size()))
    {
      MessageBox("no files found")();
      Sleep(Milliseconds(500));
      return;
    }
    for (int i = 0; i < files_name.size(); i++)
      if (files_name[i].substr(files_name[i].length() - 4, 4) != ".bmp")
      {
        files_name.erase(files_name.begin() + i);
        i--;
      }
    Keyboard key("select BMP:", files_name);
    s8 i = key.Open();
    if (i != -1)
    {
      switch (Keyboard("which", {"Top", "Bottom"}).Open())
      {
      case 0:
        AliceCodes::SetTopScreenBackground("BMP/" + files_name[i], false);
        Sleep(Milliseconds(500));
        break;
      case 1:
        AliceCodes::SetBottomScreenBackground("BMP/" + files_name[i], false);
        Sleep(Milliseconds(500));
        break;
      }
    }
  }

  void PlayMusic(MenuEntry *entry)
  {
    StringVector files_name;
    Directory("MUSIC", true).ListFiles(files_name, ".bcwav");
    if (!files_name.size())
    {
      MessageBox("no files found")();
      return;
    }
    s8 i = Keyboard("select file:", files_name).Open();
    if (i != -1)
    {
      // u32 temp;
      // File fp("MUSIC/" + files_name[i]);
      // Result res = svcControlMemoryEx(&temp, (u32)0x04000000, (u32)0x04000000, fp.GetSize(), MEMOP_MAP, (MemPerm)(MEMPERM_READ | MEMPERM_WRITE), true);
      // if (R_SUCCEEDED(res))
      // {
      //   fp.Inject(temp, fp.GetSize());
      //   Sound((u8 *)temp).Play();
      // }
      // else
      //   MessageBox(Utils::Format("memory error%08X", (u32)temp))();
      // fp.Close();
      Sound("MUSIC/" + files_name[i]).Play();
    }
  }
}