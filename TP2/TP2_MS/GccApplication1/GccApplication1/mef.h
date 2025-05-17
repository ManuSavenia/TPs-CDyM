#ifndef MEF_H
#define MEF_H

void init_mef();
void update_mef(uint8_t key);
void update_hp();
void reset_time();
uint8_t get_random_word(char *dict[], uint8_t num_words);
#endif // MEF_H