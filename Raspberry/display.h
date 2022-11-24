#ifndef DISPLAY_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define DISPLAY_H

void write_string(char *string);
void clear_display(void);
void init_display(void);
void write_char(char c);

/**
 * @brief Escreve uma string no display
 *
 * @param s string a ser escrita no display
 */
void write_string(char* s) { for (int i = 0; i < strlen(s) && i < 16; i++) write_char(s[i]); }


#endif