#pragma once

#include "Globals.h"


class cSound {
public:
	cSound();
	~cSound();

	bool loadWAV(const char* fileName, bool continuousLoop);
	bool play();
	bool pause();
	bool stop();
	bool rewind();

	bool loaded() const;

	void gain(float gainLevel);
	float gain();

private:
	bool _loaded;
	ALuint _audioBuffer;
	ALuint _audioSource;
};



#define SOUND_PANTALLA_INICIAL 0
#define SOUND_NIVEL1 1
#define SOUND_NIVEL2 2
#define SOUND_NIVEL3 3
#define SOUND_NIVEL4 4
#define SOUND_JEFE1 5
#define SOUND_JEFE2 6
#define SOUND_JEFE3 7
#define SOUND_JEFE4 8
#define SOUND_SCORE 9
#define SOUND_JUEGO_COMPLETO 10
#define SOUND_SET_HI_SCORE 11
#define SOUND_GAME_OVER 12

#define SOUND_OPCION_MENU 13

#define SOUND_EXPLO1 14
#define SOUND_EXPLO2 15

#define SOUND_DISPARO_ENEMIGO 16
#define SOUND_DISPARO_JEFE1 17
#define SOUND_DISPARO_GUSANO 18

#define SOUND_DISPARO_NAVE 19
#define SOUND_DISPARO_RB_NAVE 20
#define SOUND_CARGA_DISPARO 21
#define SOUND_DISPARO_CARGADO 22

#define SOUND_DISPARO_ESCUDO1 23
#define SOUND_DISPARO_ESCUDO2 24

#define NUM_SOUNDS 25


class cSoundManager {
private:

	vector<cSound> _sounds[NUM_SOUNDS];
	int _soundIndex[NUM_SOUNDS];
	vector<long long> _lastPlayed[NUM_SOUNDS];
	long long _delay[NUM_SOUNDS];

public:
	void cargaSonido(int id, const char* ficheroSonido, bool loop, int num, long long delay);
	void playSonido(int id, long long time);
	void stopSonido(int id);

	void stopSonidos();

};
