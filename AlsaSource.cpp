// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "AlsaSource.h"

using namespace std;

AlsaSource::AlsaSource():
	_device(nullptr),
	_formatSize(0)
{}

AlsaSource::~AlsaSource() {
	// Close the device if it's open.
	if (_device != nullptr) {
		snd_pcm_close(_device);
	}
}

void AlsaSource::open(const string& hw, const int rate, const int channels, const snd_pcm_format_t format) {
	// Open the device for audio capture.
	ALSA_THROW(snd_pcm_open(&_device, hw.c_str(), SND_PCM_STREAM_CAPTURE, 0));
	// Set device parameters.
	ALSA_THROW(snd_pcm_set_params(_device, format, SND_PCM_ACCESS_RW_INTERLEAVED, channels, rate, 1, 100000));
	// Save the number of bytes per sample for later buffer calculations.
	_formatSize = snd_pcm_format_size(format, 1);
}

bool AlsaSource::record(std::vector<uint8_t>& buffer) {
	// Fill buffer with captured audio.
	size_t size = buffer.size() / _formatSize;
	size_t captured = snd_pcm_readi(_device, buffer.data(), size);
	if (captured != size) {
		// Audio buffer underrun, try to recover device and warn that audio wasn't captured.
		ALSA_THROW(snd_pcm_recover(_device, captured, 1));
		return false;
	}
	return true;
}

void AlsaSource::pause() {
	ALSA_THROW(snd_pcm_drop(_device));
}

void AlsaSource::resume() {
	ALSA_THROW(snd_pcm_prepare(_device));
}
