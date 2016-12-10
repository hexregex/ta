#ifndef AT_TA_TA
#define AT_TA_TA

/* File descriptors for the communication pipes.
 * 'ta' == main, 'in' == input, 'out' == output, 'plr' == player */
                      /* __________    */
int ta_read_from_in;  /* | input  |  4 */
int in_write_to_ta;   /* |________|  5 */
int out_read_from_ta; /* | output |  6 */
int ta_write_to_out;  /* |________|  7 */
int ta_read_from_plr; /* | player |  8 */
int plr_write_to_ta;  /* |        |  9 */
int plr_read_from_ta; /* |        | 10 */
int ta_write_to_plr;  /* |________| 11 */

#endif /* AT_TA_TA */
