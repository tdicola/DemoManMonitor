// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// ALSA-based audio source.

#pragma once

#include <stdexcept>

#include <alsa/asoundlib.h>

#include "AudioSource.h"

class AlsaSource: public AudioSource {
public:
	AlsaSource();
	~AlsaSource();
	void open(const std::string& hw, const int rate, const int channels, const snd_pcm_format_t format);
	virtual void record(std::vector<uint8_t>& buffer);

private:
	snd_pcm_t* _device;
	size_t _formatSize;

};
