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
#include <SDL/SDL.h>
#undef main
int main(int argc, char *argv[])
{
 srand(time(NULL));
 initscr();
 noecho();
 cbreak();
 keypad(stdscr, true);
 init_colors();
 curs_set(0);
 SDL_WM_SetCaption("Cataclysm",NULL);
 rand();
 game g;
 while (!g.do_turn());
 erase();
 endwin();
 system("clear");
 return 0;
}

