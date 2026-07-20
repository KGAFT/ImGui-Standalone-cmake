#include "Drawing.h"

#include <optional>

LPCSTR Drawing::lpWindowName = "ImGui Standalone";
ImVec2 Drawing::vWindowSize = {100, 100};
ImGuiWindowFlags Drawing::WindowFlags = 0;
bool Drawing::bDraw = true;



void Drawing::Init() {

}

void Drawing::Active() {
    bDraw = true;
}

bool Drawing::isActive() {
    return bDraw == true;
}

void Drawing::Draw(uint32_t monitor_width, uint32_t monitor_height) {
    if (isActive()) {

    }
}
