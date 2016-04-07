#pragma once

#include "Globals.h"


class cSound {
public:
	cSound();
	~cSound();

	bool loadWAV(const char* fileName, bool continuousLoop, float gain);
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
#define SOUND_EXPLO_NAVE 16

#define SOUND_DISPARO_ENEMIGO 17
#define SOUND_DISPARO_JEFE1 18
#define SOUND_DISPARO_GUSANO 19

#define SOUND_DISPARO_NAVE 20
#define SOUND_DISPARO_RB_NAVE 21
#define SOUND_CARGA_DISPARO 22
#define SOUND_DISPARO_CARGADO 23

#define SOUND_DISPARO_ESCUDO1 24
#define SOUND_DISPARO_ESCUDO2 25

#define SOUND_ITEM_VIDA 26
#define SOUND_ITEM 27

#define SOUND_DISPARO_NAVE_EXPLO1 28
#define SOUND_DISPARO_NAVE_EXPLO2 29
#define SOUND_DISPARO_ENEMIGO_EXPLO1 30
#define SOUND_DISPARO_ENEMIGO_EXPLO2 31

#define SOUND_MAGIA 32
#define SOUND_CLIT 33
#define SOUND_CUC 34
#define SOUND_ENEMIGO_HIT 35

#define NUM_SOUNDS 36


struct cSoundWrapper {
	vector<cSound> _sources;
	int _numSources;
	long long _lastPlayed;
	long long _delay;
	int _queued;
	int _playIndex;
	bool _loaded;

	cSoundWrapper();
	void init(const char* ficheroSonido, bool loop, int numSources, long long delay, float gain);

};


#define SONIDO_MAX_QUEUED 6
#define SONIDO_DELAY 1

class cSoundManager {
private:

	long long _tiempo;
	long long _ultimoSonido;
	list<pair<long long, int> > _que;
	cSoundWrapper _sounds[NUM_SOUNDS];

	bool _paused;

public:

	cSoundManager();

	void suena();

	void cargaSonido(int id, const char* ficheroSonido, bool loop, int num, long long delay, float gain);
	void playSonido(int id);
	void stopSonido(int id);
	void stopSonidos();

	void pauseSonido(int id);
	void resumeSonido(int id);

	void pauseSonidos();
	void resumeSonidos();

};
