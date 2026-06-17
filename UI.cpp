#include <iostream>
#include "UI.h"
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <format>

using namespace std;

struct AsciiTimeData 
{
    int Time_F = 0;
    int Time_B = 0;
    int Time_Zone = 0;
    bool isPM = true;
};

const vector<string> Icon =
{
    "ooooo      ooo oooooooooo.   ooooo  .oooooo..o",
    " `888b.    `8' `888'   `Y8b  `888' d8P'    `Y8",
    " 8 `88b.    8   888      888  888  Y88bo.     ",
    " 8   `88b.  8   888      888  888   `'Y8888o. ",
    " 8     `88b.8   888      888  888       `'Y88b",
    " 8       `888   888     d88'  888  oo     .d8P",
    "o8o        `8  o888bood8P'   o888o `'888888P' ",
};

const vector<string> IconSmall =
{
    "      __     __ ",
    "|\\ | |  \\ | /__`",
    "| \\| |__/ | .__/"
};

const vector<string> Num =
{
    "  ████  " ,
    " ██  ██ ",
    "██  ████",
    "██ ██ ██",
    "████  ██",
    " ██  ██ ",
    "  ████  ",

    "   ██   ",
    " ████   ",
    "   ██   ",
    "   ██   ",
    "   ██   ",
    "   ██   ",
    "████████",

    "  █████ ",
    "██    ██",
    "      ██",
    "  █████ ",
    " ██     ",
    "██      ",
    "████████",

    " ██████ ",
    "██    ██",
    "      ██",
    "  █████ ",
    "      ██",
    "██    ██",
    " ██████ ",

    "   ████ ",
    "  ██ ██ ",
    " ██  ██ ",
    "██   ██ ",
    "████████",
    "     ██ ",
    "     ██ ",

    "████████",
    "██      ",
    "███████ ",
    "      ██",
    "██    ██",
    "██    ██",
    " ██████ ",

    " ██████ ",
    "██    ██",
    "██      ",
    "███████ ",
    "██    ██",
    "██    ██",
    " ██████ ",

    "████████",
    "██    ██",
    "     ██ ",
    "    ██  ",
    "   ██   ",
    "   ██   ",
    "   ██   ",

    " ██████ ",
    "██    ██",
    "██    ██",
    " ██████ ",
    "██    ██",
    "██    ██",
    " ██████ ",

    " ██████ ",
    "██    ██",
    "██    ██",
    " ███████",
    "      ██",
    "██    ██",
    " ██████ "
};

const vector<string> Ex =
{
    " __||___           ",
    "/______\\\\__      ",
    "\\_________/       ",
    "~v~V~v~V~v~V~v~V~v~"
};

const vector<string> Im =
{
    "           ___||__ ",
    "        __//______\\",
    "        \\_________/",
    "~v~V~v~V~v~V~v~V~v~"
};

const vector<string> PM =
{
    "████████   ███      ███",
    "██     ██  ████    ████",
    "██     ██  ██ ██  ██ ██",
    "████████   ██  ████  ██",
    "██         ██   ██   ██",
    "██         ██        ██",
    "██         ██        ██"
};

const vector<string> AM =
{
    "   ███     ███      ███",
    "  ██ ██    ████    ████",
    " ██   ██   ██ ██  ██ ██",
    "█████████  ██  ████  ██",
    "██     ██  ██   ██   ██",
    "██     ██  ██        ██",
    "██     ██  ██        ██"
};

const vector<string> Dot =
{

    "  ",
    "  ",
    "██",
    "  ",
    "  ",
    "██",
    "  "
};

void printNum (int num, int x,int y,bool colour = true)
{
    int a = num / 10;  
    int b = num % 10;
    int c = 7;

    a *= 7;
    b *= 7;

    if (colour)
    {
        if (num >= 73) c = 2;
        if (num < 73) c = 6;
        if (num < 60) c = 4;
    }

    setColour(c);

    for (int i = 0; i < 7; i++)
    {
        Console::gotoxy(x,y+i);
        cout << Num[a] << endl;
        a++;
    }

    for (int i = 0; i < 7; i++)
    {
        Console::gotoxy(x+10, y + i);
        cout << Num[b] << endl;
        b++;
    }

    setColour(7);
}

void printVec(const vector<string> &vec ,int colour, int x, int y)
{
    for (int i = 0; i < 7; i++)
    {
        Console::gotoxy(x, y + i);
        cout << vec[i] << endl;
    }
}

int getConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int getConsoleLength() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

long long findBigNum(const vector<long long>& array) 
{
    long long big = array[0];

    for (int i = 1; i < array.size(); i++) {
        if (array[i] > big) {
            big = array[i];
        }
    }

    return big;
}

void printCenter(const string&text, bool newLine)
{
    int width = getConsoleWidth();
    int paddingWidth = (width - text.length()) / 2;

    for (int i = 0; i < paddingWidth; i++)
        cout << " ";

    cout << text;

    if (newLine) cout << endl;
}

void printPerfectCenter(const string& text)
{
    int userScreen = getConsoleWidth();
    int center = findCenterY();

    int setPos = ((userScreen - text.size()) / 2);

    Console::gotoxy(setPos, center);cout << text;
}

void setPrintBox(const string& text, int num, const string& head, const string& tail)
{
    int width = getConsoleWidth();
    int paddingWidth = (width - (num+2)) / 2;
    paddingWidth = max(0, paddingWidth);

    for (int i = 0; i < paddingWidth; i++) cout << " ";
    cout << head;

    for (int i = 0; i < num; i++) cout << text;
    cout << tail;

    cout << endl;
}

void printFull(const string& text, int space)
{
    int width = getConsoleWidth();

    if(space!=0)for (int i = 0; i < space; i++) cout << " ";
    
    for (int i = 0; i < width-(space*2); i++) cout << text;

    cout << endl;
}

void printPic(const vector<string> &pic, int c)
{
    setColour(c);
    int width = getConsoleWidth();
    int offset = (width - pic[0].length()) / 2;

    

    for (int i = 0; i < pic.size(); i++) 
    {
        for (int i = 0; i < offset; i++) cout << " ";
        cout << pic[i] << endl;
    }
}

void printUI(int c)
{
    cout << endl;

    printPic(Icon,c);

    cout << endl;
}

int findCenterY()
{
    int length = getConsoleLength();
    int paddingLength = (length / 2) - 1;

    return paddingLength;
}

void setColour(int colour)
{
    //0 黑 1 蓝 2 绿 3 青 4 红 5 紫 6 黄 7 白 8 灰 9 亮蓝 10 亮绿 11 亮青 12 亮红 13 粉 14 亮黄 15 亮白
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, colour);
}

void printPageMenu(menu cfg,UIcfg Uicfg)
{
    system("cls");

    setColour(cfg.c1);

    setPrintBox("─", Uicfg.getWidth(), "┌", "┐");

    printUI(cfg.c2);

    setColour(cfg.c1);

    setPrintBox("─", Uicfg.getWidth() , "├", "┤ ");

    if (!cfg.text.empty()) 
    {
        setColour(cfg.c3);

        printCenter(cfg.text);

        setColour(cfg.c1);

        setPrintBox("─", Uicfg.getWidth(),  "├", "┤ ");
    }
    setColour(7);
}

