#pragma once

#include <vector>
#include<windows.h>
#include<string>

using namespace std;

enum WeatherType 
{
    WEATHER_UNKNOWN,
    WEATHER_THUNDERSTORM,
    WEATHER_DRIZZLE,
    WEATHER_RAIN,
    WEATHER_SNOW,
    WEATHER_CLEAR,
    WEATHER_CLOUDS,
    WEATHER_MIST,
    WEATHER_SMOKE,
    WEATHER_HAZE,
    WEATHER_DUST,
    WEATHER_SAND,
    WEATHER_FOG,
    WEATHER_SQUALL,
    WEATHER_TORNADO,
};

class Console {
public:
    static void gotoxy(int x = 0, int y = 0)
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos{ static_cast<SHORT>(x), static_cast<SHORT>(y) };
        SetConsoleCursorPosition(h, pos);
    }
}; //gotoxy(x,y) for coordinate

struct LoadingConfig {
    int bar = 100;
    int c1 = 3;
    int c2 = 6;
    int c3 = 14;
    int c4 = 1;
    int speed = 20;
};

struct menu
{
    string text = "";
    int c1 = 7;
    int c2 = 7;
    int c3 = 7;
};

int getConsoleWidth();//get console width

int getConsoleLength();//get console length

struct UIcfg 
{
    double widthDv;

    int getWidth()
    {
        if (widthDv <= 1.0)
        {
            widthDv = 1.0;
            return getConsoleWidth() - 3;
        }
        else
        {
            return getConsoleWidth() / widthDv;
        }
    }
};

extern const vector<string> Icon;

extern const vector<string> IconSmall;

void printCenter(const string& text, bool check = 1);//print the text at the center(horizontal) , 1 stand for endl, if 0 then after the text will not cout endl

void printPerfectCenter(const string& text);// just perfect center (horizontal and vertical)

void setPrintBox(const string& text, int num, const string &head, const string &tail);//print the box at the center(horizontal), text for "─", num for how long, head for "┌", tail for "┐"

void printFull(const string& text, int space = 0);//print full horizontal center example "-", space for how many space should be left on the side

void printPic(const vector<string> &pic, int c = 7);//print any ASCII ART 

void printUI(int c=7);//print NDIS

int findCenterY();//just find center

void setColour(int colour = 7); //0 黑 1 蓝 2 绿 3 青 4 红 5 紫 6 黄 7 白 8 灰 9 亮蓝 10 亮绿 11 亮青 12 亮红 13 粉 14 亮黄 15 亮白

void sendUserMsg(const string& text, int c1, int c2); //at the center, text for massage c1 for border colour,c2 for text colour

void Loading(LoadingConfig cfg = {});//reference to struct at the top modify using example: Loading({.speed=2}) other I m a bit lazy to say,just test it sryyy

void printPageMenu(menu cfg = {}, UIcfg = {}); //the header which show NDIS Icon

void printCinBar(int c1 = 7, UIcfg = {},int ext = 0); //print the bar for user to cin smth

void printClear(const string& text, UIcfg = {} ,int speed = 0,int ext = 0); // clear the and show the message 

void printSocialSystem();

void printEconomicSystem();

void printEandCSystem();

void printNewSystem();