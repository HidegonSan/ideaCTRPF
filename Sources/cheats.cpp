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

  // たまにクラッシュするからだれか直してくれ
  void Pipes(MenuEntry *entry)
  {
    static u32 xPos = 150, yPos = 120;
    static u8 direct = 3, temp_direct, buff_direct, i = 0, len, j;
    Color colorList[7] = {Color::Red, Color::Blue, Color::Cyan, Color::Green, Color::Yellow, Color::Orange, Color::White};
    static bool isReset = false;
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

  std::vector<int> find_all(const std::string str, const std::string subStr)
  {
    std::vector<int> result;

    int subStrSize = subStr.size();
    int pos = str.find(subStr);

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

  u8 getReturnCount(std::string &str)
  {
    u16 count = 0;
    count = find_all(str, "\n").size();
    for (int i = 0; i < count - 1; i++)
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
      KeyboardImpl key(11 < count ? commandLine_buffer.substr(find_all(commandLine_buffer, "\n")[count - 12] + 1) : commandLine_buffer);
      key.SetLayout(Layout::QWERTY);
      if (key.Run() <= -1)
        break;
      std::string space_delimiter = " ";
      input = key.GetInput() + space_delimiter;
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
      LS:
        u16 buff_size = commandLine_buffer.size();
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
              KeyboardImpl key(11 < count ? commandLine_buffer.substr(find_all(commandLine_buffer, "\n")[count - 12] + 1) : commandLine_buffer);
              key.SetLayout(Layout::QWERTY);
              if (key.Run() <= -1)
                break;
              if (key.GetInput() == "y")
                Directory::Remove(dir.GetFullName() + "/" + getFolderObscurely(dir, args[1]));
            }
            else if (File::Exists(dir.GetFullName() + "/" + getFileObscurely(dir, args[1])))
            {
              commandLine_buffer += "really delete " + dir.GetFullName() + "/" + getFileObscurely(dir, args[1]) + "?[y/n]:\n";
              u8 count = getReturnCount(commandLine_buffer);
              KeyboardImpl key(11 < count ? commandLine_buffer.substr(find_all(commandLine_buffer, "\n")[count - 12] + 1) : commandLine_buffer);
              key.SetLayout(Layout::QWERTY);
              if (key.Run() <= -1)
                break;
              if (key.GetInput() == "y")
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

  void Tetris_Class::GameOver(void)
  {
    MessageBox(Utils::Format("Game Over\nyour score is %d", _score))();
    _score = 0;
    _mino.turn = 0;
    _level = 0;
    _field = std::vector<std::vector<u8>>(FIELD_WIDTH, std::vector<u8>(FIELD_HEIGHT, 0));
  }

  void Tetris_Class::Restart(void)
  {
    for (UIntVector block : _mino.blocks)
    {
      if (block.y == 1)
        GameOver();

      _field[block.x][block.y] = _mino.kind + 1;
    }
    if (_field[4][0])
      GameOver();

    // 新しいミノを生成
    _mino.kind = Utils::Random(0, MINO_KINDS_COUNT - 1);
    _mino.turn = 0;
    _mino.dropClock.Restart();
    _mino.blocks = _mino_templates[0][_mino.kind];
    for (int i = 0; i < _mino.blocks.size(); i++)
      _mino.blocks[i].x += 3;
  }

  void Tetris_Class::TurnBlock(bool turn_right)
  {
    IntVector pos = {(int)((int)_mino.blocks[0].x - (int)_mino_templates[_mino.turn][_mino.kind][0].x), (int)((int)_mino.blocks[0].y - (int)_mino_templates[_mino.turn][_mino.kind][0].y)};
    if (turn_right)
    {
      if (_mino.turn == 3)
        _mino.turn = 0;
      else
        _mino.turn++;
    }
    else
    {
      if (_mino.turn == 0)
        _mino.turn = 3;
      else
        _mino.turn--;
    }
    for (UIntVector block : _mino_templates[_mino.turn][_mino.kind])
      if ((int)(block.x) + pos.x < 0 || (int)(block.y) + pos.y < 0)
        goto RESET_TURN;
    _mino.blocks = _mino_templates[_mino.turn][_mino.kind];
    for (int i = 0; i < _mino.blocks.size(); i++)
    {
      _mino.blocks[i].x += pos.x;
      _mino.blocks[i].y += pos.y;
    }
    for (UIntVector block : _mino.blocks)
      if (block.x < 0 || _field.size() - 1 < block.x || _field[0].size() - 1 < block.y || _field[block.x][block.y])
      {
        goto RESET_TURN;
      }
    return;
  RESET_TURN:
    if (turn_right)
    {
      if (_mino.turn == 0)
        _mino.turn = 3;
      else
        _mino.turn--;
    }
    else
    {
      if (_mino.turn == 3)
        _mino.turn = 0;
      else
        _mino.turn++;
    }
    _mino.blocks = _mino_templates[_mino.turn][_mino.kind];
    for (int i = 0; i < _mino.blocks.size(); i++)
    {
      _mino.blocks[i].x += pos.x;
      _mino.blocks[i].y += pos.y;
    }
  }

  void Tetris_Class::MoveMino(int moveX, int moveY)
  {
    for (int i = 0; i < _mino.blocks.size(); i++)
    {
      _mino.blocks[i].x += moveX;
      _mino.blocks[i].y += moveY;
    }
  }

  void Tetris_Class::Tetris_Loop(HotkeyManager Hotkeys)
  {
    const Screen &topScr = OSD::GetTopScreen();
    topScr.DrawRect(137, 9, 126, 222, Color::Black);
    topScr.DrawRect(139, 11, 122, 218, Color::White, false);

    if (Hotkeys[0].IsDown())
    {
      for (UIntVector block : _mino.blocks)
        if (block.x <= 0 || _field[block.x - 1][block.y])
          goto END;
      if(Hotkeys[0].IsPressed())
      {
        MoveMino(-1, 0);
        _mino.moveClock.Restart();
      }
      else if(_mino.moveClock.HasTimePassed(Seconds(0.2)))
      {
        MoveMino(-1, 0);
      }
    }
    else if (Hotkeys[1].IsDown())
    {
      for (UIntVector block : _mino.blocks)
        if (_field.size() - 1 <= block.x || _field[block.x + 1][block.y])
          goto END;
      if(Hotkeys[1].IsPressed())
      {
        MoveMino(1, 0);
        _mino.moveClock.Restart();
      }
      else if(_mino.moveClock.HasTimePassed(Seconds(0.2)))
      {
        MoveMino(1, 0);
      }
    }
    else if (Hotkeys[2].IsDown() && _mino.softdropClock.HasTimePassed(Seconds(0.08)))
    {
      for (UIntVector block : _mino.blocks)
        if (_field[0].size() - 1 <= block.y || _field[block.x][block.y + 1])
          goto END;
      MoveMino(0, 1);
      _mino.softdropClock.Restart();
    }
    else if (Hotkeys[3].IsPressed())
    {
      for (int i = 0; i < _field[0].size(); i++)
        for (UIntVector block : _mino.blocks)
          if (block.y + i >= _field[0].size() - 1 || _field[block.x][block.y + 1 + i])
          {
            for (int j = 0; j < _mino.blocks.size(); j++)
              _mino.blocks[j].y = _mino.blocks[j].y + i;
            Restart();
            goto END;
          }
    }
    else if (Hotkeys[4].IsPressed())
      TurnBlock(false);
    else if (Hotkeys[5].IsPressed())
      TurnBlock(true);
  END:

    // ミノ落下
    if (_mino.dropClock.HasTimePassed(Milliseconds(1000 - _level * 30)))
    {
      for (UIntVector block : _mino.blocks)
        if (block.y >= _field[0].size() - 1 || _field[block.x][block.y + 1])
        {
          Restart();
          break;
        }
      for (int i = 0; i < _mino.blocks.size(); i++)
        _mino.blocks[i].y++;
      _mino.dropClock.Restart();
    }

    // フィールド上のブロック描画
    for (int i = 0; i < _field.size(); i++)
      for (int j = 0; j < _field[i].size(); j++)
        if (_field[i][j])
          topScr.DrawRect(140 + i * BLOCK_WIDTH + 1, BLOCK_WIDTH + j * BLOCK_WIDTH + 1, BLOCK_WIDTH - 2, BLOCK_WIDTH - 2, _colorfulMode ? _mino_colors[_field[i][j]] : _mino_colors[0]);

    // 落下中のミノ描画
    for (UIntVector block : _mino.blocks)
      topScr.DrawRect(140 + block.x * BLOCK_WIDTH + 1, BLOCK_WIDTH + block.y * BLOCK_WIDTH + 1, BLOCK_WIDTH - 2, BLOCK_WIDTH - 2, _colorfulMode ? _mino_colors[_mino.kind + 1] : _mino_colors[0]);

    // 落下位置描画
    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
      for (UIntVector block : _mino.blocks)
      {
        if (block.y + i >= FIELD_HEIGHT - 1 || _field[block.x][block.y + 1 + i])
        {
          for (int j = 0; j < _mino.blocks.size(); j++)
          {
            for (UIntVector block : _mino.blocks)
            {
              u32 y = block.y + i;
              topScr.DrawRect(140 + block.x * BLOCK_WIDTH + 1, BLOCK_WIDTH + y * BLOCK_WIDTH + 1, BLOCK_WIDTH - 2, BLOCK_WIDTH - 2, _colorfulMode ? _mino_colors[_mino.kind + 1] : _mino_colors[0], false);
            }
            goto SUPER_BREAK;
          }
        }
      }
    }

  SUPER_BREAK:

    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
      u8 count = 0;
      for (int j = 0; j < FIELD_WIDTH; j++)
        count += _field[j][i] ? 1 : 0;
      if (count == FIELD_WIDTH)
      {
        _score++;
        // 10ごとにレベルを上げる
        if (!(_score % 10) && _level < 11)
          _level++;

        for (int k = i - 1; k >= 0; k--)
          for (int j = 0; j < FIELD_WIDTH; j++)
            _field[j][k + 1] = _field[j][k];
      }
    }
  }

  Tetris_Class *Tetris_Class::_instance = nullptr;
  
  void Tetris(MenuEntry *entry)
  {
    Tetris_Class::GetInstance()->Tetris_Loop(entry->Hotkeys);
  }

  void SetTetrisSetting(MenuEntry *entry)
  {
    s8 answer;
    if (0 <= (answer = Keyboard("tetris color", {"monochrome", "colorful"}).Open()))
      Tetris_Class::GetInstance()->SetColorful(answer);
    if (0 <= (answer = Keyboard("level", {"easy", "normal", "difficult"}).Open()))
      Tetris_Class::GetInstance()->SetLevel(answer * 5);
  }
}