#include "pitches.h"

/*
Each song requires three variables:
  song[] lists notes of the song
  song_tempo[] defines length of the each notes
  song_length is the length of song array
*/

int totoro[] = {
  NOTE_E5, NOTE_G5, NOTE_C6, NOTE_G5, // あるこー あ
  NOTE_A5, NOTE_G5, 0, NOTE_C5,       // るこー わ
  NOTE_E5, NOTE_G5, NOTE_C6, NOTE_B5, // たしはげ
  NOTE_A5, NOTE_G5, 0, NOTE_A5,       // んきー あ
  NOTE_A5, NOTE_A5, NOTE_A5, NOTE_C6, // るくのーだ
  NOTE_B5, NOTE_A5, NOTE_G5, 0,       // いすきー
  NOTE_A5, NOTE_G5, NOTE_A5, NOTE_G5, // どんどん
  NOTE_D5, NOTE_E5, NOTE_C5           // いこう
};
int totoro_tempo[] = {
  8, 8, 4, 8,
  8, 4, 12, 12,
  12, 12, 8, 12,
  12, 2, 8, 12,
  12, 12, 8, 12,
  12, 12, 2, 8,
  12, 12, 12, 12,
  8, 8, 2
};
int totoro_length = sizeof(totoro);

int geragera[] = {
  NOTE_FS4, NOTE_G4, NOTE_F4, NOTE_G4, // げらげら
  NOTE_DS4, 0, NOTE_F4, NOTE_G4,       // ぽー げーら
  NOTE_FS4, NOTE_G4, NOTE_DS4, 0,      // げらぽー
  NOTE_FS4, NOTE_G4, NOTE_F4, NOTE_G4, // げらげら
  NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_G4, // ぽーぽげーら
  NOTE_FS4, NOTE_G4, NOTE_DS4, 0      // げらぽー
};
int geragera_tempo[] = {
  8, 8, 8, 8,
  4, 8, 4, 8,
  8, 8, 4, 4,
  8, 8, 8, 8,
  4, 8, 4, 8,
  8, 8, 4, 4
};
int geragera_length = sizeof(geragera);

int yukidaruma[] = {
  NOTE_DS5, NOTE_DS5, NOTE_DS5, NOTE_AS4, // ゆきだる
  NOTE_DS5, NOTE_G5, NOTE_F5, NOTE_G5,    // まつくろー
  0, NOTE_DS5, NOTE_DS5, NOTE_AS4,        //  どあを
  NOTE_DS5, NOTE_G5, NOTE_F5, 0,          // あけて
  NOTE_DS5, NOTE_DS5, NOTE_AS4, NOTE_DS5, // いっしょに
  NOTE_G5, NOTE_A5, NOTE_G5, NOTE_DS5,    // あそぼう
  0, NOTE_DS5, NOTE_A5, NOTE_G5,          //  どうし
  NOTE_DS5, 0, NOTE_DS5, NOTE_DS5,        // て でて
  NOTE_AS4, NOTE_DS5, NOTE_G5, NOTE_B5    // こないの？
};
int yukidaruma_tempo[] = {
  8, 8, 8, 8,
  8, 8, 4, 2,
  3, 8, 8, 8,
  8, 8, 1, 3,
  8, 8, 8, 8,
  8, 8, 8, 4,
  8, 8, 8, 8,
  4, 8, 8, 8,
  8, 8, 8, 1
};
int yukidaruma_length = sizeof(yukidaruma);

int yume_dora[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_D4, // こころの
  NOTE_E4, NOTE_F4, 0, NOTE_C4,       // なか いー
  NOTE_C4, NOTE_D4, NOTE_D4, NOTE_E4,      // つもいつ
  NOTE_F4, 0, NOTE_A4, NOTE_A4, // も えが
  NOTE_G4, NOTE_FS4, NOTE_G4, 0 // いてるー
};
int yume_dora_tempo[] = {
  8, 8, 8, 8,
  8, 8, 8, 4,
  8, 8, 8, 8,
  8, 8, 8, 8,
  8, 8, 3, 8
};
int yume_dora_length = sizeof(yume_dora);

int shaka[] = {
  NOTE_F4, NOTE_D5, NOTE_DS5, NOTE_D5,  // おおTD
  NOTE_C5, 0, NOTE_G4, NOTE_A4,         // K おお
  NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_F4,  // おshaka
  0, NOTE_D4, NOTE_DS4, NOTE_F4,        //  ほこれ
  NOTE_AS4, NOTE_D5, NOTE_D5, NOTE_AS4, // よそのな
  NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,   // たから
  NOTE_C5, NOTE_AS4                     // かに
};
int shaka_tempo[] = {
  4, 2, 4, 4,
  3, 12, 8, 8,
  2, 4, 4, 3,
  8, 4, 8, 4,
  4, 4, 4, 4,
  4, 3, 8, 4,
  4, 1
};
int shaka_length = sizeof(shaka);

// from toneMelody example
int theEnd[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int theEnd_tempo[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
int theEnd_length = sizeof(theEnd);

//Mario main theme melody
//http://www.princetronics.com/supermariothemesong/
int marioMain[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int marioMain_tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
int marioMain_length = sizeof(marioMain);

//Underworld melody
int marioUnderworld[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
//Underwolrd tempo
int marioUnderworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};
int marioUnderworld_length = sizeof(marioUnderworld);

// http://www.mariopiano.com/index.html
int mario1Up[] = {
  NOTE_E5, NOTE_G5, NOTE_E6, NOTE_C6,
  NOTE_D6, NOTE_G6, 0, 0
};
int mario1Up_tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12
};
int mario1Up_length = sizeof(mario1Up);

int marioPowerUp[] = {
  NOTE_B2, NOTE_D3, NOTE_D5, NOTE_G5, NOTE_B5,
  NOTE_CS3, NOTE_E3, NOTE_E5, NOTE_A5, NOTE_C6,
  NOTE_DS3, NOTE_F3, NOTE_F5, NOTE_B5, NOTE_D6
};
int marioPowerUp_tempo[] = {
  20, 20, 20, 20, 20,
  20, 20, 20, 20, 20,
  20, 20, 20, 20, 20
};
int marioPowerUp_length = sizeof(marioPowerUp);

int marioCoin[] = {
  NOTE_B5, NOTE_E6
};
int marioCoin_tempo[] = {
  16, 2
};
int marioCoin_length = sizeof(marioCoin);
