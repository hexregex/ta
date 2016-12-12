#ifndef AT_NCURSES_TA
#define AT_NCURSES_TA

#include "communicate.h"
#include "output.h"

void nc_init();
void nc_dest();

void nc_play_time_str(const char *time_str);
void nc_track_list(Track *track_list, int track_count);
void nc_track(Track *track);
void nc_operation(OutCode operation);

#endif /* AT_NCURSES_TA */
