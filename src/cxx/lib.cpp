//
// Created by kgaft on 14.12.2025.
//

#include "UI.h"
#ifdef __cplusplus
extern "C" {
#endif

struct Vec2
{
    float x, y;
};

struct RectDrawCommand
{
    //X,Y,WIDTH,HEIGHT
    float coords[4];
    float thickness;
    float rounding;
    float color[4];
    bool filled;
};


struct LineDrawCommand
{
    //POINT1, POINT2
    float coords[4];
    float thickness;
    float color[4];
};

struct TextDrawCommand
{
    float coords[2];
    char* text;
    float thickness;
    float color[4];
};


struct WindowDrawCommand
{
    char* title;
    uint32_t width;
    uint32_t height;
};

struct ButtonDrawCommand
{
    char* title;
    float size[2];
};

struct CheckBoxCommand
{
    char* title;
    bool* p_res_out;
};

struct WindowTextDrawCommand
{
    char* text;
};

struct SliderCommand
{
    char* title;
    float* p_res_out;
    float min_val;
    float max_val;
};

struct ComboBoxCommand
{
    char** items;
    int32_t* item_output;
    int32_t items_amount;
    char* title;
};

struct ColorPickerCommand
{
    float* rgba_out;
    char* title;
};

void initialize_context()
{
    UI::Initialize();
}

void pre_render_event()
{
    UI::PreRenderEvent();
}

void post_render_event()
{
    UI::PostRenderEvent();
}

void destroy_context()
{
    UI::Destroy();
}

bool need_exit()
{
    return UI::NeedExit();
}

void begin_window(WindowDrawCommand* cmd)
{
    ImGui::SetNextWindowSize(ImVec2(cmd->width, cmd->height));
    ImGui::Begin(cmd->title);
}

bool window_button(ButtonDrawCommand* cmd)
{
    return ImGui::Button(cmd->title, ImVec2(cmd->size[0], cmd->size[1]));
}

void window_checkbox(CheckBoxCommand* cmd)
{
    ImGui::Checkbox(cmd->title, cmd->p_res_out);
}

void window_text(WindowTextDrawCommand* cmd)
{
    ImGui::Text(cmd->text);
}

void window_slider(SliderCommand* cmd)
{
    ImGui::SliderFloat(cmd->title, cmd->p_res_out, cmd->min_val, cmd->max_val);
}

void window_combo_box(ComboBoxCommand* cmd)
{
    ImGui::Combo(cmd->title, cmd->item_output, cmd->items, cmd->items_amount);
}

void window_color_picker(ColorPickerCommand* cmd)
{
    ImGui::ColorEdit4(cmd->title, cmd->rgba_out,   ImGuiColorEditFlags_AlphaBar |
    ImGuiColorEditFlags_PickerHueWheel);
}

void same_line(float* spacings)
{
    if (spacings[0] < 0 && spacings[1] < 0)
    {
        ImGui::SameLine();
    }
    else
    {
        ImGui::SameLine(spacings[0], spacings[1]);
    }
}

bool begin_table(char* title, uint32_t columns)
{
    return ImGui::BeginTable(title, columns, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
}

void table_next_row()
{
    ImGui::TableNextRow();
}

void table_set_column_index(uint32_t column)
{
    ImGui::TableSetColumnIndex(column);
}

void end_table()
{
    ImGui::EndTable();
}

void enable_click_through(bool state)
{
    UI::EnableClickThrough(state);
}

void end_window()
{
    ImGui::End();
}

void draw_rect(RectDrawCommand* command)
{
    //rect
    if (!command->filled)
    {
        ImGui::GetBackgroundDrawList()->AddRect(ImVec2(command->coords[0], command->coords[1]),
                                                ImVec2(command->coords[2], command->coords[3]),
                                                IM_COL32(command->color[0]*255, command->color[1]*255,
                                                         command->color[2]*255,
                                                         command->color[3]*255),
                                                command->rounding, 0, command->thickness);
    }
    else
    {
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(command->coords[0], command->coords[1]),
                                                      ImVec2(command->coords[2], command->coords[3]),
                                                      IM_COL32(command->color[0]*255, command->color[1]*255,
                                                               command->color[2]*255,
                                                               command->color[3]*255),
                                                      command->rounding, 0);
    }
}

void draw_line(LineDrawCommand* command)
{
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(command->coords[0], command->coords[1]),
                                            ImVec2(command->coords[2], command->coords[3]), IM_COL32(
                                                command->color[0]*255, command->color[1]*255,
                                                command->color[2]*255,
                                                command->color[3]*255), command->thickness);
}

void draw_text(TextDrawCommand* command)
{
    ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), command->thickness,
                                            ImVec2(command->coords[0], command->coords[1]), IM_COL32(
                                                command->color[0]*255, command->color[1]*255,
                                                command->color[2]*255,
                                                command->color[3]*255), command->text);
}

Vec2 calculate_text_size(char* text)
{
    auto res = ImGui::CalcTextSize(text);
    return Vec2{res.x, res.y};
}

uint32_t get_monitor_width()
{
    return UI::Width();
}

uint32_t get_monitor_height()
{
    return UI::Height();
}

#ifdef __cplusplus
}
#endif
