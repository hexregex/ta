#ifndef AT_NCURSES_TA
#define AT_NCURSES_TA

#include "communicate.h"
#include "output.h"

void nc_out_init();
void nc_track_list(Track *track_list);
void nc_track(Track *track);
void nc_play_time(int seconds);
void nc_operation(OutCode operation);
void nc_dest();

#endif /* AT_NCURSES_TA */
