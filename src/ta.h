#ifndef AT_TA_TA
#define AT_TA_TA

/* File descriptors for the communication pipes between modules.
 * The numbers indicate the probable values assigned.
 * 'ta' == main, 'in' == input, 'out' == output, 'plr' == player
.__________________________________________________,
|*/  int ta_read_from_in;   /*  4| read from input |\
|*/  int in_write_to_ta;    /*  5|_________________| \
|*/  int out_read_from_ta;  /*  6| write to output |\ |
|*/  int ta_write_to_out;   /*  7| _ _ _ _ _ _ _ _ | \|
|*/  int out_read_from_plr; /*  8|  (from player)  |. |
|*/  int plr_write_to_out;  /*  9|_________________| .|
|*/  int plr_read_from_ta;  /* 10| write to player |\ |
|*/  int ta_write_to_plr;   /* 11|                 | \|
|________________________________|_________________|  |
 \                                \                 \ |
  \________________________________\_________________\|

       */
#endif /* AT_TA_TA
     */
