// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "AlsaSink.h"

using namespace std;

AlsaSink::AlsaSink():
	_device(nullptr),
	_formatSize(0),
	_chunkSize(0),
	_playing(nullptr),
	_pos(0),
	_frames(0)
{}

AlsaSink::~AlsaSink() {
	// Close the device if it's open.
	if (_device != nullptr) {
		snd_pcm_close(_device);
	}
}

void AlsaSink::open(const string& hw, const int rate, const int channels, const snd_pcm_format_t format) {
	// Open the device for audio playback.
	ALSA_THROW(snd_pcm_open(&_device, hw.c_str(), SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK));
	// Set device parameters.
	ALSA_THROW(snd_pcm_set_params(_device, format, SND_PCM_ACCESS_RW_INTERLEAVED, channels, rate, 1, 100000));
	// Save the number of bytes per sample for later buffer calculations.
	_formatSize = snd_pcm_format_size(format, 1);
	// Pick a chunk size for async playback.
	snd_pcm_uframes_t bufferSize;
	snd_pcm_uframes_t periodSize;
	ALSA_THROW(snd_pcm_get_params(_device, &bufferSize, &periodSize))
	_chunkSize = periodSize;
}

void AlsaSink::play(std::vector<uint8_t>& buffer) {
	playAsync(buffer);
	while (asyncUpdate());
}

void AlsaSink::playAsync(std::vector<uint8_t>& buffer) {
	_playing = buffer.data();
	_pos = 0;
	_frames = buffer.size() / _formatSize;
}

bool AlsaSink::asyncUpdate() {
	// Check if there is still audio to play.
	if (_pos < _frames) {
		// Play either a chunk of frames, or all the remaining frames if less than a chunk.
		auto toPlay = min(_chunkSize, _frames - _pos);
		auto written = snd_pcm_writei(_device, _playing + (_pos*_formatSize), toPlay);
		if (written == -EAGAIN) {
			// No space in buffer for new frames, keep trying later.
			return true;
		}
		// Update position based on how many frames were played.
		_pos += written;
		// Keep playing audio.
		return true;
	}
	else {
		// Nothing left to play.
		return false;
	}
}

void AlsaSink::pause() {
	// Enter blocking mode before calling draing (required by drain).
	ALSA_THROW(snd_pcm_nonblock(_device, 0));
	// Wait for audio left in buffer to finish playing.
	ALSA_THROW(snd_pcm_drain(_device));
	// Go back to nonblocking mode.
	ALSA_THROW(snd_pcm_nonblock(_device, 1));
}

void AlsaSink::resume() {
	ALSA_THROW(snd_pcm_prepare(_device));
}

