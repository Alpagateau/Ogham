/* Copyright (C) 2024 Martin Nadaud - All Rights Reserved
* 
* You may use, distribute and modify this code under the
* terms of the MIT license, which unfortunately won't be
* written for another century.
*
* You should have received a copy of the MIT license with
* this file. If not, please write to: alpaga.ytb@gmail.com, or visit : github.com/Alpagateau/
*/

#ifndef OGHAM_HPP
#define OGHAM_HPP

#pragma once 
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cctype>
#include <cwchar>
#include <codecvt>
#include <locale>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#define cursor(x, y) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){x, y})
#elif defined(__linux__)
#include <unistd.h>
#include <sys/ioctl.h>
#define cursor(x,y) printf("\033[%d;%dH", x, y)
#endif // Windows/Linux 

void get_terminal_size(int& width, int& height) {
  #if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = (int)(csbi.srWindow.Right-csbi.srWindow.Left+1);
    height = (int)(csbi.srWindow.Bottom-csbi.srWindow.Top+1);
#elif defined(__linux__)
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = (int)(w.ws_col);
    height = (int)(w.ws_row);
#endif // Windows/Linux
}

int set_utf8()
{
#if defined(__linux__)
  try {
      std::locale::global(std::locale("en_US.UTF-8"));
  } catch (const std::exception& e) {
      std::cerr << "Locale setting error: " << e.what() << std::endl;
      return 1; // Exit if locale setting fails
  }
#elif defined(_WIN32)
  SetConsoleOutputCP(65001);
#endif
  return 0;
}

