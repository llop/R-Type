#include "cSound.h"


cSoundManager::cSoundManager() {
	_tiempo = 0;
	_ultimoSonido = -6;
}

void cSoundManager::cargaSonido(int id, const char* ficheroSonido, bool loop, int num, long long delay) {
	if (_sounds[id].empty()) {
		_sounds[id].resize(num);
		_lastPlayed[id].resize(num);
		for (int i = 0; i < num; ++i) {
			_sounds[id][i].loadWAV(ficheroSonido, loop);
			_lastPlayed[id][i] = -delay;
		}
		_delay[id] = delay;
		_soundIndex[id] = 0;
	}
}

void cSoundManager::playSonido(int id) {
	//long long intervalo = _tiempo - _ultimoSonido;
	//if (intervalo < 6) return;

	if (!_sounds[id].empty()) {
		long long interval = _tiempo - _lastPlayed[id][_soundIndex[id]];
		if (interval >= _delay[id]) {
			cSound &sound = _sounds[id][_soundIndex[id]];
			sound.stop();
			sound.rewind();
			sound.play();
			_soundIndex[id] = (_soundIndex[id] + 1) % _sounds[id].size();
			_lastPlayed[id][_soundIndex[id]] = _tiempo;
			_ultimoSonido = _tiempo;
		}
	}
}

void cSoundManager::stopSonido(int id) {
	for (unsigned int i = 0; i < _sounds[id].size(); ++i) {
		_sounds[id][i].stop();
		_sounds[id][i].rewind();
	}
}

void cSoundManager::stopSonidos() {
	for (int id = 0; id < NUM_SOUNDS; ++id) {
		for (unsigned int i = 0; i < _sounds[id].size(); ++i) {
			_sounds[id][i].stop();
			_sounds[id][i].rewind();
		}
	}
}

void cSoundManager::logica() {
	++_tiempo;
}


cSound::cSound() {
	_loaded = false;
	_audioBuffer = 0;
	_audioSource = 0;
}


cSound::~cSound() {
	if (_audioBuffer != 0) alDeleteBuffers(1, &_audioBuffer);
	if (_audioSource != 0) alDeleteSources(1, &_audioSource);
}


bool cSound::loaded() const {
	return _loaded;
}

bool cSound::loadWAV(const char* fileName, bool continuousLoop) {
	ALenum format;
	ALsizei size;
	ALvoid *data;
	ALsizei freq;
	ALboolean loop;

	if (alGetError() != AL_NO_ERROR) return false;
	
	alGenBuffers(1, &_audioBuffer);
	if (alGetError() != AL_NO_ERROR) return false;

	// Generate the source.   
	alGenSources(1, &_audioSource);
	if (alGetError() != AL_NO_ERROR) return false;

	// Load the WAV file   
	alutLoadWAVFile((ALbyte *)fileName, &format, &data, &size, &freq, &loop);
	if (data == NULL) return false;

	// Copy the audio date into the buffer.   
	alBufferData(_audioBuffer, format, data, size, freq);
	if (alGetError() != AL_NO_ERROR) return false;

	// Unload the WAV file. It's not needed now.   
	alutUnloadWAV(format, data, size, freq);
	if (alGetError() != AL_NO_ERROR) return false;

	// Connect the buffer to the source.   
	alSourcei(_audioSource, AL_BUFFER, _audioBuffer);
	if (alGetError() != AL_NO_ERROR) return false;

	// Position of the source of the sound.   
	ALfloat sourcePosition[] = { 0.0, 0.0, 0.0 };
	// Velocity of the source of the sound.   
	ALfloat sourceVelocity[] = { 0.0, 0.0, 0.0 };

	alSourcei(_audioSource, AL_BUFFER, _audioBuffer);
	alSourcef(_audioSource, AL_PITCH, 1.0f);
	alSourcef(_audioSource, AL_GAIN, 1.0f);
	alSourcefv(_audioSource, AL_POSITION, sourcePosition);
	alSourcefv(_audioSource, AL_VELOCITY, sourceVelocity);
	alSourcei(_audioSource, AL_LOOPING, continuousLoop);
	if (alGetError() != AL_NO_ERROR) return false;
	return _loaded = true;
}


bool cSound::play() {
	alSourcePlay(_audioSource);
	return (alGetError() == AL_NO_ERROR);
}


bool cSound::pause() {
	alSourcePause(_audioSource);
	return (alGetError() == AL_NO_ERROR);
}


bool cSound::stop() {
	alSourceStop(_audioSource);
	return (alGetError() == AL_NO_ERROR);
}


bool cSound::rewind() {
	alSourceRewind(_audioSource);
	return (alGetError() == AL_NO_ERROR);
}


void cSound::gain(float gainLevel) {
	alSourcef(_audioSource, AL_GAIN, gainLevel);
}

float cSound::gain() {
	float gainLevel;
	alGetSourcef(_audioSource, AL_GAIN, &gainLevel);
	return (gainLevel);
}

