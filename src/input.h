#ifndef AT_INPUT_TA
#define AT_INPUT_TA

typedef enum
{
    NOP, /* No operation, no input, no key, do nothing */
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ESC,
    SPACE,
    SEMICOLON,
    H,
    J,
    K,
    L,
    Q
} InCode;

void load_keymap ();

InCode in_str_to_code(const char * in_str);

void in_process_go (int fd_write_to_main);

#endif /* AT_INPUT_TA */