void printCinBar(int c1,UIcfg cfg,int ext)
{

    int width = getConsoleWidth();
    int cursorPos = (width - (cfg.getWidth() + 2)) / 2;
    cursorPos = max(0, cursorPos);

    int AxisY = 30 + ext;
    
    //string Error = "Please Enter valid input";

    setColour(c1);
    Console::gotoxy(0, AxisY);setPrintBox("─", cfg.getWidth(), "├", "┤ ");

    Console::gotoxy(cursorPos + 2, (AxisY + 2));cout << "►" << endl;

    Console::gotoxy(0, (AxisY + 4));setPrintBox("─", cfg.getWidth(), "└", "┘");

    Console::gotoxy(cursorPos + 4, (AxisY + 2));
    setColour(7);

    /*
    Console::gotoxy(4, 32);cout << Error;

    Sleep(500);

    Console::gotoxy(4, 32); cout << Error.length();;

    Console::gotoxy(4, 32);
    */
}

void printClear(const string& text,UIcfg cfg, int speed,int ext)
{
    int width = getConsoleWidth();
    int cursorPos = (width - (cfg.getWidth() + 2)) / 2;
    cursorPos = (max(0, cursorPos)) +4;

    int AxisY = 30 + ext;

    if (speed != 0) { setColour(8);Console::gotoxy(cursorPos, AxisY + 2); cout << text; setColour(7); }

    Sleep(speed);

    Console::gotoxy(cursorPos, AxisY + 2); cout << string(text.length()+1, ' '); Console::gotoxy(cursorPos, AxisY + 2);
}


void Loading(LoadingConfig cfg)
{
    system("cls");
    int userScreen = getConsoleWidth();
    int center = findCenterY();
    int barWidth = cfg.bar + 2;

    setColour(cfg.c1);

    Console::gotoxy(0, center - 1);
    setPrintBox("─", cfg.bar, "┌", "┐");

    Console::gotoxy(0, center);
    setPrintBox(" ", cfg.bar, "│", "│");


    Console::gotoxy(0, center + 1);
    setPrintBox("─", cfg.bar, "└", "┘");

    int setPos = ((userScreen - barWidth) / 2) + 1;

    setColour(cfg.c2);

    for (int i = 0; i < cfg.bar; i++)
    {
        Console::gotoxy(setPos + i, center);cout << "░";
    }

    setColour(cfg.c3);

    Console::gotoxy(0, center + 3);printCenter(". . .Loading. . .");

    for (int i = 0; i < cfg.bar; i++)
    {
        Console::gotoxy(setPos + i, center);cout << "█";
        Console::gotoxy(setPos + barWidth, center);cout << i + 1 << "%";
        Sleep(cfg.speed);
    }

    setColour(cfg.c4);

    Console::gotoxy(0, center + 3);printCenter(" . . .Complete. . .", 0);

    Sleep(300);

    system("cls");
}

void sendUserMsg(const string& text, int c1, int c2)
{
    system("cls");
    int center = findCenterY();
    int bar = text.size() + 2;

    setColour(c1);

    Console::gotoxy(0, center - 1);
    setPrintBox("─", bar, "┌", "┐");

    Console::gotoxy(0, center);
    setPrintBox(" ", bar, "│", "│");


    Console::gotoxy(0, center + 1);
    setPrintBox("─", bar, "└", "┘");

    setColour(c2);

    printPerfectCenter(text);

    cin.ignore();

    system("cls");
}

string SimplyNum(double Num)
{
    if(Num < 1000) return to_string((int)Num);

    double division[] = { 1000000000000000,1000000000000,1000000000,1000000,1000 };
    string unit[] = {"P","T","B","M","K"};
    
    for (int i = 0; i < 5; i++)
    {
        if (Num >division[i])
        {
            int temp = Num / division[i];
            return to_string(temp) + unit[i];
        }
    }

    return to_string((int)Num);
}

string P_N(double n)
{
    string temp = to_string(abs((int)n));
    string s = "";
    
    n < 0 ? s += "-" : n > 0 ? s += "+" : temp = "0";

    return s += temp;
}

AsciiTimeData getDigitsForAscii(long long raw_timestamp, int timezone_offset) 
{
    time_t local_timestamp = static_cast<time_t>(raw_timestamp + timezone_offset);

    struct tm timeInfo;

    gmtime_s(&timeInfo, &local_timestamp);

    AsciiTimeData data;

    int hour24 = timeInfo.tm_hour;
    int minute = timeInfo.tm_min;

    data.isPM = (hour24 >= 12);

    int hour12 = hour24 % 12;
    if (hour12 == 0) hour12 = 12;

    data.Time_F = hour12;
    data.Time_B = minute;

    data.Time_Zone = timezone_offset / 3600;

    return data;
}

WeatherType resolveWeather(const std::string& weather_str) 
{
    static const map<string, WeatherType> weatherMap = 
    {
        {"Thunderstorm",WEATHER_THUNDERSTORM},
        {"Drizzle",     WEATHER_DRIZZLE},
        {"Rain",        WEATHER_RAIN},
        {"Snow",        WEATHER_SNOW},
        {"Clear",       WEATHER_CLEAR},
        {"Clouds",      WEATHER_CLOUDS},
        {"Mist",        WEATHER_MIST},
        {"Smoke",       WEATHER_SMOKE},
        {"Haze",        WEATHER_HAZE},
        {"Dust",        WEATHER_DUST},
        {"Sand",        WEATHER_SAND},
        {"Fog",         WEATHER_FOG},
        {"Squall",      WEATHER_SQUALL},
        {"Tornado",     WEATHER_TORNADO}

    };

    auto it = weatherMap.find(weather_str);
    if (it != weatherMap.end()) {
        return it->second;
    }

    return WEATHER_UNKNOWN;
}

