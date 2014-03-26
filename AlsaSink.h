// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// ALSA-based audio playback sink.

#pragma once

#include <stdexcept>

#include <alsa/asoundlib.h>

#include "AudioSink.h"

class AlsaSink: public AudioSink {
public:
	AlsaSink();
	~AlsaSink();
	void open(const std::string& hw, const int rate, const int channels, const snd_pcm_format_t format);
	virtual void play(std::vector<uint8_t>& buffer);
	virtual void pause();
	virtual void resume();

private:
	snd_pcm_t* _device;
	size_t _formatSize;

};
