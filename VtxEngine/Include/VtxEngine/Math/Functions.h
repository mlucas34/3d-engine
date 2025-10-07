#pragma once

class Math
{
public:
	static float Lerp(float a, float b, float delta)
	{
		return a * (1.0f - delta) + b * (delta);
	}
};