void getWeather(const string &weather,int x ,int y)
{
    WeatherType type = resolveWeather(weather);
    
    switch(type)
    {
    case WEATHER_THUNDERSTORM:

        Console::gotoxy(x, y);setColour(7);  cout << "         " << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << "    _.-.  " << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << "  (_(   ) " << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << " (____(__)" << endl;
        Console::gotoxy(x, y + 4);setColour(9);cout << " ʻ⚡,ʻ,⚡ʻ" << endl;
        Console::gotoxy(x, y + 5);setColour(9);cout << " ʻ,ʻ⚡,ʻ,ʻ" << endl;
        Console::gotoxy(x, y + 6);setColour(7);cout << "          " << endl;
        setColour(7);
        break;

    case WEATHER_DRIZZLE:

        Console::gotoxy(x, y);setColour(7);  cout <<   "         " << endl;
        Console::gotoxy(x, y + 1);setColour(7);cout << "    _.-.  " << endl;
        Console::gotoxy(x, y + 2);setColour(7);cout << "  (_(   ) " << endl;
        Console::gotoxy(x, y + 3);setColour(7);cout << " (____(__)" << endl;
        Console::gotoxy(x, y + 4);setColour(1);cout << "  ʻ ʻ ʻ ʻ " << endl;
        Console::gotoxy(x, y + 5);setColour(1);cout << " ʻ ʻ ʻ ʻ  " << endl;
        Console::gotoxy(x, y + 6);setColour(7);cout << "         " << endl;
        setColour(7);
        break;
            
    case WEATHER_RAIN:

        Console::gotoxy(x, y);setColour(7);  cout <<   "         " << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << "    _.-.  " << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << "  (_(   ) " << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << " (____(__)" << endl;
        Console::gotoxy(x, y + 4);setColour(9);cout << " ʻ,ʻ,ʻ,ʻ,ʻ" << endl;
        Console::gotoxy(x, y + 5);setColour(9);cout << " ʻ,ʻ,ʻ,ʻ,ʻ" << endl;
        Console::gotoxy(x, y + 6);setColour(7);cout << "         " << endl;
        setColour(7);
        break;
    
    case WEATHER_SNOW:

        Console::gotoxy(x, y);setColour(7);  cout << "         " << endl;
        Console::gotoxy(x, y + 1);setColour(15);cout << "   _.-.  " << endl;
        Console::gotoxy(x, y + 2);setColour(15);cout << "  (_(   ) " << endl;
        Console::gotoxy(x, y + 3);setColour(15);cout << " (____(__)" << endl;
        Console::gotoxy(x, y + 4);setColour(15);cout << " ❆ ❅ ❆ ❅ ❆" << endl;
        Console::gotoxy(x, y + 5);setColour(15);cout << "❅ ❆ ❅ ❆ ❅" << endl;
        Console::gotoxy(x, y + 6);setColour(7);cout << "         " << endl;
        setColour(7);
        break;

    case WEATHER_CLEAR:
        
        Console::gotoxy(x, y);setColour(6);  cout << "         " << endl;
        Console::gotoxy(x, y+1);setColour(6);cout << "   \\   / " << endl;
        Console::gotoxy(x, y+2);setColour(6);cout << "    .-.   " << endl;
        Console::gotoxy(x, y+3);setColour(6);cout << " ‒ (   ) ‒" << endl;
        Console::gotoxy(x, y+4);setColour(6);cout << "    '-'   " << endl;
        Console::gotoxy(x, y+5);setColour(6);cout << "   /   \\ " << endl;
        Console::gotoxy(x, y+6);setColour(6);cout << "         " << endl;
        setColour(7);
        break;

    case WEATHER_CLOUDS:

        Console::gotoxy(x, y);setColour(7);  cout <<   "          " << endl;
        Console::gotoxy(x, y + 1);setColour(7);cout << "    _.-.   " << endl;
        Console::gotoxy(x, y + 2);setColour(7);cout << "  (_( _ ). " << endl;
        Console::gotoxy(x, y + 3);setColour(7);cout << " ( _.  (  )" << endl;
        Console::gotoxy(x, y + 4);setColour(7);cout << "(  ._.' ¯) " << endl;
        Console::gotoxy(x, y + 5);setColour(7);cout << " ¯'._.--'  " << endl;
        Console::gotoxy(x, y + 6);setColour(7);cout << "          " << endl;
        setColour(7);
        break;

    case WEATHER_MIST:

        Console::gotoxy(x, y);setColour(8);  cout <<   "         " << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << "_ - _ - _" << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << "         " << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << "- _ - _ -" << endl;
        Console::gotoxy(x, y + 4);setColour(8);cout << "         " << endl;
        Console::gotoxy(x, y + 5);setColour(8);cout << "_ - _ - _" << endl;
        Console::gotoxy(x, y + 6);setColour(8);cout << "         " << endl;
        setColour(7);
        break;
            
    case WEATHER_SMOKE:

        Console::gotoxy(x, y);setColour(8);  cout <<   "         " << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << "~ ~ _ ~ ~" << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << "~ _ ~ ~ _" << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << "~ ~ _ ~ ~" << endl;
        Console::gotoxy(x, y + 4);setColour(8);cout << "~ _ ~ ~ _" << endl;
        Console::gotoxy(x, y + 5);setColour(8);cout << "~   _   ~" << endl;
        Console::gotoxy(x, y + 6);setColour(8);cout << "         " << endl;
        setColour(7);
        break;

    case WEATHER_HAZE:

        Console::gotoxy(x, y);setColour(8);  cout << "         " << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << ". , . , ." << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << " , . , . ," << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << ". , . , ." << endl;
        Console::gotoxy(x, y + 4);setColour(8);cout << " , . , . ," << endl;
        Console::gotoxy(x, y + 5);setColour(8);cout << ". , . , ." << endl;
        Console::gotoxy(x, y + 6);setColour(8);cout << "         " << endl;
        setColour(7);
        break;

    case WEATHER_DUST:

        Console::gotoxy(x, y);setColour(8);  cout << "         " << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << " ;  .;  : " << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << ". ;  .;  :" << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << " :  .;  :;" << endl;
        Console::gotoxy(x, y + 4);setColour(8);cout << ".  :;  . :" << endl;
        Console::gotoxy(x, y + 5);setColour(8);cout << " ;  .;  ; " << endl;
        Console::gotoxy(x, y + 6);setColour(8);cout << "         " << endl;
        setColour(7);
        break;
           
    case WEATHER_SAND:

        Console::gotoxy(x, y);setColour(8);  cout << "         " << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << ",/ /,/ ,/" << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << ",/ /, / /" << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << "/, / ,/ ," << endl;
        Console::gotoxy(x, y + 4);setColour(8);cout << ",/ /, /, " << endl;
        Console::gotoxy(x, y + 5);setColour(8);cout << "/, / ,/ /" << endl;
        Console::gotoxy(x, y + 6);setColour(8);cout << "         " << endl;
        setColour(7);
        break;

    case WEATHER_FOG:

        Console::gotoxy(x, y);setColour(8);  cout << "         " << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << "= - = - =" << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << " = - = - " << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << "= - = - =" << endl;
        Console::gotoxy(x, y + 4);setColour(8);cout << " - = - = " << endl;
        Console::gotoxy(x, y + 5);setColour(8);cout << "= - = - =" << endl;
        Console::gotoxy(x, y + 6);setColour(8);cout << "         " << endl;
        setColour(7);
        break;
            
    case WEATHER_SQUALL:

        Console::gotoxy(x, y);setColour(8);  cout <<   "           " << endl;
        Console::gotoxy(x, y + 1);setColour(7);cout << "/ ʻ  / ʻ  /" << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << "  / ʻ  / ʻ " << endl;
        Console::gotoxy(x, y + 3);setColour(7);cout << "/  / ʻ  / ʻ" << endl;
        Console::gotoxy(x, y + 4);setColour(7);cout << "  / ʻ  / ʻ " << endl;
        Console::gotoxy(x, y + 5);setColour(8);cout << "/ ʻ  / ʻ  /" << endl;
        Console::gotoxy(x, y + 6);setColour(8);cout << "           " << endl;
        setColour(7);
        break;

    case WEATHER_TORNADO:

        Console::gotoxy(x, y);setColour(7);  cout <<   "--_ - __- --" << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << "   -_ - _   " << endl;
        Console::gotoxy(x, y + 2);setColour(7);cout << "    -_ -    " << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << "   _ -      " << endl;
        Console::gotoxy(x, y + 4);setColour(7);cout << "  _ -       " << endl;
        Console::gotoxy(x, y + 5);setColour(7);cout << "  -_        " << endl;
        Console::gotoxy(x, y + 6);setColour(8);cout << "  _ - _     " << endl;
        setColour(7);
        break;
            
    case WEATHER_UNKNOWN:
    default:
        Console::gotoxy(x, y);setColour(7);  cout <<   "            " << endl;
        Console::gotoxy(x, y + 1);setColour(6);cout << "  \\  /      " << endl;
        Console::gotoxy(x, y + 2);setColour(6);cout << "_ /\"\"\\   " << endl;Console::gotoxy(x+5, y + 2);setColour(7);cout <<".-." << endl;
        Console::gotoxy(x, y + 3);setColour(6);cout << "  \\__/  " << endl;Console::gotoxy(x+4, y + 3);setColour(7);cout << "(   )." << endl;
        Console::gotoxy(x, y + 4);setColour(6);cout << "  /  \\  " << endl;Console::gotoxy(x+3,y + 4);setColour(7);cout << "(___(__)" << endl;
        Console::gotoxy(x, y + 5);setColour(7);cout << "            " << endl;
        Console::gotoxy(x, y + 6);setColour(7);cout << "    [N/A]   " << endl;
        setColour(7);
        break;
    } 


}

