// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Simple audio recording source interface.

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class AudioSource {
public:
	virtual ~AudioSource() {}
	// Fill the provided byte buffer with audio.
	virtual bool record(std::vector<uint8_t>& buffer) = 0;
	// Stop recording audio.
	virtual void pause() = 0;
	// Resume recording audio after stop.
	virtual void resume() = 0;
};
