#include "cSound.h"


cSoundWrapper::cSoundWrapper() {
	_loaded = false;
}

void cSoundWrapper::init(const char* ficheroSonido, bool loop, int numSources, long long delay) {
	_numSources = numSources;
	_sources.resize(_numSources);
	for (int i = 0; i < _numSources; ++i) _sources[i].loadWAV(ficheroSonido, loop);
	_delay = delay;
	_lastPlayed = -_delay;
	_queued = 0;
	_playIndex = 0;
	_loaded = true;
}


cSoundManager::cSoundManager() {
	_tiempo = 0;
	_ultimoSonido = -SONIDO_DELAY;

	_paused = false;
}

void cSoundManager::cargaSonido(int id, const char* ficheroSonido, bool loop, int num, long long delay) {
	if (!_sounds[id]._loaded) {
		_sounds[id].init(ficheroSonido, loop, num, delay);
	}
}

void cSoundManager::playSonido(int id) {
	//long long intervalo = _tiempo - _ultimoSonido;
	//if (intervalo < SONIDO_DELAY) return;

	if (_sounds[id]._loaded) {

		if (_sounds[id]._queued < 1) {//_sounds[id]._numSources) {
			++_sounds[id]._queued;
			_que.push_back(id);
		}

	}
}

void cSoundManager::stopSonido(int id) {
	if (_sounds[id]._loaded) {
		for (int i = 0; i < _sounds[id]._numSources; ++i) {
			_sounds[id]._sources[i].stop();
			_sounds[id]._sources[i].rewind();
			_sounds[id]._queued = 0;
		}
		_que.remove(id);
	}
}

void cSoundManager::stopSonidos() {
	for (int id = 0; id < NUM_SOUNDS; ++id) {
		stopSonido(id);
	}
}

void cSoundManager::pauseSonido(int id) {
	if (_sounds[id]._loaded) {
		for (int i = 0; i < _sounds[id]._numSources; ++i) {
			_sounds[id]._sources[i].pause();
		}
	}
}

void cSoundManager::resumeSonido(int id) {
	if (_sounds[id]._loaded) {
		for (int i = 0; i < _sounds[id]._numSources; ++i) {
			_sounds[id]._sources[i].resume();
		}
	}
}

void cSoundManager::pauseSonidos() {
//	if (!_paused) {
//		_paused = true;
		for (int id = 0; id < NUM_SOUNDS; ++id) {
			pauseSonido(id);
		}
		//	}
}

void cSoundManager::resumeSonidos() {
	//if (_paused) {
	//_paused = false;
		for (int id = 0; id < NUM_SOUNDS; ++id) {
			resumeSonido(id);
		}
		//}
}

void cSoundManager::suena() {
	++_tiempo;

	long long intervalo = _tiempo - _ultimoSonido;
	if (intervalo < SONIDO_DELAY) return;

	if (!_que.empty()) {
		list<int>::iterator it = _que.begin();
		while (it != _que.end()) {
			int id = *it;

			bool played = false;
			if (_sounds[id]._loaded && _sounds[id]._queued) {
				long long inter = _tiempo - _sounds[id]._lastPlayed;
				if (inter > _sounds[id]._delay) {
					int index = _sounds[id]._playIndex;
					cSound &sound = _sounds[id]._sources[index];
					sound.stop();
					sound.rewind();
					sound.play();
					_sounds[id]._playIndex = (index + 1) % _sounds[id]._numSources;
					_sounds[id]._lastPlayed = _tiempo;
					--_sounds[id]._queued;

					_ultimoSonido = _tiempo;
					played = true;
				}
			}

			if (played) {
				_que.erase(it);
				return;
			} else ++it;

		}
	}
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
	ALint state;
	alGetSourcei(_audioSource, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING) alSourcePause(_audioSource);
	return (alGetError() == AL_NO_ERROR);
}

bool cSound::resume() {
	ALint state;
	alGetSourcei(_audioSource, AL_SOURCE_STATE, &state);
	if (state == AL_PAUSED) alSourcePlay(_audioSource);
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

