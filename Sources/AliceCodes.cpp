#include "AliceCodes.hpp"
#include <CTRPluginFramework.hpp>
#include <CTRPluginFrameworkImpl.hpp>

namespace CTRPluginFramework
{
  Result AliceCodes::SetTopScreenBackground(const std::string &filename, bool resize)
  {
    BMPImage *image = new BMPImage(filename);

    if (!image->IsLoaded())
    {
      delete image;
      return -1;
    }

    if (image->Width() != 340 || image->Height() != 200)
    {
      if (resize)
      {
        image->SetWidthHeight(340, 200);
        image->SaveImage(filename.substr(0, filename.length() - 4) + "(top).bmp");
        delete image;
        BMPImage *image = new BMPImage(filename.substr(0, filename.length() - 4) + "(top).bmp");
      }
      else
      {
        delete image;
        MessageBox(Color::Red << "An error has occurred.\nfile size is wrong")();
        return -1;
      }
    }

    Preferences::topBackgroundImage = image;
    Window::UpdateBackgrounds();
    return 0;
  }

  Result AliceCodes::SetBottomScreenBackground(const std::string &filename, bool resize)
  {
    BMPImage *image = new BMPImage(filename);

    if (!image->IsLoaded())
    {
      delete image;
      return -1;
    }

    if (image->Width() != 280 || image->Height() != 200)
    {
      if (resize)
      {
        image->SetWidthHeight(280, 200);
      }
      else
      {
        delete image;
        MessageBox(Color::Red << "An error has occurred.\nfile size is wrong")();
        return -1;
      }
    }

    Preferences::bottomBackgroundImage = image;
    Window::UpdateBackgrounds();
    return 0;
  }
}