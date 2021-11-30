#pragma once
#include <cstdint>

class Color_t
{
	Color_t()
	{
		R = 0;
		G = 0;
		B = 0;
	}

	Color_t(uint8_t red, uint8_t green, uint8_t blue)
	{
		R = red;
		G = green;
		B = blue;
	}


	uint8_t R;
	uint8_t G;
	uint8_t B;
};