void printWindDirection(int deg) 
{
    deg = (deg % 360 + 360) % 360;

    string arrows[] = { "↓", "↙", "←", "↖", "↑", "↗", "→", "↘" };

    int index = static_cast<int>((deg + 22.5) / 45) % 8;
    
    cout << arrows[index];
}

void Sun(int x,int y,int num )
{
    if (num == 0)
    {
        Console::gotoxy(x, y);setColour(6);cout << "    \\   /   " << endl;
        Console::gotoxy(x, y+1);setColour(6);cout << "     .-.     " << endl;
        Console::gotoxy(x, y+2);setColour(6);cout << "  ‒ (   ) ‒  " << endl;
        Console::gotoxy(x, y+3);setColour(6);cout << "     '-'     " << endl;
        Console::gotoxy(x, y+4);setColour(6);cout << "    /   \\   " << endl;
        Console::gotoxy(x, y+5);setColour(1
        );cout << " ~~~~~~~~~~~ " << endl;
        setColour(7);
    }
    else 
    {
        Console::gotoxy(x, y);setColour(6);cout <<   "   .-'¯'-.   " << endl;
        Console::gotoxy(x, y+1);setColour(6);cout << "  /       \\  " << endl;
        Console::gotoxy(x, y + 2);setColour(6);cout << " ┴─────────┴ " << endl;
        Console::gotoxy(x, y+3);setColour(7);cout << "   ───────   " << endl;
        Console::gotoxy(x, y+4);setColour(8);cout << "     ───     " << endl;
    }
    setColour(7);
}
void Clouds_num(int percent,int x, int y)
{
    if(percent >70)
    {
        Console::gotoxy(x, y);setColour(8);cout << "    _.-.   " << endl;
        Console::gotoxy(x, y + 1);setColour(8);cout << "  _(_(  )  " << endl;
        Console::gotoxy(x, y + 2);setColour(8);cout << " ( .-'¯¯ )._ " << endl;
        Console::gotoxy(x, y + 3);setColour(8);cout << "( _ )(_( _  )" << endl;
        Console::gotoxy(x, y + 4);setColour(8);cout << " '.__( _.  )" << endl;
        Console::gotoxy(x, y + 5);setColour(8);cout << " (  ._.' ¯)" << endl;
        Console::gotoxy(x, y + 6);setColour(8);cout << "  ¯'._.--'" << endl;
        setColour(7);
    }
    else if (percent > 30)
    {
        Console::gotoxy(x, y);     cout << "           " << endl;
        Console::gotoxy(x, y + 1); cout << "    _.-.   " << endl;
        Console::gotoxy(x, y + 2); cout << "  (_( _ ). " << endl;
        Console::gotoxy(x, y + 3); cout << " ( _.  (  )" << endl;
        Console::gotoxy(x, y + 4); cout << "(  ._.' ¯) " << endl;
        Console::gotoxy(x, y + 5); cout << " ¯'._.--'  " << endl;
        Console::gotoxy(x, y + 6); cout << "           " << endl;
    }
    else if (percent <= 30)
    {
        cout << "";
        Console::gotoxy(x, y);     cout << "        " << endl;
        Console::gotoxy(x, y + 1);setColour(6); cout << "  \\_/" << endl;
        Console::gotoxy(x, y + 2);setColour(6); cout << "─ (  " << endl;Console::gotoxy(x + 4, y + 2);setColour(7); cout << "_.-._" << endl;
        Console::gotoxy(x, y + 3);setColour(6); cout << "  /   " << endl;Console::gotoxy(x + 3, y + 3);setColour(7); cout << "(_ ( _)." << endl;
        Console::gotoxy(x, y + 4); cout << "        " << endl;              Console::gotoxy(x + 2, y + 4);setColour(7); cout << "(____)___)" << endl;
        Console::gotoxy(x, y + 5); cout << "        " << endl;
        Console::gotoxy(x, y + 6); cout << "        " << endl;
    }

    setColour(8);
    for (int i = 0; i < 5; i++)
    {
        Console::gotoxy(x + 15, y+1 + i); cout << "░░";
    }

    int loop = (((double)percent / 100) * 5);

    setColour(15);
    for (int i = 0; i < loop; i++)
    {
        Console::gotoxy(x + 15, y+5 - i); cout << "██";
    }
    setColour(7);

    Console::gotoxy(x + 18, y+3); cout << percent<< "%";
}

void H_temp(double temp,int x,int y)
{
    Console::gotoxy(x, y );setColour(7);    cout << " ,_______________/¯¯¯\\" << endl;
    Console::gotoxy(x, y + 1);setColour(7); cout << "(                   ) |";Console::gotoxy(x+18, y + 1);setColour(4); cout << "─⬤"; cout << endl;
    Console::gotoxy(x, y + 2);setColour(7); cout << " `¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\\___/" << endl;     

    int loop = (temp / 100) * 16;

    for (int i = 0; i < 16; i++)
    {
        Console::gotoxy(x + 17 - i, y + 1);setColour(8); cout << "─"; cout << endl;
    }

    for (int i = 0; i < loop; i++)
    {
        Console::gotoxy(x + 17-i, y + 1);setColour(4); cout << "─"; cout << endl;
    }
    setColour(7);

    Console::gotoxy(x + 2, y - 2);setColour(7); cout << "Temp (D) : "; cout << temp << " %";

}