std::wstring str2wstr(const std::string& str) {
    // Create a wide string converter
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string appndWcharToStr(const std::string& original, wchar_t wideChar) {
    // Create a converter to convert wide characters to UTF-8
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    // Convert the wide character to a UTF-8 string
    std::wstring wideString(1, wideChar); // Create a wstring with the wide character
    std::string utf8String = converter.to_bytes(wideString);
    // Append the UTF-8 string to the original string
    return original + utf8String;
}

//defines 
#define START 0
#define MIDDLE 1
#define END 2
#define NEXT 3
#define XSTART 0
#define XMIDDLE 1
#define XEND 2
#define XNEXT 3
#define YSTART 0
#define YMIDDLE 4
#define YEND 8
#define YNEXT 12

//colors 
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

namespace ogm{
  struct tchar
  {
    wchar_t value; // using string to represent unicode...
    uint8_t foreground;
    uint8_t background;
  };
  class widget {
  public:
    int width, height;
    float wratio, hratio;
    int anchorType = 0;
    bool bordered = false;
    bool relative = false;
    bool colored = false;
    tchar base_char = {L' ', WHITE, BLACK};
    // Use Xpos | Y pos to get the total position 
    // The NEXT constant puts the position at the xoffset or y offset
    std::vector<widget*> children; 
    std::vector<tchar> buffer; //internal buffer    
    std::vector<tchar> rbuffer; //render buffer 
    widget* parent = NULL;
    //methods 
    widget(int w, int h){
      width = w;
      height = h;
    } 
     
    widget(float wr, float hr)
    {
      wratio = wr;
      hratio = hr;
      get_terminal_size(width, height);
      height--;
      relative = true;
    }
    
    bool SetAnchorType(int at)
    {
      anchorType = at;
      return true;
    }

    bool addChild(widget* child)
    {
      children.push_back(child);
      child->parent = this;
      if(child->relative)
      {
        child->width = width;
        child->height = height;
      }
      return true;
    } 
    
    bool resize(int nw, int nh)
    {
      int boff = bordered ? 1 : 0;
      tchar oldb[width-(2*boff)][height-(2*boff)];
      for(int i = 0; i < width-(2*boff); i++)
      {
        for(int j = 0; j < height-(2*boff); j++)
        {
          int idx = j*(width-(2*boff)) + i;
          
          if(idx < buffer.size())
            oldb[i][j] = buffer.at(j*(width-(2*boff))+ i);
          else {
            oldb[i][j] = base_char;
          }
        }
      }
      int oldw = width-(2*boff);
      int oldh = height-(2*boff);
      width = nw;
      height = nh;
      buffer.clear();
      buffer = std::vector<tchar>(1,base_char);
      for(int i = 0; i < nw; i++)
      {
        for(int j = 0; j < nh; j++)
        {
          if(i < oldw && j < oldh)
            WriteAt(oldb[i][j], i, j);
          else {
            WriteAt(' ', i, j);
          }
        }
      } 
      
      return true;
    }
    
    bool WriteAt(wchar_t c, int x, int y)
    {
      return WriteAt(
      (tchar){c, base_char.foreground, base_char.background},
      x,
      y
      );
    }

    bool WriteAt(tchar c, int x, int y)
    {
      int boff = bordered ? 2 : 0;
      if(x >= width-boff)
      {
        //std::cout << x << "is bigger than the width " << width << std::endl;
        return false;
      }
      if(y >= height-boff)
      {
        //std::cout << y << "is bigger than the height " << height << std::endl;
        return false;
      }
      if(!std::isprint(c.value))
      {
        return false;
      }
      
      int i = y*width + x; 
      if(bordered)
      {
        i = y*(width-2)+x;
      }
      int s = buffer.size(); 
      if(i < s)
      {
        buffer.at(i) = c;
      } 
      else
      {
        for(int j = 0; j <= i-s; j++)
        { 
          buffer.push_back(base_char);
        }
        buffer.at(i) = c;
      }
      
      return true;
    }
    
    bool WriteAt(std::wstring msg, int x, int y)
    {
      int px = x;
      int py = y;
      
      for(int i = 0; i < msg.size(); i++)
      { 
        if(msg[i] == '\n')
        {
          py++;
          px = 0;
        }
        else
        {
          WriteAt(msg[i], px, py);
          px++;
        }
      }
      
      return true;
    }
    
    virtual bool render()
    {
      //first, init FULL vector 
      int boff = bordered ? 1:0;
      rbuffer = std::vector<tchar>(width*height, base_char);
      for(int i = 0; i<buffer.size(); i++)
      {
        int ni = i;
        if(bordered)
        {
          int x = i%(width-2);
          int y = (i-x)/(width-2);
          ni = (y+1)*width + x+1;
        }
        if(ni < rbuffer.size())
          rbuffer[ni] = buffer[i];
        else 
        {
          throw std::out_of_range(
            "ni is too big here "
            + std::to_string(ni)
            );
        }
      } // internal buffer pass 
      //Children pass
      int offx = 0;
      int offy = 0;
      for (int i = 0; i < children.size(); i++)
      { 
          int W = children[i]->width;
          int H = children[i]->height;
          float cwr = children[i]->wratio;
          float chr = children[i]->hratio;
          if(children[i]->relative)
          {
            children[i]->resize(
              (int)(width * cwr), 
              (int)(height* chr)
            );
            W = children[i]->width;
            H = children[i]->height;
          }
          children[i]->render();
 
          //placing 
          int ap = children[i]->anchorType;
          int xa = ap % 4;
          int ya = (ap >> 2) % 4;
          int xpos = 0; // right 
          int ypos = 0; // top
          
          switch (xa) {
            case 1: //middle 
              xpos = (width/2)-(W/2);
              break;
            case 2: //end 
              xpos = width-W;
              break;
            case 3:
              xpos = offx;
              break;
            default:
              break;
          } 
          switch (ya) {
            case 1: //middle 
              ypos = (height/2)-(H/2);
              break;
            case 2: //end
              ypos = height-H;
              break;
            case 3:
              ypos = offy;
              break;
            default:
              break;
          }

          //copying into rbuffer
          for (int x = 0; x < W; x++) {
             for (int y = 0; y < H; y++) {
                if(x + xpos < width && y + ypos < height)
                {
                  if(x+xpos >= 0 && y+ypos>=0){
                    int pos = (y+ypos)*width + x+xpos; 
                    rbuffer[pos] = children[i]->rbuffer[y*W + x];
                  }
                }
             }
          }
          //end rendering
          offx = children[i]->width+1;
      }
      //borders 
      if(bordered)
      {
        for (int i = 0; i < width; i++) {
          rbuffer[i] = 
            {L'━', base_char.foreground, base_char.background};
          rbuffer[(height-1)*width + i] = 
            {L'━', base_char.foreground, base_char.background};
        }
        for (int i = 0; i < height; i++) {
          rbuffer[i*width] = 
            {L'┃', base_char.foreground, base_char.background};
          rbuffer[(i+1)*width - 1] = 
            {L'┃', base_char.foreground, base_char.background};
        }
        rbuffer[0] = {L'┏', base_char.foreground, base_char.background};
        rbuffer[width-1]= {L'┓', base_char.foreground, base_char.background};
        rbuffer[(height-1)*width] = {L'┗', base_char.foreground, base_char.background};
        rbuffer[height*width - 1] = {L'┛', base_char.foreground, base_char.background};
      }
      return true;
    }
     
    void d_display()
    {
      for(int i = 0; i < width*height; i++)
      {
        if(i % width == 0)
        {
          std::cout << std::endl;
        }
        if(i < buffer.size())
          std::cout << buffer[i].value;
        else 
          std::cout << ' ';
      }
      std::cout << std::endl;
    }
      
    void d_render()
    {
      render();
      for(int i = 0; i < width*height; i++)
      {
        if(i % width == 0 && i > 0)
        {
          std::cout << std::endl;
        }
        if(i < rbuffer.size())
          std::cout << rbuffer[i].value;
        else 
          std::cout << ' ';
      }
    }
    
    void show()
    {
      render();
      std::string l = "";
      cursor(0,0);
      int fore = 0;
      int back = 0;
      for(int i = 0; i < height; i++)
      {
        for(int j = 0; j < width; j++)
        {
          wchar_t nc = L' ';
          if(!colored)
          {
            nc = rbuffer[i*width + j].value;
            l = appndWcharToStr(l,nc);
          }
          else 
          {
            std::string cstr = "";
            tchar ntc = rbuffer[i*width+j];
            if(ntc.foreground != fore || ntc.background != back)
            {
              fore = ntc.foreground;
              back = ntc.background;
              cstr += "\033[" 
                  + std::to_string(30+fore) + ";"
                  + std::to_string(40+back) + "m"; 
            } 
            cstr = appndWcharToStr(cstr,ntc.value);
            l += cstr;
          }
        } 
        l += '\n';
      }
      std::cout << l; 
    }
  };  

  class window : public widget
  {
  public:
    window() : widget(0,0)
    {
      get_terminal_size(width,height); 
      //std::cout << "init window " << height << "|" << width <<std::endl;
      //width = w.ws_col;
      height--;
    }
    
    void show()
    {
      int nw = 0, nh = 0;
      get_terminal_size(nw, nh);
      if(nw != width || nh-1 != height)
        widget::resize(nw, nh-1);
      widget::show();
    }
  };
  
  class text : public widget 
  {
  public:
    std::wstring label;
    bool wrap = false;
    text(int w, int h) : widget(w, h){}
    text(float rw, float rh) : widget(rw, rh){};
    
    bool render()
    {
      widget::WriteAt(label, 0, 0);
      return widget::render();
    }
  };
}
#endif
