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
    static Command &GetInstance()
    {
      static Command instance;
      return instance;
    }

    Command(Command const &) = delete;
    Command &operator=(Command const &) = delete;
    Command(Command &&) = delete;
    Command &operator=(Command &&) = delete;
  
  private:
    std::vector<int> FindAll(const std::string str, const std::string subStr);
    std::string TreeFolder(std::string path, u8 space);
    std::string GetFileObscurely(Directory dir, std::string word);
    std::string GetFolderObscurely(Directory dir, std::string word);
    size_t GetReturnCount(std::string &str);
    void Cd(Directory &dir, std::string &path, std::string &str);
    void GetEntries(MenuFolder *folder, std::vector<MenuEntry *> &entries);

    Command() = default;
    ~Command() = default;
  };
}