void print_RorS_fall(double rain_3h, double snow_3h, int x, int y) {

    if (snow_3h > 0.0) 
    {
        setColour(15);
        Console::gotoxy(x, y);setColour(15); cout <<     "  .   ❆  *  ❆ " << endl;
        Console::gotoxy(x, y + 1);setColour(15); cout << " ❅ . ❆ .  ❆  " << endl;
        Console::gotoxy(x, y + 2);setColour(7); cout <<  "  ❅   * .  ❆  " << endl;
        Console::gotoxy(x, y + 3);setColour(15); cout << " ❆  * .  ❆ * " << endl;
        Console::gotoxy(x, y + 4);setColour(15); cout << "▄▆█▆▆▄▄▆█▆▄▄▆▄" << endl;


        setColour(7);
        Console::gotoxy(x + 16, y + 1); cout << "│ [❆ WINTER INFO] ";
        Console::gotoxy(x + 16, y + 2); cout << "│ Status   : SNOWING";
        Console::gotoxy(x + 16, y + 3); cout << "│ Snow(3h) : " << snow_3h << " mm";
    }
    else if (rain_3h > 0.0) 
    {
        setColour(11);
        Console::gotoxy(x, y);setColour(8); cout << "  /   /   /   " << endl;
        Console::gotoxy(x, y + 1);setColour(7); cout << " .   /   .    " << endl;
        Console::gotoxy(x, y + 2);setColour(8); cout << "  /   .   /   " << endl;
        Console::gotoxy(x, y + 3);setColour(7); cout << " /   /   /    " << endl;
        Console::gotoxy(x, y + 4);setColour(11); cout << "~~~~~~~~~~~~~ " << endl;

        setColour(7);
        Console::gotoxy(x + 16, y + 1); cout << "│ [⛆ PRECIPITATION]";
        Console::gotoxy(x + 16, y + 2); cout << "│ Status   : RAINING";
        Console::gotoxy(x + 16, y + 3); cout << "│ Rain(3h) : " << rain_3h << " mm";
    }
    else 
    {
        Console::gotoxy(x, y);setColour(6);cout << "   \\   / " << endl;
        Console::gotoxy(x, y + 1);setColour(6);cout << "    .-.   " << endl;
        Console::gotoxy(x, y + 2);setColour(6);cout << " ‒ (   ) ‒" << endl;
        Console::gotoxy(x, y + 3);setColour(6);cout << "    '-'   " << endl;
        Console::gotoxy(x, y + 4);setColour(6);cout << "   /   \\ " << endl;

        setColour(7);
        Console::gotoxy(x + 16, y + 1); cout << "│ [✹ HYDRATION ]";
        Console::gotoxy(x + 16, y + 2); cout << "│ Status   : CLEAR";
        Console::gotoxy(x + 16, y + 3); cout << "│ Moisture : NONE";
    }

    setColour(7);
}

void mountain(int g_hPa ,int s_hPa,int x,int y)
{ 
    Console::gotoxy(x, y);setColour(15);cout <<    "       ／￣＼" << endl;Console::gotoxy(x+14, y);setColour(7);cout << "-> "<<g_hPa <<" hPa (Ground Lv Pressure)" << endl;
    Console::gotoxy(x, y + 1);setColour(7);cout << "  ⊂⌒⊃／～～～＼⊂⊃" << endl;
    Console::gotoxy(x, y + 2);setColour(7);cout << "    ⊂⌒⊃／人＼＼＼" << endl;
    Console::gotoxy(x, y + 3);setColour(7);cout << "~~／~~~／~~＼~~~~＼~~" << endl;Console::gotoxy(x + 22, y+3);setColour(7);cout << "-> " << s_hPa << " hPa (Sea Lv Pressure)" << endl;
   
    Console::gotoxy(x, y + 3);setColour(9);cout << "~~";
    Console::gotoxy(x + 4, y + 3);setColour(9);cout << "~~~";
    Console::gotoxy(x + 9, y + 3);setColour(9);cout << "~~";
    Console::gotoxy(x + 13, y + 3);setColour(9);cout << "~~~~";
    Console::gotoxy(x+19, y + 3);setColour(9);cout << "~~";



}

