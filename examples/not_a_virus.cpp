#include <iostream>
#include "../ogham.hpp"

int main() 
{
  set_utf8();
  ogm::window win;
  win.bordered = true;
  win.colored= true;
  win.base_char = (ogm::tchar){L' ', WHITE, RED};
  
  ogm::widget cwin(30,10,true);
  cwin.colored = true;
  cwin.SetAnchorType(XMIDDLE | YMIDDLE);
  cwin.base_char = (ogm::tchar){L' ', RED, WHITE};
  
  win.addChild(&cwin);

  int counter = 0;
  std::wstring fullmsg = L"Tihihi \nYour computer is mine";
  std::wstring msg;
  while(true)
  {
    win.show();
    if((counter/2)< fullmsg.size() && counter%2)
    {
      msg += fullmsg[counter/2];
      if(fullmsg[counter/2] == L'\n')
      {
        sleep(5);
      }
    }
    else if (counter/2 > fullmsg.size())
    {
      win.base_char = (ogm::tchar){L'ඞ', WHITE, RED};
    }
    cwin.WriteAt(msg, 0,0);
    counter++;
    sleep(0.5);
  }
  return 0;
}
