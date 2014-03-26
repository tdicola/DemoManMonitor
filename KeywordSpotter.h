// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Interface for keyword spotting algorithm.

#pragma once

#include <cstdint>
#include <string>
#include <vector>

class KeywordSpotter {
public:
	virtual ~KeywordSpotter() {}
	// Send in a buffer of 16khz, mono, signed 16bit little endian audio and read back any spotted keyword.
	// Return an empty string if no keyword is spotted.
	virtual std::string process(std::vector<uint8_t>& buffer) = 0;

};
