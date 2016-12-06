#ifndef AT_INPUT_TA
#define AT_INPUT_TA

typedef enum
{
    NOP, /* No operation, no input, no key, do nothing */
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
InCode in_char_to_code(unsigned char in_char);
void in_process_go (int fd_write_to_main);

#endif /* AT_INPUT_TA */
