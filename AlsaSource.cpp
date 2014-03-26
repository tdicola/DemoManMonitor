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
	if (_device != nullptr) {
		throw runtime_error("Device already open!");
	}
	// Open the device for audio capture.
	if (snd_pcm_open(&_device, hw.c_str(), SND_PCM_STREAM_CAPTURE, 0) < 0) {
		throw runtime_error("Failed to open " + string(hw) + " for capture.");
	}
	// Set device parameters.
	if (snd_pcm_set_params(_device, format, SND_PCM_ACCESS_RW_INTERLEAVED, channels, rate, 1, 100000) < 0) {
		throw runtime_error("Failed to set device parameters.");
	}
	// Save the number of bytes per sample for later buffer calculations.
	_formatSize = snd_pcm_format_size(format, 1);
}

void AlsaSource::record(std::vector<uint8_t>& buffer) {
	if (_device == nullptr) {
		throw runtime_error("Device must be open!");
	}
	// Fill buffer with captured audio.
	size_t size = buffer.size() / _formatSize;
	size_t captured = snd_pcm_readi(_device, buffer.data(), size);
	if (captured != size) {
		throw runtime_error("Audio buffer underrun!");
	}
}

void AlsaSource::pause() {
	if (_device == nullptr) {
		throw runtime_error("Device must be open!");
	}
	if (snd_pcm_drop(_device) < 0) {
		throw runtime_error("Call to snd_pcm_drop failed.");
	}
}

void AlsaSource::resume() {
	if (_device == nullptr) {
		throw runtime_error("Device must be open!");
	}
	if (snd_pcm_prepare(_device) < 0) {
		throw runtime_error("Call to snd_pcm_prepare failed.");
	}
}
