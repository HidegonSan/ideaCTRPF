#include "cheats.hpp"
#include "osdjp.hpp"
#include "Convert.hpp"
#include "JPKeyboard.hpp"
#include "KaniCodes.hpp"
#include "Tetris.hpp"
#include "LifeGame.hpp"
#include "Command.hpp"
#include "threeDGame.hpp"
#include "Led.hpp"
#include "Game2048.hpp"
#include "Paint.hpp"

namespace CTRPluginFramework
{
  void Test1(MenuEntry *entry)
  {
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
    for (auto &&folder1 : folders)
    {
      bool flag = true;
      for (auto &&ancestorFolder : ancestorFolders)
        if (folder1 == ancestorFolder)
        {
          flag = false;
          break;
        }
      if (flag)
      {
        ancestorFolders.emplace_back(folder1);
        addSearch(folder1, SearchFolder, input, ancestorFolders);
      }
    }
    for (auto &&entry : entries)
    {
      if ((Convert::toLower(entry->Name()).find(input) != std::string::npos) || (Convert::hiraganaToKatakana(entry->Note()).find(input) != std::string::npos) || (Convert::katakanaToHiragana(entry->Note()).find(input) != std::string::npos))
      {
        *SearchFolder += entry;
      }
    }
  }

  void Search(MenuEntry *entry)
  {
    std::string input;
    PluginMenu *menu = PluginMenu::GetRunningInstance();
    if (!JPKeyboard("エントリー名を入力してください").SetMaxLength(30).Open(input))
      return;

    input = Convert::hiraganaToKatakana(Convert::toLower(input));
    std::vector<MenuFolder *> folders = menu->GetFolderList();
    MenuFolder *SearchFolder = nullptr;
    for (auto &&folder : folders)
    {
      if (folder->Name() == "Search")
      {
        folder->Clear();
        *folder += new MenuEntry("Search", nullptr, Search);
        SearchFolder = folder;
      }
    }
    std::vector<CTRPluginFramework::MenuEntry *> entries = menu->GetEntryList();
    for (auto &&menu_entry : entries)
    {
      if ((Convert::toLower(menu_entry->Name()).find(input) != std::string::npos) || (Convert::toLower(menu_entry->Note()).find(input) != std::string::npos))
        *SearchFolder += new MenuEntry(menu_entry->Name(), menu_entry->GetGameFunc(), menu_entry->GetMenuFunc(), menu_entry->Note());
    }

    std::vector<MenuFolder *> ancestorFolders;
    for (auto &&folder : folders)
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
    if (0 <= i)
    {
      Sound("MUSIC/" + files_name[i]).Play();
    }
  }

