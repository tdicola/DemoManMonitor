// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Interface for keyword spotting algorithm.

#pragma once

#include <string>

class KeywordSpotter {
public:
	virtual ~KeywordSpotter() {}
	virtual std::string update() = 0;

};
