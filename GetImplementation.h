#pragma once
#include "Interfaces.h"
#include <memory>

class GetImplementation
{
public:
	static std::unique_ptr<IAlgorithm> algorithm() { return nullptr; }
	static std::unique_ptr<IVisualisedAlgorithm> visualAlgorithm() { return nullptr; }
	static std::unique_ptr<IEngine> engine() { return nullptr; }
};

