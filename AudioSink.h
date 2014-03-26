// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Simple audio playback sink interface.

#pragma once

#include <cstdint>
#include <vector>

class AudioSink {
public:
	virtual ~AudioSink() {}
	// Play back the provided buffer of audio.
	virtual void play(std::vector<uint8_t>& buffer) = 0;

};
