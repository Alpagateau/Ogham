#include <ctime>
#include <iostream> 
#include "../ogham.hpp"
#include <string>

int main()
{
  set_utf8(); 

  ogm::window win;
  ogm::widget small(0.5f, 0.5f, true);
  ogm::text hello(1.0f, 0.5f);
  
  win.bordered = true;
  win.colored = true;

  small.colored = true;
  small.base_char.foreground = BLACK;
  small.base_char.background = BLUE;
  
  hello.bordered = true;
  hello.base_char.foreground = RED;
  hello.base_char.background = BLUE;
  hello.label = L"Hello world\nHow are you ?";
  hello.SetAnchorType(XMIDDLE | YSTART);
  
  win.WriteAt(L"ABC\nBC\nC", 0, 0);

  small.WriteAt(L"ΩΨ3", 0, 0);
  small.WriteAt(L"Ψ3", 1, 0);
  small.WriteAt(L'3', 2, 0);
  small.SetAnchorType(XMIDDLE | YEND);
  
  //creates hierachy 
  win.addChild(&small);
  small.addChild(&hello);
  
  //Event loop 
  while(true){
    win.show();
  }
  return 0;
}
