// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "AlsaSource.h"

#include <iostream>

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
	snd_pcm_hw_params_t* hwparams;
	snd_pcm_hw_params_alloca(&hwparams);
	if (snd_pcm_hw_params_any(_device, hwparams) < 0) {
		throw runtime_error("Call to snd_pc_hw_params_any failed.");
	}
	if (snd_pcm_hw_params_set_access(_device, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
		throw runtime_error("Call to snd_pcm_hw_params_set_access failed.");
	}
	if (snd_pcm_hw_params_set_rate(_device, hwparams, rate, 0) < 0) {
		throw runtime_error("Call to snd_pcm_hw_params_set_rate failed.");
	}
	if (snd_pcm_hw_params_set_channels(_device, hwparams, channels) < 0) {
		throw runtime_error("Call to snd_pcm_hw_params_set_channels failed.");
	}
	if (snd_pcm_hw_params_set_format(_device, hwparams, format) < 0) {
		throw runtime_error("Call to snd_pcm_hw_params_set_format failed.");
	}
	int result = snd_pcm_hw_params(_device, hwparams);
	if (result < 0) {
		throw runtime_error("Error setting device parameters: " + string(snd_strerror(result)));
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