void printSocialSystem()
{
    UIcfg uiCfg;
    uiCfg.widthDv = 1.5;

    int ext = 10;

    int AxisX = 25;
    int AxisY = 13;

    int check = 0;

    int year = 2026;

    vector <long long> totalPol = { 32380000,32800000,33200000,33580000,33940000,34300000,34680000,35050000,35420000 };
    
    long long bigNum = findBigNum(totalPol);

    long long latestTotal = totalPol[8];
    long long child = 7969500;
    long long middle = 24333540;
    long long older = 3116960;
    double childper = 22.5;
    double middleper = 68.7;
    double olderper = 8.8;
    int totalper = 100;
    double brate = 5.343;
    double death = 3.122;
    double LifeExp = 80;

    double sec[] = { 100,92.5,86.7,76.9 };

    double high[] = { 40.5,35.3,28.6,15.2 };


    double allPer[] = { childper,middleper,olderper,totalper };

    string child_per = format("{}", childper);
    string middle_per = format("{}", middleper);
    string older_per = format("{}", olderper);
    string S_brate = format("{}", brate);
    string S_death = format("{}", death);

    string text[] = { child_per,to_string(child),middle_per,to_string(middle),older_per,to_string(older),to_string(totalper),to_string(latestTotal)};



    printPageMenu({ .text = "SOCIAL DATA",.c1 = 7 ,.c2 = 3 ,.c3 = 6 }, uiCfg);

    Console::gotoxy(AxisX + 1, AxisY);printCenter("┌─ Annual Population ────────────────────────────────┐ ┌─ Percentage of children ──────────────────────┐", 0);

    for (int i = 1; i < 12; i++)
    {
        Console::gotoxy(AxisX + 1, AxisY + i);cout << "│"; Console::gotoxy(AxisX + 54, AxisY + i); cout << "│";
        if (i == 11) { Console::gotoxy(AxisX + 1, AxisY + i);cout << "└────────────────────────────────────────────────────┘"; }

        string text = "";

        int result = i % 3;

        if (result == 0)
        {
            if (check == 0)
            {
                Console::gotoxy(AxisX + 56, AxisY + i); cout << "┌─ " << "Percentage Of Middle-Aged People ────────────┐";
                check++;
            }
            else if (check == 1)
            {
                Console::gotoxy(AxisX + 56, AxisY + i); cout << "┌─ " << "Percentage Of Elderly People ────────────────┐";
                check++;
            }
            else if (check == 2)
            {
                Console::gotoxy(AxisX + 56, AxisY + i); cout << "┌─ " << "Total Population ────────────────────────────┐";
            }
        }
        else if (result == 1)
        {
            Console::gotoxy(AxisX + 56, AxisY + i); cout << "│"; Console::gotoxy(AxisX + 104, AxisY + i); cout << "│";
        }
        else if (result == 2)
        {
            Console::gotoxy(AxisX + 56, AxisY + i); cout << "└───────────────────────────────────────────────┘";
        }
        else if (result == 3)
        {
            Console::gotoxy(AxisX + 56, AxisY + i); ;
        }

    }

    for (int i = 1; i < 5; i++)
    {
        long double big = (bigNum * ((5.0-i) / 4.0));
        Console::gotoxy(AxisX + 2, AxisY-1 + (i * 2)); cout << right << setw(5) << SimplyNum(big) << "│";
    }

    Console::gotoxy(AxisX + 2, AxisY + 9); cout << right << setw(5) << "0";

    for (int i = 1; i < 5; i++)
    {
        Console::gotoxy(AxisX + 7, AxisY + 10 - (i * 2)); cout << "│";
    }

    Console::gotoxy(AxisX + 7, AxisY + 9); cout << "└────────────────────────────────────────────";

    Console::gotoxy(AxisX + 8, AxisY + 10);

    year -= 8;

    for (int i = 0; i <= 8; i++)
    {
        cout << year << " ";
        year++;
    }

    for (int i = 0; i < 9;i++)
    {
        for (int j = 0; j < round((((double long)totalPol[i]/ (double long)bigNum)*8)); j++)
        {
            Console::gotoxy((AxisX + 9) + (i * 5), (AxisY + 8) - j); cout << "██";
        }
    }

    for (int j = 0; j < 4; j++)
    {
        int pos = (43 - ((text[(j*2)].length() + text[(j * 2)+1].length()))) / 2;

        for (int i = 0; i < ((allPer[j]/100)*47); i++)
        {
            switch (j)
            {
            case 0:
                setColour(2);
                break;
            case 1:
                setColour(1);
                break;
            case 2:
                setColour(4);
                break;
            case 3:
                setColour(6);
                break;
            default:
                setColour(7);
            }

            Console::gotoxy((AxisX + 57) + i, (AxisY + 1) + j * 3);
            cout << "│";
        }
        setColour(7);
        Console::gotoxy((AxisX + 57) + pos, (AxisY + 1) + j * 3);
        cout << text[(j * 2)] << "% (" << text[(j * 2) + 1]<<")";
    }

    setColour(7);
    Console::gotoxy(AxisX + 1, AxisY + 13);cout << "┌─ Demographic Dynamics ──────────┐"; 

    for (int i = 1; i < 13; i++)
    {
        Console::gotoxy(AxisX + 1, AxisY + 13 + i); cout << "│";
        Console::gotoxy(AxisX + 35, AxisY + 13 + i); cout << "│";
    }

    Console::gotoxy(AxisX + 1, AxisY + 25);cout << "│      BIRTH           DEATH      │";

    Console::gotoxy(AxisX + 1, AxisY + 26);cout << "└─────────────────────────────────┘";  

    int loopBrate = ceil((brate / 20) * 9);
    if (loopBrate > 9) loopBrate = 9;

    int loopDeath = ceil((death / 20) * 9);
    if (loopDeath > 9) loopDeath = 9;


    for (int j = 0; j < loopBrate; j++)
    {
        setColour(10);
        for (int i = 0; i < 15; i++)
        {
            Console::gotoxy(AxisX + 3 + i, AxisY + 24 - j); cout << "═";
        }
        if (loopBrate - 1)
        {
            setColour(7);Console::gotoxy(AxisX + 3 + ((15 - S_brate.length()) / 2), AxisY + 23 - j); cout << brate;
        }
    }


    for (int j = 0; j < loopDeath; j++)
    {
        setColour(4);
        for (int i = 0; i < 15; i++)
        {
            Console::gotoxy(AxisX + 19 + i, AxisY + 24 - j); cout << "═";
        }
        if (loopDeath - 1)
        {
            setColour(7);Console::gotoxy(AxisX + 19 + ((15 - S_death.length()) / 2), AxisY + 23 - j); cout << death;
        }
    }

    for (int i = 0; i < 40; i++)
    {
        setColour(8);
        Console::gotoxy(AxisX + 37 + i, AxisY + 23);cout << "░";
    }
    setColour(7);

    int BarLength = (LifeExp / 100) * 40;

    int c = 7;

    if (LifeExp >= 73) c = 2;
    if (LifeExp < 73) c = 6;
    if (LifeExp < 60) c = 4;

    setColour(c);

    for (int i = 0; i < BarLength; i++)
    {
        Console::gotoxy(AxisX + 37 + i, AxisY + 23);cout << "█";
    }

    setColour(7);

    string msg = "Life Expectancy";

    printNum(LifeExp, AxisX + 48, AxisY + 15);

    Console::gotoxy(AxisX +2, AxisY + 12); cout << "------------------------------------------------------------------------------------------------------";

    Console::gotoxy(AxisX + 36, AxisY + 13); cout << "┌──────────────────YEAR──────────────────┐";

    Console::gotoxy(AxisX + 37 + ((40 - msg.length()) / 2), AxisY + 25); cout << msg;

    Console::gotoxy(AxisX + 36, AxisY + 26); cout << "└────────────────────────────────────────┘";

    Console::gotoxy(AxisX + 78, AxisY + 13);cout << "┌─ Education ────────────┐";

    for (int i = 1; i < 13; i++)
    {
        Console::gotoxy(AxisX + 78, AxisY + 13 + i); cout << "│";
        Console::gotoxy(AxisX + 103, AxisY + 13 + i); cout << "│";
    }

    Console::gotoxy(AxisX + 78, AxisY + 24);cout << "│ ──────────  ────────── │";

    Console::gotoxy(AxisX + 78, AxisY + 25);cout << "│  Secondary    Higher   │";

    Console::gotoxy(AxisX + 78, AxisY + 26);cout << "└────────────────────────┘";

    for (int j = 0; j < 4; j++)
    {
        int size = (round(sec[j]) / 100) * 9;

        for (int i = 0; i < size; i++)
        {
            switch (i)
            {
            case 0:

            case 1:
                setColour(1);
                break;
            case 2:

            case 3:
                setColour(9);
                break;
            case 4:

            case 5:
                setColour(3);
                break;
            case 6:

            case 7:
                setColour(11);
                break;
            case 8:

            case 9:
                setColour(13);
                break;
            default:
                setColour(7);
            }

            Console::gotoxy(AxisX + 81 + (j * 2), (AxisY + 23) - i); cout << "│";
            Console::gotoxy(AxisX + 82 + (j * 2), (AxisY + 23) - i); cout << "│";
        }
    }

    for (int j = 0; j < 4; j++)
    {
        int size = (high[j] / 100) * 10;

        for (int i = 0; i < size; i++)
        {
            switch (i)
            {
            case 0:

            case 1:
                setColour(1);
                break;
            case 2:

            case 3:
                setColour(9);
                break;
            case 4:

            case 5:
                setColour(3);
                break;
            case 6:

            case 7:
                setColour(11);
                break;
            case 8:

            case 9:
                setColour(13);
                break;
            default:
                setColour(7);
            }
            Console::gotoxy(AxisX + 93 + (j * 2), (AxisY + 23) - i); cout << "│";
            Console::gotoxy(AxisX + 94 + (j * 2), (AxisY + 23) - i); cout << "│";
        }
    }

    printCinBar(7, uiCfg, ext);

    cin.ignore();
}