  void Command(MenuEntry *entry)
  {
    Command::GetInstance().Command_Loop();
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
    Tetris::GetInstance().Start(entry->Hotkeys);
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

  void MenuEntryPaint(MenuEntry *entry)
  {
    Paint paint;

    if (paint.Open() == 0)
    {
      entry->SetGameFunc(ShowPallet);
    }
  }

  void LifeGame(MenuEntry *entry)
  {
    LifeGame::GetInstance().LifeGame_Loop();
  }

  void threeD(MenuEntry *entry)
  {
    threeDGame::GetInstance().threeDGameLoop();
  }

  // based on https://github.com/HaramakiPtr/CalcLED3gx/blob/master/Sources/LED.cpp#L76
  void LedEffect(MenuEntry *entry)
  {
    Led led(entry);

    if (Controller::IsKeyDown(Key::Y))
    {
      led.setSmoothing(0x20);
    }

    if (Controller::IsKeyDown(Key::DPadLeft))
    {
      // red
      led.setColor(0xFF, 0x00, 0x00);
    }
    else if (Controller::IsKeyDown(Key::DPadDown))
    {
      // green
      led.setColor(0x00, 0xFF, 0x00);
    }
    else if (Controller::IsKeyDown(Key::DPadRight))
    {
      // blue
      led.setColor(0x00, 0x00, 0xFF);
    }
    else if (Controller::IsKeyDown(Key::DPadUp))
    {
      // white
      led.setColor(0xFF, 0xFF, 0xFF);
    }
    else
    {
      // off
      led.setColor(0x00, 0x00, 0x00);
    }

    led.update();
  }

  void Game2048(MenuEntry *entry)
  {
    Game2048::GetInstance().Game2048_Loop();
  }

  // made by maru
  struct vec3d
  {
    float x, y, z;
  };

  struct triangle
  {
    vec3d p[3];
  };

  struct mesh
  {
    std::vector<triangle> tris;
  };

  struct mat4x4
  {
    float m[4][4] = {0};
  };

  void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m)
  {
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f)
    {
      o.x /= w;
      o.y /= w;
      o.z /= w;
    }
  }

  void DrawLine(s32 x1, s32 y1, s32 x2, s32 y2, Color c)
  {
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1;
    dy = y2 - y1;
    const Screen &scr = OSD::GetTopScreen();

    // straight lines idea by gurkanctn
    if (dx == 0) // Line is vertical
    {
      if (y2 < y1)
        std::swap(y1, y2);
      for (y = y1; y <= y2; y++)
        scr.DrawPixel(x1, y, c);
      return;
    }

    if (dy == 0) // Line is horizontal
    {
      if (x2 < x1)
        std::swap(x1, x2);
      for (x = x1; x <= x2; x++)
        scr.DrawPixel(x, y1, c);
      return;
    }

    // Line is Funk-aye
    dx1 = std::abs(dx);
    dy1 = std::abs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
      if (dx >= 0)
      {
        x = x1;
        y = y1;
        xe = x2;
      }
      else
      {
        x = x2;
        y = y2;
        xe = x1;
      }

      scr.DrawPixel(x, y, c);

      for (i = 0; x < xe; i++)
      {
        x = x + 1;
        if (px < 0)
          px = px + 2 * dy1;
        else
        {
          if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
            y = y + 1;
          else
            y = y - 1;
          px = px + 2 * (dy1 - dx1);
        }

        scr.DrawPixel(x, y, c);
      }
    }
    else
    {
      if (dy >= 0)
      {
        x = x1;
        y = y1;
        ye = y2;
      }
      else
      {
        x = x2;
        y = y2;
        ye = y1;
      }

      scr.DrawPixel(x, y, c);

      for (i = 0; y < ye; i++)
      {
        y = y + 1;
        if (py <= 0)
          py = py + 2 * dx1;
        else
        {
          if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
            x = x + 1;
          else
            x = x - 1;
          py = py + 2 * (dx1 - dy1);
        }

        scr.DrawPixel(x, y, c);
      }
    }
  }

  void DrawTriangle(s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, Color c)
  {
    DrawLine(x1, y1, x2, y2, c);
    DrawLine(x2, y2, x3, y3, c);
    DrawLine(x3, y3, x1, y1, c);
  }

  void Projection(MenuEntry *entry)
  {
    // Init
    mesh meshCube;
    mat4x4 matProj;

    float fTheta = 0.0f;

    meshCube.tris = {

        // SOUTH
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},

        // EAST
        {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

        // NORTH
        {1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},

        // WEST
        {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

        // TOP
        {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

        // BOTTOM
        {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},

    };

    // Projection Matrix
    const int screenWidth = 240, screenHeight = 240;
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)screenWidth / (float)screenHeight;
    float fFovRad = 1.0f / std::tan(fFov * 0.5f / 180.0f * 3.14159f);

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

    const Screen &scr = OSD::GetTopScreen();

    while (1)
    {
      scr.DrawRect(0, 0, screenWidth, screenHeight, Color::Black);

      // Set up rotation matrices
      mat4x4 matRotZ, matRotX;
      fTheta += 0.05;

      // Rotation Z
      matRotZ.m[0][0] = std::cos(fTheta);
      matRotZ.m[0][1] = std::sin(fTheta);
      matRotZ.m[1][0] = -std::sin(fTheta);
      matRotZ.m[1][1] = std::cos(fTheta);
      matRotZ.m[2][2] = 1;
      matRotZ.m[3][3] = 1;

      // Rotation X
      matRotX.m[0][0] = 1;
      matRotX.m[1][1] = std::cos(fTheta * 0.5f);
      matRotX.m[1][2] = std::sin(fTheta * 0.5f);
      matRotX.m[2][1] = -std::sin(fTheta * 0.5f);
      matRotX.m[2][2] = std::cos(fTheta * 0.5f);
      matRotX.m[3][3] = 1;

      // Draw Triangles
      for (auto tri : meshCube.tris)
      {
        triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

        // Rotate in Z-Axis
        MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
        MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
        MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

        // Rotate in X-Axis
        MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
        MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
        MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

        // Offset into the screen
        triTranslated = triRotatedZX;
        triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
        triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
        triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

        // Project triangles from 3D --> 2D
        MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
        MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
        MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

        // Scale into view
        triProjected.p[0].x += 1.0f;
        triProjected.p[0].y += 1.0f;
        triProjected.p[1].x += 1.0f;
        triProjected.p[1].y += 1.0f;
        triProjected.p[2].x += 1.0f;
        triProjected.p[2].y += 1.0f;
        triProjected.p[0].x *= 0.5f * (float)screenWidth;
        triProjected.p[0].y *= 0.5f * (float)screenHeight;
        triProjected.p[1].x *= 0.5f * (float)screenWidth;
        triProjected.p[1].y *= 0.5f * (float)screenHeight;
        triProjected.p[2].x *= 0.5f * (float)screenWidth;
        triProjected.p[2].y *= 0.5f * (float)screenHeight;

        // Rasterize triangle
        DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                     triProjected.p[1].x, triProjected.p[1].y,
                     triProjected.p[2].x, triProjected.p[2].y,
                     Color::Yellow);
      }
      OSD::SwapBuffers();

      Controller::Update();
      if (Controller::IsKeyPressed(Key::B))
        break;
    }
    // Update
  }

  bool http_download(const char *url, u8 **output, u32 *outSize)
  {
    Result ret = 0;
    httpcContext context;
    char *newurl = NULL;
    u32 statuscode = 0;
    u32 contentsize = 0, readsize = 0, size = 0;
    u8 *buf, *lastbuf;

    do
    {
      ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url, 1);

      // This disables SSL cert verification, so https:// will be usable
      ret = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);

      // Enable Keep-Alive connections
      ret = httpcSetKeepAlive(&context, HTTPC_KEEPALIVE_ENABLED);

      // Set a User-Agent header so websites can identify your application
      ret = httpcAddRequestHeaderField(&context, "User-Agent", "httpc-example/1.0.0");

      // Tell the server we can support Keep-Alive connections.
      // This will delay connection teardown momentarily (typically 5s)
      // in case there is another request made to the same server.
      ret = httpcAddRequestHeaderField(&context, "Connection", "Keep-Alive");

      ret = httpcBeginRequest(&context);
      if (ret != 0)
      {
        httpcCloseContext(&context);
        if (newurl != NULL)
          free(newurl);
        return false;
      }

      ret = httpcGetResponseStatusCode(&context, &statuscode);
      if (ret != 0)
      {
        httpcCloseContext(&context);
        if (newurl != NULL)
          free(newurl);
        return false;
      }

      if ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308))
      {
        if (newurl == NULL)
          newurl = (char *)calloc(1, 0x1000); // One 4K page for new URL
        if (newurl == NULL)
        {
          httpcCloseContext(&context);
          return false;
        }
        ret = httpcGetResponseHeader(&context, "Location", newurl, 0x1000);
        url = newurl;                // Change pointer to the url that we just learned
        httpcCloseContext(&context); // Close this context before we try the next
      }
    } while ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308));

    if (statuscode != 200)
    {
      httpcCloseContext(&context);
      if (newurl != NULL)
        free(newurl);
      return false;
    }

    // This relies on an optional Content-Length header and may be 0
    ret = httpcGetDownloadSizeState(&context, NULL, &contentsize);
    if (ret != 0)
    {
      httpcCloseContext(&context);
      if (newurl != NULL)
        free(newurl);
      return false;
    }

    // Start with a single page buffer
    buf = (u8 *)calloc(1, 0x100);
    if (buf == NULL)
    {
      httpcCloseContext(&context);
      if (newurl != NULL)
        free(newurl);
      return false;
    }

    do
    {
      // This download loop resizes the buffer as data is read.
      ret = httpcDownloadData(&context, buf + size, 0x100, &readsize);
      size += readsize;
      if (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING)
      {
        lastbuf = buf; // Save the old pointer, in case realloc() fails.
        buf = (u8 *)realloc(buf, size + 0x100);
        if (buf == NULL)
        {
          httpcCloseContext(&context);
          free(lastbuf);
          if (newurl != NULL)
            free(newurl);
          return false;
        }
      }
    } while (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);

    if (ret != 0)
    {
      httpcCloseContext(&context);
      if (newurl != NULL)
        free(newurl);
      free(buf);
      return false;
    }

    // Resize the buffer back down to our actual final size
    lastbuf = buf;
    buf = (u8 *)realloc(buf, size);
    if (buf == NULL)
    { // realloc() failed.
      httpcCloseContext(&context);
      free(lastbuf);
      if (newurl != NULL)
        free(newurl);
      return false;
    }

    httpcCloseContext(&context);
    *output = buf;
    *outSize = size;
    if (newurl != NULL)
      free(newurl);

    return true;
  }
}