#pragma once

#include "Globals.h"


class cSound {
public:
	cSound();
	~cSound();

	bool loadWAV(const char* fileName, bool continuousLoop);
	bool play();
	bool pause();
	bool resume();
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
#define SOUND_NIVEL1_INTRO 1
#define SOUND_NIVEL1 2
#define SOUND_NIVEL2 3
#define SOUND_NIVEL3 4
#define SOUND_NIVEL4 5
#define SOUND_JEFE_INTRO 6
#define SOUND_JEFE 7
#define SOUND_SCORE 8
#define SOUND_JUEGO_COMPLETO 9
#define SOUND_SET_HI_SCORE 10
#define SOUND_GAME_OVER 11

#define SOUND_OPCION_MENU 12
#define SOUND_START_GAME 13

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


struct cSoundWrapper {
	vector<cSound> _sources;
	int _numSources;
	long long _lastPlayed;
	long long _delay;
	int _queued;
	int _playIndex;
	bool _loaded;

	cSoundWrapper();
	void init(const char* ficheroSonido, bool loop, int numSources, long long delay);

};


#define SONIDO_DELAY 6

class cSoundManager {
private:

	long long _tiempo;
	long long _ultimoSonido;
	list<int> _que;
	cSoundWrapper _sounds[NUM_SOUNDS];

	bool _paused;

	//vector<cSound> _sounds[NUM_SOUNDS];
	//int _soundIndex[NUM_SOUNDS];
	//vector<long long> _lastPlayed[NUM_SOUNDS];
	//long long _delay[NUM_SOUNDS];

public:

	cSoundManager();

	void suena();

	void cargaSonido(int id, const char* ficheroSonido, bool loop, int num, long long delay);
	void playSonido(int id);
	void stopSonido(int id);
	void stopSonidos();

	void pauseSonido(int id);
	void resumeSonido(int id);

	void pauseSonidos();
	void resumeSonidos();

};