void printEconomicSystem()
{
    UIcfg uiCfg;
    uiCfg.widthDv = 1.5;

    int ext = 10;

    int AxisX = 25;
    int AxisY = 13;

    int year = 2026;

    vector <long long> allGdp = { 358700000000,365300000000,337300000000,374100000000,407000000000,431000000000,451500000000,472800000000,495000000000};

    vector <long long> allGdp_perC = { 11070,11140,10160,11110,11990,12560,13010,13490,13975 };

    vector <double> growthRate = { 4.8,4.4,-5.5,3.3,8.7,3.7,4.2,4.5,4.4 };

    double Export = 265400000000;

    double Import = 224100000000;

    double Infla = 20;

    double debt = 401500000000.0;

    long long Gdp_latest = allGdp[8];

    long long bigGdp = findBigNum(allGdp);

    long long bigGdp_perC = findBigNum(allGdp_perC);

    long long debt_int = static_cast<long long>(debt);

    printPageMenu({ .text = "ECONOMIC DATA",.c1 = 7 ,.c2 = 3 ,.c3 = 6 }, uiCfg);

    Console::gotoxy(AxisX + 1, AxisY);printCenter("┌─ Gross Domestic Product (GDP) ─────────────────────┐ ┌─ GDP Annual Growth Rate ──────────────────────┐", 0);
    
    for (int i = 1; i < 20; i++)
    {
        Console::gotoxy(AxisX + 1, AxisY + i);cout << "│"; Console::gotoxy(AxisX + 54, AxisY + i); cout << "│";
    }

    Console::gotoxy(AxisX + 7, AxisY + 9); cout << "└────────────────────────────────────────────";

    Console::gotoxy(AxisX + 8, AxisY + 10);

    year -= 8;

    for (int i = 0; i <= 8; i++)
    {
        cout << year << " ";
        year++;
    }
    //
    for (int i = 1; i < 5; i++)
    {
        long double big = (bigGdp * ((5.0 - i) / 4.0));
        Console::gotoxy(AxisX + 2, AxisY - 1 + (i * 2)); cout << right << setw(5) << SimplyNum(big) << "│";
    }

    for (int i = 1; i < 5; i++)
    {
        Console::gotoxy(AxisX + 7, AxisY + 10 - (i * 2)); cout << "│";
    }

    Console::gotoxy(AxisX + 2, AxisY + 9); cout << right << setw(5) << "0";

    Console::gotoxy(AxisX + 2, AxisY + 11); cout << right << setw(5) << "0";

    for (int i = 0; i < 9;i++)
    {
        for (int j = 0; j < round((((double long)allGdp[i] / (double long)bigGdp) * 8)); j++)
        {
            Console::gotoxy((AxisX + 9) + (i * 5), (AxisY + 8) - j); cout << "██";
        }
    }

    Console::gotoxy(AxisX + 7, AxisY+11); cout << "┌────────────────────────────────────────────";

    for (int i = 1; i < 5; i++)
    {
        long double big = (bigGdp_perC * ((5.0 - i) / 4.0));
        Console::gotoxy(AxisX + 2, (AxisY +21) - (i * 2)); cout << right << setw(5) << SimplyNum(big) << "│";
    }

    for (int i = 1; i < 5; i++)
    {
        Console::gotoxy(AxisX + 7, (AxisY + 20) - (i * 2)); cout << "│";
    }

    for (int i = 0; i < 9;i++)
    {
        for (int j = 0; j < round((((double long)allGdp_perC[i] / (double long)bigGdp_perC) * 8)); j++)
        {
            Console::gotoxy((AxisX + 9) + (i * 5), (AxisY + 12) + j); cout << "██";
        }
    }

    Console::gotoxy(AxisX + 1, AxisY + 20);cout << "└─ GDP Per Capita ───────────────────────────────────┘";

    for (int i = 1; i < 20; i++)
    {
        Console::gotoxy(AxisX + 56, AxisY + i); cout << "│";
        Console::gotoxy(AxisX + 104, AxisY  + i); cout << "│";
    }

    Console::gotoxy(AxisX + 56, AxisY + 20);cout << "└───────────────────────────────────────────────┘";

    for (int i = 0; i < 9; i++)
    {
        year--;
        Console::gotoxy(AxisX + 58, AxisY + 2 + (i*2) ); cout << year << ": ";
    }

    for (int j = 0; j < 9; j++)
    {
        setColour(10);
        if (growthRate[j] < 0) setColour(4);

        int length = (abs(growthRate[j]) / 30) * 34;

        if (length > 34) length = 34;

        for (int i = 0; i < length; i++)
        {
            Console::gotoxy(AxisX + 64 + i, AxisY + 2 + (2 * j)); cout << "█" << " " << P_N(growthRate[j]) <<"%";
        }
    }

    setColour(7);

    Console::gotoxy(AxisX + 1, AxisY + 21); cout << "┌─ T.Exports (USD) ───┐";

    for (int i = 0; i < 4; i++)
    {
        Console::gotoxy(AxisX + 1, AxisY + 22 + i); cout << "│";
        Console::gotoxy(AxisX + 23, AxisY + 22 + i); cout << "│";
    }

    Console::gotoxy(AxisX + 1, AxisY + 26); cout << "└─────────────────────┘";

    for (int i = 0; i < Ex.size(); i++)
    {
        if (i == 3) setColour(11);
        Console::gotoxy(AxisX + 3, AxisY + 22 + i); cout << Ex[i];
    }

    setColour(7);

    Console::gotoxy(AxisX + 16 + ((5 - SimplyNum(Export).length()) / 2), AxisY + 23);setColour(10); cout << "+" << SimplyNum(Export);setColour(7);

    Console::gotoxy(AxisX + 25, AxisY + 21); cout << "┌─ T.Imports (USD) ───┐";

    for (int i = 0; i < 4; i++)
    {
        Console::gotoxy(AxisX + 25, AxisY + 22 + i); cout << "│";
        Console::gotoxy(AxisX + 47, AxisY + 22 + i); cout << "│";
    }

    Console::gotoxy(AxisX + 25, AxisY + 26); cout << "└─────────────────────┘";

    for (int i = 0; i < Im.size(); i++)
    {
        if (i == 3) setColour(11);
        Console::gotoxy(AxisX + 27, AxisY + 22 + i); cout << Im[i];
    }

    setColour(7);

    Console::gotoxy(AxisX + 28 + ((5 - SimplyNum(Import).length()) / 2), AxisY + 23);setColour(4); cout << "-" << SimplyNum(Import);setColour(7);

    Console::gotoxy(AxisX + 49, AxisY + 21); cout << "┌─ Inflation Rate ────┐";

    for (int i = 0; i < 4; i++)
    {
        Console::gotoxy(AxisX + 49, AxisY + 22 + i); cout << "│";
        Console::gotoxy(AxisX + 71, AxisY + 22 + i); cout << "│";
    }

    Console::gotoxy(AxisX + 49, AxisY + 26); cout << "└─────────────────────┘";
    
    Console::gotoxy(AxisX + 52, AxisY + 23 ); cout << " ↑ ↑ ↑ ↑ ";
    Console::gotoxy(AxisX + 52, AxisY + 24);setColour(6); cout << "⛀.⛁.⛁.⛁.⛀";
    Console::gotoxy(AxisX + 51, AxisY + 25);setColour(7);  cout << "-------------";

    for (int i = 0; i <5; i++)
    {
        Console::gotoxy(AxisX + 51 + (i * 4), AxisY + 25);setColour(10); cout << "⬆";
        Console::gotoxy(AxisX + 53 + (i * 4), AxisY + 25);setColour(4); cout << "⬇";
    }

    Console::gotoxy(AxisX + 64 + ((4 - P_N(Infla).length()) / 2), AxisY + 23);
    Infla < 0 ? setColour(10) : setColour(4); cout << P_N(Infla) << "%";setColour(7);

    Console::gotoxy(AxisX + 73, AxisY + 21); cout << "┌─ Government Debt (USD) ─────┐";

    for (int i = 0; i < 4; i++)
    {
        Console::gotoxy(AxisX + 73, AxisY + 22 + i); cout << "│";
        Console::gotoxy(AxisX + 103, AxisY + 22 + i); cout << "│";
    }

    Console::gotoxy(AxisX + 73, AxisY + 26); cout << "└─────────────────────────────┘";
    
    Console::gotoxy(AxisX + 75, AxisY + 23); cout << "𓆩[$()$]𓆪";
    Console::gotoxy(AxisX + 76, AxisY + 23);setColour(10); cout << "[";
    Console::gotoxy(AxisX + 78, AxisY + 23);setColour(10); cout << "()";
    Console::gotoxy(AxisX + 81, AxisY + 23);setColour(10); cout << "]";
    Console::gotoxy(AxisX + 75, AxisY + 25); cout << "---------------------------";

    for (int i = 0; i < 4; i++)
    {
        Console::gotoxy(AxisX + 75 + (i * 8), AxisY + 25);setColour(10); cout << "€";
        Console::gotoxy(AxisX + 77 + (i * 8), AxisY + 25);setColour(6); cout << "$";
    }

    for (int i = 0; i < 3; i++)
    {
        Console::gotoxy(AxisX + 79 + (i * 8), AxisY + 25);setColour(1); cout << "₹";
        Console::gotoxy(AxisX + 81 + (i * 8), AxisY + 25);setColour(4); cout << "¥";
    }

    for (int i = 0; i < 9; i++)
    {
        Console::gotoxy(AxisX + 52 + (i * 2), AxisY + 25);setColour(7); cout << "-";
    }

    int debtPer = (((double)debt / (double)Gdp_latest) * 100);

    if (debtPer >= 100) setColour(4);
    else if (debtPer >= 77) setColour(6);
    else if (debtPer >= 60) setColour(14);
    else  setColour(10);


    Console::gotoxy(AxisX + 84 + ((18 - ((to_string(debt_int).length())+2)) / 2), AxisY + 23);cout << "-" << debt_int << "$";setColour(7);

    printCinBar(7, uiCfg, ext);

    cin.ignore();
}

