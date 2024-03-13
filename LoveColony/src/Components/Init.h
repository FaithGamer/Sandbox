#pragma once

/// @brief Entity factory interface
class Init
{
public:
	virtual ~Init(){}
	virtual void CreateEntity() = 0;
};