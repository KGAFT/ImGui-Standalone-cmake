#ifndef DRAWING_H
#define DRAWING_H

#include <cstdint>

#include "pch.h"

class Drawing
{
private:
	static LPCSTR lpWindowName;
	static ImVec2 vWindowSize;
	static ImGuiWindowFlags WindowFlags;
	static bool bDraw;

public:
	static void Init();
	static void Active();
	static bool isActive();
	static void Draw(uint32_t monitor_width, uint32_t monitor_height);
};

#endif
