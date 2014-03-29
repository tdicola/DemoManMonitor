// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "AlsaSink.h"

using namespace std;

AlsaSink::AlsaSink():
	_device(nullptr),
	_formatSize(0)
{}

AlsaSink::~AlsaSink() {
	// Close the device if it's open.
	if (_device != nullptr) {
		snd_pcm_close(_device);
	}
}

void AlsaSink::open(const string& hw, const int rate, const int channels, const snd_pcm_format_t format) {
	if (_device != nullptr) {
		throw runtime_error("Device already open!");
	}
	// Open the device for audio capture.
	if (snd_pcm_open(&_device, hw.c_str(), SND_PCM_STREAM_PLAYBACK, 0) < 0) {
		throw runtime_error("Failed to open " + string(hw) + " for playback.");
	}
	// Set device parameters.
	if (snd_pcm_set_params(_device, format, SND_PCM_ACCESS_RW_INTERLEAVED, channels, rate, 1, 100000) < 0) {
		throw runtime_error("Failed to set device parameters.");
	}
	// Save the number of bytes per sample for later buffer calculations.
	_formatSize = snd_pcm_format_size(format, 1);
}

void AlsaSink::play(std::vector<uint8_t>& buffer) {
	// if (_device == nullptr) {
	// 	throw runtime_error("Device must be open!");
	// }
	// // Play audio from buffer.
	// size_t size = buffer.size() / _formatSize;
	// size_t played = snd_pcm_writei(_device, buffer.data(), size);
	// if (played != size) {
	// 	throw runtime_error("Audio buffer underrun!");
	// }
	play(buffer.data(), buffer.size() / _formatSize);
}

void AlsaSink::play(uint8_t* buffer, size_t frames) {
	if (_device == nullptr) {
		throw runtime_error("Device must be open!");
	}
	size_t played = snd_pcm_writei(_device, buffer, frames);
	if (played != frames) {
		throw runtime_error("Audio buffer underrun!");
	}
}

unsigned long AlsaSink::available() {
	auto result = snd_pcm_avail(_device);
	if (result < 0) {
		throw runtime_error("Call to snd_pcm_avail failed.");
	}
	return (unsigned long)result;
}

void AlsaSink::pause() {
	if (_device == nullptr) {
		throw runtime_error("Device must be open!");
	}
	if (snd_pcm_drop(_device) < 0) {
		throw runtime_error("Call to snd_pcm_drop failed.");
	}
}

void AlsaSink::resume() {
	if (_device == nullptr) {
		throw runtime_error("Device must be open!");
	}
	if (snd_pcm_prepare(_device) < 0) {
		throw runtime_error("Call to snd_pcm_prepare failed.");
	}
}

