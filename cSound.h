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
#define SOUND_START_GAME 14

#define SOUND_EXPLO1 15
#define SOUND_EXPLO2 16

#define SOUND_DISPARO_ENEMIGO 17
#define SOUND_DISPARO_JEFE1 18
#define SOUND_DISPARO_GUSANO 19

#define SOUND_DISPARO_NAVE 20
#define SOUND_DISPARO_RB_NAVE 21
#define SOUND_CARGA_DISPARO 22
#define SOUND_DISPARO_CARGADO 23

#define SOUND_DISPARO_ESCUDO1 24
#define SOUND_DISPARO_ESCUDO2 25



#define NUM_SOUNDS 26


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
	void pauseSonidos();

};
