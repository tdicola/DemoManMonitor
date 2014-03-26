// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Interface for a source of audio data.

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class AudioSource {
public:
	virtual ~AudioSource() {}
	// Fill the provided byte buffer with audio.
	virtual void record(std::vector<uint8_t>& buffer) = 0;

};
