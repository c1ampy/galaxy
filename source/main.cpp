#include <stdlib.h>
#include <stdio.h>
#include "sound.h"


int main() {
	
	initSound();
	playSound(SOUND_MENU);
	playSound(SOUND_GAMEOVER);

	return 0;
}
