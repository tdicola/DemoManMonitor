#pragma once

#include <string>

class KeywordSpotter {
public:
	virtual ~KeywordSpotter() {}
	virtual std::string update() = 0;

};
