#include "cheats.hpp"
#include "osdjp.hpp"
#include "Convert.hpp"
#include "JPKeyboard.hpp"
#include "KaniCodes.hpp"
#include "Tetris.hpp"
#include "LifeGame.hpp"
#include "Command.hpp"

#include <sstream>

#include <sstream>

namespace CTRPluginFramework
{
  void Test1(MenuEntry *entry) {}

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

  void Pipes(MenuEntry *entry)
  {
    static u32 xPos = 150, yPos = 120;
    static u8 direct = 3, temp_direct, buff_direct, i = 0, len, j;
    Color colorList[7] = {Color::Red, Color::Blue, Color::Cyan, Color::Green, Color::Yellow, Color::Orange, Color::White};
    static bool isReset = false;
    if (entry->WasJustActivated())
    {
      fillScreenBuffer(Color(0, 0, 0, 0));
      OSD::Run(ShowScreenBuffer);
    }
    if (!entry->IsActivated())
    {
      OSD::Stop(ShowScreenBuffer);
    }
    if (isReset)
    {
      switch (direct)
      {
      case 0:
        xPos = Utils::Random(2, 398);
        yPos = 2;
        break;
      case 1:
        xPos = 2;
        yPos = Utils::Random(2, 238);
        break;
      case 2:
        xPos = 398;
        yPos = Utils::Random(2, 238);
        break;
      default:
        xPos = Utils::Random(2, 398);
        yPos = 238;
        break;
      }
      isReset = false;
      if (i < 6)
        i++;
      else
        i = 0;
    }
    if (len > 0)
    {
      if (xPos <= 1 || xPos >= 399 || yPos <= 1 || yPos >= 239)
        isReset = true;
      else
      {
        for (int k = -1; k < 2; k++)
        {
          setScreenBuffer(xPos + k, yPos + k, colorList[i]);
        }
        switch (direct)
        {
        case 0:
          if (buff_direct == 1)
            setScreenBuffer(xPos + 2, yPos, Color(0, 0, 0, 0));
          else
            setScreenBuffer(xPos - 2, yPos, Color(0, 0, 0, 0));
          yPos++;
          break;
        case 1:
          if (buff_direct == 0)
            setScreenBuffer(xPos, yPos + 2, Color(0, 0, 0, 0));
          else
            setScreenBuffer(xPos, yPos - 2, Color(0, 0, 0, 0));
          xPos++;
          break;
        case 2:
          if (buff_direct == 0)
            setScreenBuffer(xPos, yPos + 2, Color(0, 0, 0, 0));
          else
            setScreenBuffer(xPos, yPos - 2, Color(0, 0, 0, 0));
          xPos--;
          break;
        default:
          if (buff_direct == 1)
            setScreenBuffer(xPos + 2, yPos, Color(0, 0, 0, 0));
          else
            setScreenBuffer(xPos - 2, yPos, Color(0, 0, 0, 0));
          yPos--;
          break;
        }
        len--;
      }
    }
    else
    {
      len = Utils::Random(3, 30);
      while (1)
      {
        temp_direct = Utils::Random(0, 3);
        if (direct != 3 - temp_direct)
          break;
      }
      buff_direct = direct;
      direct = temp_direct;
    }
    if (Controller::IsKeyPressed(Key::Start))
    {
      fillScreenBuffer(Color(0, 0, 0, 0));
      isReset = true;
    }
  }

