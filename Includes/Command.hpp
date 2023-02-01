#pragma once

#include <CTRPluginFramework.hpp>
#include "types.h"

namespace CTRPluginFramework
{
  using StringVector = std::vector<std::string>;

  class Command
  {
  public:
    void Command_Loop();
    static Command *GetInstance()
    {
      if (!_instance)
        _instance = new Command();
      return _instance;
    }
  
  private:
    static Command *_instance;

    std::vector<int> FindAll(const std::string str, const std::string subStr);
    std::string TreeFolder(std::string path, u8 space);
    std::string GetFileObscurely(Directory dir, std::string word);
    std::string GetFolderObscurely(Directory dir, std::string word);
    size_t GetReturnCount(std::string &str);
    void Cd(Directory &dir, std::string &path, std::string &str);
    void GetEntries(MenuFolder *folder, std::vector<MenuEntry *> &entries);

    Command();
    ~Command();
  };
}