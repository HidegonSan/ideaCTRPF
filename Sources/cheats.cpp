#include "cheats.hpp"
#include "osdjp.hpp"
#include "Convert.hpp"
#include "JPKeyboard.hpp"
#include "KaniCodes.hpp"
#include "Tetris.hpp"
#include "LifeGame.hpp"

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

  std::vector<int> find_all(const std::string str, const std::string subStr)
  {
    std::vector<int> result;

    int subStrSize = subStr.size();
    size_t pos = str.find(subStr);

    while (pos != std::string::npos)
    {
      result.push_back(pos);
      pos = str.find(subStr, pos + subStrSize);
    }

    return result;
  }

  std::string treeFolder(std::string path, u8 space)
  {
    std::string buff = "";
    StringVector files, folders;
    Directory dir(path);
    dir.ListFiles(files);
    dir.ListDirectories(folders);
    for (auto file : files)
      buff += std::string(space * 2, ' ') + "├" + file + "\n";
    for (auto folder : folders)
    {
      buff += std::string(space * 2, ' ') + "├" + (Color::Green << folder) + "\n"
              << Color::White;
      buff += treeFolder(path + "/" + folder, space + 1);
    }
    return buff;
  }

  std::string getFileObscurely(Directory dir, std::string word)
  {
    StringVector files;
    dir.ListFiles(files);
    for (auto file : files)
      if (file.find(word) != std::string::npos)
        return file;
    return "no file";
  }
  std::string getFolderObscurely(Directory dir, std::string word)
  {
    StringVector folders;
    dir.ListDirectories(folders);
    for (auto folder : folders)
      if (folder.find(word) != std::string::npos)
        return folder;
    return "no folder";
  }

  size_t getReturnCount(std::string &str)
  {
    size_t count = 0;
    count = find_all(str, "\n").size();
    for (size_t i = 0; i < count - 1; i++)
    {
      if (
          OSD::GetTextWidth(true, str.substr(find_all(str, "\n")[i], find_all(str, "\n")[i + 1] - find_all(str, "\n")[i])) > 330)
        for (int j = 0;; j++)
        {
          if (OSD::GetTextWidth(true, str.substr(find_all(str, "\n")[i], j)) > 330)
          {
            str = str.substr(0, find_all(str, "\n")[i] + j - 1) + "\n" + str.substr(find_all(str, "\n")[i] + j - 1);
            break;
          }
        }
    }
    count = find_all(str, "\n").size();
    return count;
  }

  void Cd(Directory &dir, std::string &path, std::string &str)
  {
    u64 pos = 18446744073709551615UL;
    if (path.substr(0, 2) == "..")
    {
      Directory::Open(dir, dir.GetFullName().substr(0, find_all(dir.GetFullName(), "/")[find_all(dir.GetFullName(), "/").size() - 1]));
      if (path.find("/") == std::string::npos)
        str += dir.GetFullName() + "\n";
      else
      {
        path.erase(0, 3);
        StringVector files, folders;
        dir.ListFiles(files);
        dir.ListDirectories(folders);
        Cd(dir, path, str);
      }
    }
    else if (Directory::IsExists(dir.GetFullName() + "/" + getFolderObscurely(dir, path.find("/") != std::string::npos ? path.substr(0, path.find("/")) : path)))
    {
      Directory::Open(dir, dir.GetFullName() + "/" + getFolderObscurely(dir, path.find("/") != std::string::npos ? path.substr(0, path.find("/")) : path));
      if ((pos = path.find("/")) == std::string::npos)
        str += dir.GetFullName() + "\n";
      else
      {
        path.erase(0, pos + 1);
        StringVector files, folders;
        dir.ListFiles(files);
        dir.ListDirectories(folders);
        Cd(dir, path, str);
      }
    }
    else
      str += "invalid directory\n" + path;
  }

  void getEntries(MenuFolder *folder, std::vector<MenuEntry *> &entries)
  {
    std::vector<MenuEntry *> menuEntries = folder->GetEntryList();
    for (auto menuEntry : menuEntries)
      if (menuEntry->GetMenuFunc() == nullptr)
        entries.push_back(menuEntry);
    std::vector<MenuFolder *> menuFolders = folder->GetFolderList();
    for (auto menuFolder : menuFolders)
      getEntries(menuFolder, entries);
  }

  void Command(MenuEntry *entry)
  {
    std::string commandLine_buffer = Color::Green << "command" << Color::White;
    Directory dir("");

    while (1)
    {
      if (dir.GetFullName().substr(dir.GetFullName().length() - 1, 1) == "/")
        Directory::Open(dir, dir.GetFullName().substr(0, dir.GetFullName().length() - 1));
      StringVector files, folders;
      dir.ListFiles(files);
      dir.ListDirectories(folders);
      commandLine_buffer += "\n->";
      std::string input;
      u8 count = getReturnCount(commandLine_buffer);

      Keyboard key(11 < count ? commandLine_buffer.substr(find_all(commandLine_buffer, "\n")[count - 12] + 1) : commandLine_buffer);
      if (key.Open(input) <= -1)
        break;
      std::string space_delimiter = " ";
      input += space_delimiter;
      commandLine_buffer += input + "\n";
      std::vector<std::string> args{};

      size_t pos = 0;
      while ((pos = input.find(space_delimiter)) != std::string::npos)
      {
        args.push_back(input.substr(0, pos));
        input.erase(0, pos + space_delimiter.length());
      }
      if (args[0] == "ls")
      {
        size_t buff_size = commandLine_buffer.size();
        for (auto file : files)
          commandLine_buffer += "\n" + file;
        for (auto folder : folders)
          commandLine_buffer += Color::Green << "\n" + folder;
        if (files.size() + folders.size() > 10)
          MessageBox(commandLine_buffer.substr(buff_size) + "")();
        commandLine_buffer = commandLine_buffer << Color::White;
      }
      else if (args[0] == "exit")
        break;
      else if ((args[0] == "clear") || (args[0] == "cls"))
        commandLine_buffer = Color::Green << "command" << Color::White;
      else if (args[0] == "cd")
      {
        if (args.size() == 1)
          commandLine_buffer += "enter directory name";
        else
          Cd(dir, args[1], commandLine_buffer);
      }
      else if (args[0] == "pwd")
        commandLine_buffer += dir.GetFullName();
      else if (args[0] == "help")
      {
        if (args.size() == 1)
        {
          std::string _help = "command list\n・ls\n・clear|cls\n・cd\n・pwd\n・mkdir\n・tree\n・secret\n・cat\n・echo\n・rm\n・touch\n・entry\n・exit\n・help [command]\n" + (Color::Green << "Can be done even when files and directories are ambiguous." << Color::White);
          commandLine_buffer += _help;
          MessageBox(_help + "")();
        }
        else if (args[1] == "ls")
          commandLine_buffer += "show files and folders";
        else if ((args[1] == "cls") || (args[1] == "clear"))
          commandLine_buffer += "clear the screen";
        else if (args[1] == "cd")
          commandLine_buffer += "change directory\ncd [directory]";
        else if (args[1] == "pwd")
          commandLine_buffer += "show current directory";
        else if (args[1] == "mkdir")
          commandLine_buffer += "make directory\nmkdir [directory]";
        else if (args[1] == "tree")
          commandLine_buffer += "show all files and directories like tree";
        else if (args[1] == "cat")
          commandLine_buffer += "display in binary\ncat [file]";
        else if (args[1] == "echo")
          commandLine_buffer += "return word\necho [word]";
        else if (args[1] == "rm")
          commandLine_buffer += "remove file or folder\nrm [file|folder]";
        else if (args[1] == "touch")
          commandLine_buffer += "make file\ntouch [file]";
        else if (args[1] == "entry")
          commandLine_buffer += "enable or disable entry(only GameFunc)\nentry\n-l list entries\n-e [entry] enable entry\n-d [entry] disable entry";
        else if (args[1] == "exit")
          commandLine_buffer += "exit command";
      }
      else if (args[0] == "mkdir")
      {
        if (args.size() == 1)
          commandLine_buffer += "enter directory's name";
        else
        {
          dir.Create(dir.GetFullName() + "/" + args[1]);
          Directory::Open(dir, dir.GetFullName());
        }
      }
      else if (args[0] == "tree")
      {
        commandLine_buffer += treeFolder(dir.GetFullName(), 0);
        MessageBox(treeFolder(dir.GetFullName(), 0) + "")();
      }
      else if (args[0] == "kani")
      {
        commandLine_buffer += "(/)(;,,;)(/)\n>=<\nV=(° °)=V";
      }
      else if (args[0] == "commit")
      {
        std::stringstream ss;
        ss << std::hex << COMMIT_HASH;
        commandLine_buffer += ss.str();
      }
      else if (args[0] == "cat")
      {
        if (args.size() == 1)
          commandLine_buffer += "enter file's name";
        else
        {
          if (File::Exists(dir.GetFullName() + "/" + getFileObscurely(dir, args[1])))
          {
            File file(dir.GetFullName() + "/" + getFileObscurely(dir, args[1]));
            std::string text = dir.GetFullName() + "/" + getFileObscurely(dir, args[1]) + "\n" + Utils::Format("%d bytes", file.GetSize()) + "\n", out;
            while (1)
            {
              u8 buffer;
              file.Read((void *)&buffer, 1);
              text += Utils::Format("%02X", buffer);
              if (file.Tell() % 8 == 0)
                text += "\n";
              else if (file.Tell() % 4 == 0)
                text += " ";
              if (file.GetSize() == file.Tell())
                break;
            }
            if (find_all(text, "\n").size() > 10)
              MessageBox(text + "")();
            commandLine_buffer += text;
          }
          else
          {
            commandLine_buffer += dir.GetFullName() + "/" + getFileObscurely(dir, args[1]) + " not found";
          }
        }
      }
      else if (args[0] == "echo")
      {
        if (args.size() == 1)
          commandLine_buffer += "enter word";
        else
          commandLine_buffer += args[1];
      }
      else if (args[0] == "rm")
      {
        if (args.size() == 1)
          commandLine_buffer += "enter file's or directory's name";
        else
        {
          if (args[1].find("/") == std::string::npos)
          {
            if (Directory::IsExists(dir.GetFullName() + "/" + getFolderObscurely(dir, args[1])))
            {
              commandLine_buffer += "really delete " + dir.GetFullName() + "/" + getFolderObscurely(dir, args[1]) + "?[y/n]:\n";
              u8 count = getReturnCount(commandLine_buffer);
              std::string output;
              Keyboard(11 < count ? commandLine_buffer.substr(find_all(commandLine_buffer, "\n")[count - 12] + 1) : commandLine_buffer);
              if (key.Open(output) <= -1)
                break;
              if (output == "y")
                Directory::Remove(dir.GetFullName() + "/" + getFolderObscurely(dir, args[1]));
            }
            else if (File::Exists(dir.GetFullName() + "/" + getFileObscurely(dir, args[1])))
            {
              commandLine_buffer += "really delete " + dir.GetFullName() + "/" + getFileObscurely(dir, args[1]) + "?[y/n]:\n";
              u8 count = getReturnCount(commandLine_buffer);
              std::string output;
              Keyboard(11 < count ? commandLine_buffer.substr(find_all(commandLine_buffer, "\n")[count - 12] + 1) : commandLine_buffer);
              if (key.Open(output) <= -1)
                break;
              if (output == "y")
                File::Remove(dir.GetFullName() + "/" + getFileObscurely(dir, args[1]));
            }
            else
              commandLine_buffer += "no files found";
          }
          else
            commandLine_buffer += "Only files in this directory can be deleted";
          Directory::Open(dir, dir.GetFullName());
        }
      }
      else if (args[0] == "touch")
      {
        if (args.size() == 1)
          commandLine_buffer += "enter file's name";
        else
        {
          File::Create(dir.GetFullName() + "/" + args[1]);
          commandLine_buffer += dir.GetFullName() + "/" + args[1];
        }
      }
      else if (args[0] == "entry")
      {
        if (args.size() >= 2)
        {
          PluginMenu *menu = PluginMenu::GetRunningInstance();
          std::vector<CTRPluginFramework::MenuEntry *> entries;
          std::vector<CTRPluginFramework::MenuEntry *> menuEntries = menu->GetEntryList();
          for (auto menuEntry : menuEntries)
            if (menuEntry->GetMenuFunc() == nullptr)
              entries.push_back(menuEntry);
          std::vector<CTRPluginFramework::MenuFolder *> menuFolders = menu->GetFolderList();
          for (auto menuFolder : menuFolders)
            getEntries(menuFolder, entries);
          if (args[1] == "-l")
          {
            std::string buff = "only GameFunc:";
            for (MenuEntry *_entry : entries)
              buff += "\n" + _entry->Name();
            MessageBox(buff + "")();
            commandLine_buffer += buff;
          }
          else if ((args[1] == "-e") || (args[1] == "-d"))
          {
            if (args.size() == 2)
              commandLine_buffer += "enter entry name";
            else
            {
              for (MenuEntry *_entry : entries)
                if (_entry->Name() == args[2])
                {
                  if (args[1] == "-e")
                    _entry->Enable();
                  else
                    _entry->Disable();
                  goto END_TOGGLE_ENTRY;
                }
              commandLine_buffer += "can't find that GameFunc";
              continue;
            END_TOGGLE_ENTRY:
              commandLine_buffer += args[1] == "-e" ? "enabled " + args[2] : "disabled " + args[2];
            }
          }
          else
            commandLine_buffer += "unknown argment";
        }
        else
          commandLine_buffer += "Please add an argument\n-l -e -d";
      }
      else
        commandLine_buffer += "unknown command";
    }
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
    Tetris_Class::GetInstance()->Tetris_Loop(entry->Hotkeys);
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
    enum
    {
      PEN,
      ERASER,
      BUCKET
    };
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
    LifeGame_Class::GetInstance()->LifeGame_Loop();
  }
}