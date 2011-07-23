/* Main Loop for cataclysm
 * Linux only I guess
 * But maybe not
 * Who knows
 */

#include <curses.h>
#include <ctime>
#include "game.h"
#include "color.h"
#if (defined _WIN32 || defined __WIN32__) && ! defined __CYGWIN__
#include <SDL/SDL.h>
#undef main
#endif
int main(int argc, char *argv[])
{
 srand(time(NULL));
 initscr();
 noecho();
 cbreak();
 keypad(stdscr, true);
 init_colors();
 curs_set(0);
#if (defined _WIN32 || defined __WIN32__) && ! defined __CYGWIN__
    SDL_WM_SetCaption("Cataclysm",NULL);
#endif
 rand();
 game g;
 while(!g.do_turn());
 erase();
 endwin();
 system("clear");
 return 0;
}

