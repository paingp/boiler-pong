
#ifndef MUSIC_H_
#define MUSIC_H_

void init_dac(void);
void music_off(void);
void init_tim6(void);

void note_off(int time, int chan, int key, int velo);

void note_on(int time, int chan, int key, int velo);

void set_tempo(int time, int value, const MIDI_Header *hdr);

void pitch_wheel_change(int time, int chan, int value);

void init_tim2(int n);

#endif
