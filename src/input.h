#ifndef AT_INPUT_TA
#define AT_INPUT_TA

typedef enum {
    SPACE,
    J,
    K,
    L,
    H,
    SEMICOLON
} InCode;


void load_keymap ();
void input (int unused, int fd_write);

#endif /* AT_INPUT_TA */
