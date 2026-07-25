use std::ffi::c_char;

#[repr(C)]
pub struct DrawRectCommand {
    //X,Y,WIDTH,HEIGHT
    pub coords: [f32; 4],
    pub thickness: f32,
    pub rounding: f32,
    pub color: [f32; 4],
    pub filled: bool,
}
#[repr(C)]
pub struct LineDrawCommand
{
    //POINT1, POINT2
    pub coords: [f32; 4],
    pub thickness: f32,
    pub color: [f32; 4],
}

#[repr(C)]
pub struct TextDrawCommand {
    //X,Y
    pub coords: [f32; 2],
    pub text: *const c_char,
    pub thickness: f32,
    pub color: [f32; 4],
}

#[repr(C)]
pub struct Vec2 {
    pub x: f32,
    pub y: f32,
}


#[repr(C)]
pub struct WindowDrawCommand {
    pub title: *const c_char,
    pub width: u32,
    pub height: u32,
}

#[repr(C)]
pub struct ButtonDrawCommand {
    pub title: *const c_char,
    pub size: [f32; 2],
}

#[repr(C)]
pub struct CheckBoxCommand {
    pub title: *const c_char,
    pub p_res_out: *mut bool,
}

#[repr(C)]
pub struct WindowTextDrawCommand {
    pub text: *const c_char,
}

#[repr(C)]
#[derive(Default)]
pub struct SliderCommand {
    pub title: *const c_char,
    pub p_res_out: *mut f32,
    pub min_val: f32,
    pub max_val: f32,
}
#[repr(C)]
#[derive(Default)]
pub struct ComboBoxCommand{
    pub items: *const *const c_char,
    pub item_output: *mut i32,
    pub items_amount: i32,
    pub title: *const c_char
}
#[repr(C)]
#[derive(Default)]
pub struct ColorPickerCommand{
    pub rgba_out: *mut f32,
    pub tile: *const c_char
}


unsafe impl Send for TextDrawCommand{}
unsafe impl Sync for TextDrawCommand{}

unsafe impl Send for LineDrawCommand{}
unsafe impl Sync for LineDrawCommand{}

unsafe impl Send for DrawRectCommand{}
unsafe impl Sync for DrawRectCommand{}

unsafe impl Send for WindowDrawCommand{}

unsafe impl Sync for WindowDrawCommand{}

unsafe impl Send for ButtonDrawCommand{}

unsafe impl Sync for ButtonDrawCommand{}

unsafe impl Send for CheckBoxCommand{}

unsafe impl Sync for CheckBoxCommand{}

unsafe impl Send for WindowTextDrawCommand{}

unsafe impl Sync for WindowTextDrawCommand{}

unsafe impl Send for SliderCommand{}

unsafe impl Sync for SliderCommand{}


unsafe impl Send for ComboBoxCommand{}

unsafe impl Sync for ComboBoxCommand{}

unsafe impl Send for ColorPickerCommand{}

unsafe impl Sync for ColorPickerCommand{}


unsafe extern "C" {
    pub fn initialize_context();

    pub fn pre_render_event();

    pub fn post_render_event();

    pub fn destroy_context();

    pub fn need_exit() -> bool;

    // window
    pub fn begin_window(cmd: *const WindowDrawCommand);
    pub fn end_window();

    // widgets
    pub fn window_button(cmd: *const ButtonDrawCommand) -> bool;
    pub fn window_checkbox(cmd: *const CheckBoxCommand);
    pub fn window_text(cmd: *const WindowTextDrawCommand);
    pub fn window_slider(cmd: *const SliderCommand);

    pub fn window_combo_box(cmd: *const ComboBoxCommand);
    pub fn window_color_picker(cmd: *const ColorPickerCommand);

    pub fn draw_rect(cmd: *const DrawRectCommand);

    pub fn draw_line(cmd: *const LineDrawCommand);

    pub fn draw_text(cmd: *const TextDrawCommand);
    //If you want to use spacings set them more than 0, if u dont set to neg values
    pub fn same_line(spacings: *const f32);
    pub fn begin_table(title: *const c_char, columns: u32) -> bool;
    pub fn table_next_row();
    pub fn table_set_column_index(column: u32);
    pub fn end_table();

    pub fn enable_click_through(state: bool);

    pub fn calculate_text_size(text: *const c_char) -> Vec2;

    pub fn get_monitor_height() -> u32;

    pub fn get_monitor_width() -> u32;
}
