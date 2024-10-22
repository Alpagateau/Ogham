#include <ctime>
#include <iostream> 
#include "ogham.hpp"
#include <string>

int main()
{
  set_utf8(); 
  std::cout << "Hello World!" << std::endl;
  ogm::window win;
  ogm::widget small(0.5f,0.5f);
  ogm::text hello(1.0f, 0.5f);

  //ogm::widget small(10,10);
  win.bordered = true;
  win.colored = true;
  small.bordered = true;
  small.colored = true;

  small.base_char.foreground = BLACK;
  small.base_char.background = BLUE;
  
  hello.base_char.foreground = RED;
  hello.bordered = true;
  hello.label = L"Hello world\nHow are you ?";
  hello.SetAnchorType(XMIDDLE | YSTART);
  win.WriteAt(L"ABC", 0, 0);
  win.WriteAt(L"BC", 0, 1);
  win.WriteAt('C', 0, 2);

  small.WriteAt(L"ΩΨ", 0, 0);
  small.WriteAt(L'Ψ', 1, 0);
  small.WriteAt(L'3', 2, 0);
  small.WriteAt(L'3', 1, 1);
  small.WriteAt(L'3', 0, 2);
  small.SetAnchorType(XMIDDLE | YEND);

  win.addChild(&small);
  small.addChild(&hello);
  //hello.WriteAt('A', 3, 3);
  int oldtime = clock();
  //std::cout << hello.label << std::endl;
  //sleep(4); 
  while(true){
    int ntime = clock();
    int delta = (ntime - oldtime);
    //small.WriteAt("delta :  " + std::to_string(delta) + "  ", 0, 0);
    win.show();
    oldtime = clock();
  }
  return 0;
}
