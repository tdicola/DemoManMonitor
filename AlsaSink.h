// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// ALSA-based audio playback sink.

#pragma once

#include <algorithm>
#include <stdexcept>

#include <alsa/asoundlib.h>

#include "AlsaError.h"
#include "AudioSink.h"

class AlsaSink: public AudioSink {
public:
	AlsaSink();
	~AlsaSink();
	void open(const std::string& hw, const int rate, const int channels, const snd_pcm_format_t format);
	virtual void play(std::vector<uint8_t>& buffer);
	virtual void playAsync(std::vector<uint8_t>& buffer);
	virtual bool asyncUpdate();
	virtual void pause();
	virtual void resume();

private:
	snd_pcm_t* _device;
	size_t _formatSize;
	size_t _chunkSize;
	uint8_t* _playing;
	size_t _pos;
	size_t _frames;

};
