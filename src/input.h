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
void in_process_go (int fd_write_to_main);

#endif /* AT_INPUT_TA */
