/* Main Loop for cataclysm
 * Linux only I guess
 * But maybe not
 * Who knows
 */

#include <curses.h>
#include <ctime>
#include "game.h"
#include "color.h"
#include <windows.h>
int main(int argc, char *argv[])
{
 srand(time(NULL));
HANDLE hOut;
hOut = GetStdHandle(STD_OUTPUT_HANDLE);
     COORD NewSBSize;
     NewSBSize.X = 200;
     NewSBSize.Y = 200;
   SetConsoleScreenBufferSize(hOut,NewSBSize);
 initscr();
 noecho();
 cbreak();
 keypad(stdscr, true);
 init_colors();
 curs_set(0);
 rand();
 game g;
 while (!g.do_turn());
 erase();
 endwin();
 system("clear");
 return 0;
}