  void addSearch(MenuFolder *folder, MenuFolder *SearchFolder, std::string input, std::vector<MenuFolder *> ancestorFolders)
  {
    if (folder->Name() == "Search")
      return;
    std::vector<CTRPluginFramework::MenuEntry *> entries = folder->GetEntryList();
    std::vector<MenuFolder *> folders = folder->GetFolderList();
    for (auto folder1 : folders)
    {
      bool flag = true;
      for (auto ancestorFolder : ancestorFolders)
        if (folder1 == ancestorFolder)
        {
          flag = false;
          break;
        }
      if (flag)
      {
        ancestorFolders.push_back(folder1);
        addSearch(folder1, SearchFolder, input, ancestorFolders);
      }
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
    if (!JPKeyboard("エントリー名を入力してください").Open(input))
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

    std::vector<MenuFolder *> ancestorFolders;
    for (auto folder : folders)
      addSearch(folder, SearchFolder, input, ancestorFolders);
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
  void rotateCube(const Screen &screen)
  {
    if (!screen.IsTop)
      return;
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
    for (size_t i = 0; i < 2; i++)
    {
      screen.DrawRect(0, 0, 400, 240, Color::Gray);
      screen.DrawSysfont("B:return to menu", 0, 0);
      screen.DrawSysfont("START:toggle auto rotate", 0, 15);
      for (int k = 0; k < width * height; k++)
      {
        switch (buffer[k])
        {
        case 64:
          screen.DrawPixel(k % width, k / width, Color::Red);
          break;
        case 36:
          screen.DrawPixel(k % width, k / width, Color::Blue);
          break;
        case 126:
          screen.DrawPixel(k % width, k / width, Color::Orange);
          break;
        case 35:
          screen.DrawPixel(k % width, k / width, Color::Green);
          break;
        case 59:
          screen.DrawPixel(k % width, k / width, Color::White);
          break;
        case 43:
          screen.DrawPixel(k % width, k / width, Color::Yellow);
          break;
        }
      }
      OSD::SwapBuffers();
    }
  }

  void Cube(MenuEntry *entry)
  {
    const Screen &screen = OSD::GetTopScreen();
    bool autoRotate = false;
    rotateCube(screen);
    while (!Controller::IsKeyPressed(Key::B))
    {
      if (Controller::IsKeyDown(Key::CPadLeft) || autoRotate)
        BB -= 0.05;
      if (Controller::IsKeyDown(Key::CPadRight))
        BB += 0.05;
      if (Controller::IsKeyDown(Key::CPadUp) || autoRotate)
        AA += 0.15;
      if (Controller::IsKeyDown(Key::CPadDown))
        AA -= 0.05;
      if (Controller::IsKeyDown(Key::CStickUp) || autoRotate)
        CC -= 0.10;
      if (Controller::IsKeyDown(Key::CStickDown))
        CC += 0.05;
      if (Controller::IsKeyDown(Key::CStickLeft))
        distanceFromCam -= 5;
      if (Controller::IsKeyDown(Key::CStickRight))
        distanceFromCam += 5;
      if (Controller::IsKeyPressed(Key::Start))
        autoRotate = !autoRotate;
      if (Controller::GetKeysDown() || autoRotate)
        rotateCube(screen);
      Controller::Update();
    }
  }

  void BadApple(MenuEntry *entry)
  {
    static int frame_num = 0;
    if (!entry->IsActivated())
    {
      Process::Play();
      return;
    }
    Process::Pause();
    if (entry->WasJustActivated())
      frame_num = 0;

    std::vector<u64> frame = getFrame(frame_num);

    const Screen &screen = OSD::GetTopScreen();
    for (size_t i = 0; i < frame.size(); i++)
    {
      u8 index = 0;
      for (u64 j = 1; j < 0x1000000000000000; j *= 2)
      {
        if (frame[i] & j)
          screen.DrawRect(380 - index * 6, 10 + i * 10, 6, 10, Color::White);
        else
          screen.DrawRect(380 - index * 6, 10 + i * 10, 6, 10, Color(0, 0, 0, 0));
        index++;
      }
    }
    Sleep(Milliseconds(100));
    frame_num++;
    OSD::SwapBuffers();
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

  void Command(MenuEntry *entry)
  {
    Command::GetInstance()->Command_Loop();
  }

  void ColorPicker(MenuEntry *entry)
  {
    Color out;
    colorPicker(out);
  }

  void Tetris(MenuEntry *entry)
  {
    if (!Process::IsPaused())
      return;
    Tetris::GetInstance()->Tetris_Loop(entry->Hotkeys);
  }

  void ShowPallet(MenuEntry *entry)
  {
    if (entry->WasJustActivated())
    {
      OSD::Run(ShowScreenBuffer);
    }
    if (!entry->IsActivated())
    {
      OSD::Stop(ShowScreenBuffer);
    }
  }

  void PaintDrawLine(std::vector<std::vector<Color>> &paintPallet, const Screen &screen, int srcX, int srcY, int dstX, int dstY, u32 size, const Color &color)
  {
    float x, y, dx, dy, step;
    int i;

    dx = dstX - srcX;
    dy = dstY - srcY;

    step = abs(dx) >= abs(dy) ? abs(dx) : abs(dy);

    dx /= step;
    dy /= step;
    x = srcX;
    y = srcY;
    i = 1;

    std::vector<UIntVector> poses;
    while (i <= step)
    {
      if (x < 0 || y < 0 || std::ssize(paintPallet) <= x || std::size(paintPallet[0]) <= y)
        break;

      u32 originX = x - (size / 2);
      u32 originY = y - (size / 2);
      for (u32 drawX = 0; drawX < size; drawX++)
      {
        for (u32 drawY = 0; drawY < size; drawY++)
        {
          paintPallet[originX + drawX][originY + drawY] = color;
          poses.push_back({static_cast<u32>(originX + drawX), static_cast<u32>(originY + drawY)});
        }
      }

      x += dx;
      y += dy;
      i++;
    }

    for (u8 i = 0; i < 2; i++)
    {
      for (auto &&pos : poses)
      {
        if (color.a) // pen
          screen.DrawPixel(pos.x + 20, pos.y + 10, color);
        else // eraser
          screen.DrawPixel(pos.x + 20, pos.y + 10, (int(pos.x) / 10 + int(pos.y) / 10) % 2 ? Color::White : Color::DarkGrey);
      }
      if (i == 0)
        OSD::SwapBuffers();
    }
  }

  bool isValid(const std::vector<std::vector<Color>> &paintPallet, int x, int y, Color prevC, Color newC)
  {
    return x >= 0 && x < std::ssize(paintPallet) && y >= 0 && y < std::ssize(paintPallet[0]) && paintPallet[x][y] == prevC && paintPallet[x][y] != newC;
  }

  void floodFill(std::vector<std::vector<Color>> &paintPallet, int x, int y, Color prevC, Color newC)
  {
    std::vector<std::pair<int, int>> queue = {{x, y}};

    std::pair<int, int> p(x, y);
    queue.push_back(p);

    paintPallet[x][y] = newC;

    while (queue.size() > 0)
    {
      std::pair<int, int> currPixel = queue[queue.size() - 1];
      queue.pop_back();

      int posX = currPixel.first;
      int posY = currPixel.second;

      if (isValid(paintPallet, posX + 1, posY, prevC, newC))
      {
        paintPallet[posX + 1][posY] = newC;
        p.first = posX + 1;
        p.second = posY;
        queue.push_back(p);
      }
      if (isValid(paintPallet, posX - 1, posY, prevC, newC))
      {
        paintPallet[posX - 1][posY] = newC;
        p.first = posX - 1;
        p.second = posY;
        queue.push_back(p);
      }
      if (isValid(paintPallet, posX, posY + 1, prevC, newC))
      {
        paintPallet[posX][posY + 1] = newC;
        p.first = posX;
        p.second = posY + 1;
        queue.push_back(p);
      }
      if (isValid(paintPallet, posX, posY - 1, prevC, newC))
      {
        paintPallet[posX][posY - 1] = newC;
        p.first = posX;
        p.second = posY - 1;
        queue.push_back(p);
      }
    }
  }

  void Paint(MenuEntry *entry)
  {
    bool isOpened = true;
    const Screen &topScr = OSD::GetTopScreen();
    const Screen &btmScr = OSD::GetBottomScreen();
    Color paintColor = Color::Black;
    u32 penSize = 1;
    UIntVector lastPos;
    std::vector<std::vector<Color>> paintPallet = std::vector<std::vector<Color>>(200, std::vector<Color>(200, Color(0, 0, 0, 0)));
    std::string paintModeName[] = {"PEN", "ERASER", "BUCKET"};
    u8 paintMode = PEN;
    Clock dropperClock;
  START:
    for (size_t i = 0; i < 2; i++)
    {
      topScr.DrawRect(0, 0, 400, 240, Color::Gray);
      btmScr.DrawRect(0, 0, 320, 240, Color::Gray);
      btmScr.DrawRect(19, 9, 202, 202, Color::Black, false);
      for (size_t x = 0; x < 200; x++)
        for (size_t y = 0; y < 200; y++)
        {
          Color color = paintPallet[x][y];
          if (color.a)
            btmScr.DrawPixel(x + 20, y + 10, color);
          else
            btmScr.DrawPixel(x + 20, y + 10, (x / 10 + y / 10) % 2 ? Color::White : Color::DarkGrey);
        }
      btmScr.DrawRect(200, 215, 50, 22, Color::Gray);
      btmScr.DrawRect(200, 215, 50, 22, Color::White, false);
      btmScr.DrawSysfont("cancel", 202, 218);
      btmScr.DrawRect(260, 215, 50, 22, Color::Gray);
      btmScr.DrawRect(260, 215, 50, 22, Color::White, false);
      btmScr.DrawSysfont("OK", 272, 218);
      btmScr.DrawSysfont("モード", 230, 10);
      btmScr.DrawRect(230, 50, 12, 12, Color::White, false);
      btmScr.DrawSysfont(paintModeName[paintMode], 230, 25);
      btmScr.DrawRect(231, 51, 10, 10, paintColor);
      OSD::SwapBuffers();
    }

    while (isOpened)
    {
      Controller::Update();
      lastPos = Touch::GetPosition();
      UIntVector dropperPos = lastPos;
      dropperClock.Restart();
      while (TouchRect(20, 10, 200, 200))
      {
        if (paintMode == BUCKET)
        {
          UIntVector pos = Touch::GetPosition();
          floodFill(paintPallet, pos.x - 20, pos.y - 10, paintPallet[pos.x][pos.y], paintColor);
          goto START;
        }
        while (TouchRect(dropperPos.x - 2, dropperPos.y - 2, 5, 5))
        {
          if (dropperClock.HasTimePassed(Seconds(1)))
          {
            if (paintPallet[dropperPos.x][dropperPos.y].a)
              paintColor = paintPallet[dropperPos.x][dropperPos.y];
            btmScr.DrawRect(231, 51, 10, 10, paintColor);
            OSD::SwapBuffers();
            btmScr.DrawRect(231, 51, 10, 10, paintColor);
            goto START;
          }
          Controller::Update();
        }
        if ((paintMode == PEN || paintMode == ERASER) && TouchRect(20, 10, 200, 200))
        {
          UIntVector pos = Touch::GetPosition();
          PaintDrawLine(paintPallet, btmScr, pos.x - 20, pos.y - 10, lastPos.x - 20, lastPos.y - 10, penSize, paintMode ? Color(0, 0, 0, 0) : paintColor);
          lastPos = Touch::GetPosition();
        }
        OSD::SwapBuffers();
        Controller::Update();
      }

      if (Controller::IsKeyPressed(Key::B) || TouchRect(200, 215, 50, 22))
      {
        isOpened = false;
      }

      if (TouchRect(260, 215, 50, 22))
      {
        for (size_t x = 0; x < 200; x++)
        {
          for (size_t y = 0; y < 200; y++)
          {
            Color color = paintPallet[x][y];
            setScreenBuffer(x + 100, y + 20, color);
          }
        }
        entry->SetGameFunc(ShowPallet);
        isOpened = false;
      }

      if (Controller::IsKeyPressed(Key::X) || TouchRect(230, 50, 12, 12))
      {
        colorPicker(paintColor);
        goto START;
      }

      if (Controller::IsKeyPressed(Key::Y))
      {
        if (paintMode == BUCKET)
          paintMode = PEN;
        else
          paintMode++;
      }
      if (Controller::IsKeyPressed(Key::R))
      {
        if (penSize < 30)
          penSize++;
      }
      if (Controller::IsKeyPressed(Key::L))
      {
        if (penSize > 1)
          penSize--;
      }
      btmScr.DrawRect(230, 25, 90, 20, Color::Gray);
      btmScr.DrawSysfont(paintModeName[paintMode], 230, 25);

      btmScr.DrawRect(231, 51, 10, 10, paintColor);

      btmScr.DrawRect(230, 65, 90, 20, Color::Gray);
      btmScr.DrawSysfont(Utils::Format("Pen: %upx", penSize), 230, 65);
      OSD::SwapBuffers();
    }
  }

  void LifeGame(MenuEntry *entry)
  {
    LifeGame::GetInstance()->LifeGame_Loop();
  }
}