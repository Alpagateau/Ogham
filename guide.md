# OGHAM 
Ogham hpp (᚛ᚑᚌᚆᚐᚋ᚜) is a terminal based widget manager, aiming to create UI that is both simple to code, and simple to navigate. 
It has been created with ease of use in mind, and cross-platform as a need. 
It supports UTF8, colored text, and resizing windows. 

## How to use 
First of all, just import **Ogham** in your program. 
```c++
#include "ogham.hpp"
```
The classes are under the `ogm` namespace, when most utilities are directly accessible. 
The availables classes and structs are :
- tchar 
- widget 
- window 
- text 

### tchar 
A `tchar` (or terminal char) is a struct containing a character and its colors. 

### widget 
The widget is the main component of this library. It's simply a rectangle of known size containing a buffer of `tchar`s and a tree of widgets. 

```bash 
g++ main.cpp -o main static-libgcc -static-libstdc++ -static 
```
