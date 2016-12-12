#ifndef AT_TA_TA
#define AT_TA_TA

/* File descriptors for the communication pipes.
 * 'ta' == main, 'in' == input, 'out' == output, 'plr' == player */
                       /* ___________________    */
int ta_read_from_in;   /* | read from input |  4 */
int in_write_to_ta;    /* |_________________|  5 */
int out_read_from_ta;  /* | write to output |  6 */
int ta_write_to_out;   /* | _ _ _ _ _ _ _ _ |  7 */
int out_read_from_plr; /* |  (from player)  |  8 */
int plr_write_to_out;  /* |_________________|  9 */
int plr_read_from_ta;  /* | write to player |  10 */
int ta_write_to_plr;   /* |_________________| 11 */

#endif /* AT_TA_TA */