void printEandCSystem()
{
    UIcfg uiCfg;
    uiCfg.widthDv = 1.5;

    int ext = 10;

    int AxisX = 25;
    int AxisY = 13;

    AsciiTimeData temp = getDigitsForAscii(1781113029, -14400);

    printPageMenu({ .text = "ENVIRONMENT AND CLIMATE",.c1 = 7 ,.c2 = 3 ,.c3 = 6 }, uiCfg);

    Console::gotoxy(AxisX + 1, AxisY);cout << "┌─ Local Time ────────────────────────────────────────────────────────┐┌─ Local Weather ───────────────┐";

    Console::gotoxy(AxisX + 15, AxisY);cout << "("; temp.Time_Zone < 0 ? cout << "-" : cout << "+";cout <<abs(temp.Time_Zone) << ") ";

    for (int i = 1; i < 10; i++)
    {
        Console::gotoxy(AxisX + 1, AxisY + i);cout << "│"; Console::gotoxy(AxisX + 71, AxisY + i); cout << "│";
    }

    for (int i = 1; i < 10; i++)
    {
        Console::gotoxy(AxisX + 72, AxisY + i);cout << "│"; Console::gotoxy(AxisX + 104, AxisY + i); cout << "│";
    }

    Console::gotoxy(AxisX+1, AxisY + 10); cout << "└─────────────────────────────────────────────────────────────────────┘└───────────────────────────────┘";

    printNum(temp.Time_F, AxisX + 3, AxisY + 2,FALSE);

    printVec(Dot, 7, AxisX + 23, AxisY + 2);

    printNum(temp.Time_B, AxisX +27, AxisY + 2, FALSE);

    temp.isPM ? printVec(PM, 7, AxisX + 47, AxisY + 2): printVec(AM, 7, AxisX + 47, AxisY + 2);

    long long sunrise = 1781493180;
    long long sunset = 1781554740;
    string weather = "Snow";
    int temperature = 26;
    int temperature_Feels = 30;
    double w_sped = 3.6;
    int w_deg = 225;
    int visib = 10000 / 1000;
    int humid = 62;
    int Cloud_P = 88;
    double high_temp = 36.5;
    double rain3h = 3.0;
    double snow3h = 0.0;
    int gP = 985;
    int sP = 1015;

    AsciiTimeData s_rise = getDigitsForAscii(sunrise, 3600);
    AsciiTimeData s_set = getDigitsForAscii(sunset, 3600);

    getWeather(weather, AxisX + 74, AxisY + 2);

    Console::gotoxy(AxisX + 88, AxisY + 3); cout << weather;
    Console::gotoxy(AxisX + 88, AxisY + 4);temperature < 0 ? cout << "-" : cout << "+"; cout << temperature << "(" << temperature_Feels << ")" << " °C";
    Console::gotoxy(AxisX + 88, AxisY + 5);printWindDirection(w_deg); cout << " " << w_sped << " m/h";
    Console::gotoxy(AxisX + 88, AxisY + 6);cout << "⚆ " << visib << " km";
    Console::gotoxy(AxisX + 88, AxisY + 7);cout << "≈ " << humid << "%";

    Console::gotoxy(AxisX + 1, AxisY +11);cout << "┌─ Sunrise ───────────────┐┌─ Sunset ────────────────┐┌─ Cloud Coverage ──────┐┌─ H.Temperature ───────┐";
    
    for (int i = 0; i < 7; i++)
    {
        Console::gotoxy(AxisX + 1, AxisY +12 + i);cout << "│"; Console::gotoxy(AxisX + 27, AxisY + 12 + i); cout << "│";
        Console::gotoxy(AxisX + 28, AxisY + 12 + i);cout << "│"; Console::gotoxy(AxisX + 54, AxisY + 12 + i); cout << "│";
        Console::gotoxy(AxisX + 55, AxisY + 12 + i);cout << "│"; Console::gotoxy(AxisX + 79, AxisY + 12 + i); cout << "│";
        Console::gotoxy(AxisX + 80, AxisY + 12 + i);cout << "│"; Console::gotoxy(AxisX + 104, AxisY + 12 + i); cout << "│";
    }

    Console::gotoxy(AxisX + 1, AxisY + 19);cout << "└─────────────────────────┘└─────────────────────────┘└───────────────────────┘└───────────────────────┘";

    Sun(AxisX + 2, AxisY + 13,0);
    
    Sun(AxisX + 29, AxisY + 14, 1);

    Console::gotoxy(AxisX + 16, AxisY + 15);cout << s_rise.Time_F << " : ";s_rise.Time_B < 10 ? cout << "0" << s_rise.Time_B : cout << s_rise.Time_B; s_rise.isPM ? cout << " PM" : cout << " AM";

    Console::gotoxy(AxisX + 43, AxisY + 15);cout << s_set.Time_F << " : ";s_set.Time_B < 10 ? cout << "0" << s_set.Time_B : cout << s_set.Time_B; s_set.isPM ? cout << " PM" : cout << " AM";

    Clouds_num(Cloud_P, AxisX + 57, AxisY + 12);
    
    H_temp(high_temp, AxisX + 81, AxisY + 15);

    Console::gotoxy(AxisX + 1, AxisY + 20);cout << "    ┌─ Rainfall/Snowfall (mm) ──────────────┐┌─ Sea/Surface Presssure(hPa) ────────────────────────┐";

    for (int i = 0; i < 5; i++)
    {
        Console::gotoxy(AxisX + 5, AxisY + 21 + i);cout << "│"; Console::gotoxy(AxisX + 45, AxisY + 21 + i); cout << "│";
        Console::gotoxy(AxisX + 46, AxisY + 21 + i);cout << "│"; Console::gotoxy(AxisX + 100, AxisY + 21 + i); cout << "│";
    }

    Console::gotoxy(AxisX + 1, AxisY + 26);cout << "    └───────────────────────────────────────┘└─────────────────────────────────────────────────────┘";

    print_RorS_fall(rain3h, snow3h, AxisX + 7, AxisY + 21);

    mountain(gP, sP, AxisX + 48, AxisY + 22);

    printCinBar(7, uiCfg, ext);

    cin.ignore();
}


void printNewSystem()
{
    UIcfg uiCfg;
    uiCfg.widthDv = 1.5;

    int ext = 10;

    int AxisX = 25;
    int AxisY = 13;

    printPageMenu({ .text = "NEWS AND HISTORY",.c1 = 7 ,.c2 = 3 ,.c3 = 6 }, uiCfg);



    printCinBar(7, uiCfg, ext);

    cin.ignore();
}
