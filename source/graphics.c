#include <SDL2/SDL_stdinc.h>
#include <common.h>
#include <graphics.h>
#include <ram.h>
#include <stddef.h>
#include <stdint.h>
#include <strings.h>

pixel_object graphics_buffer[EMULATOR_SCREEN_X][EMULATOR_SCREEN_Y];
pixel_object background_buffer[BACKGROUND_SCREEN_X][BACKGROUND_SCREEN_Y];
graphics_context graphics_ctx;
vram_tile_data_addr_mode tile_data_addr_mode = kADDR_MODE_8000;
rendering_periods current_period = kRENDER_WINDOW;
u8* framebuffer;
screen_coordinates scanline_pos;
ppu_mode current_ppu_mode = kMODE_NONE;
color_palette bg_color;

void assign_ids_to_bg_color()
{
  u8 value = read_uint8_data(BG_PALETTE_DATA_ADDR);
  bg_color.id_3.color_id = (BIT(value, 7) << 1) & (BIT(value, 6));
  bg_color.id_2.color_id = (BIT(value, 5) << 1) & (BIT(value, 4));
  bg_color.id_1.color_id = (BIT(value, 3) << 1) & (BIT(value, 2));
  bg_color.id_0.color_id = (BIT(value, 1) << 1) & (BIT(value, 0));
}

color_palette dma_palette_bit_to_color_palette(short dma_palette_bit)
{
  color_palette palette;
  u8 value;
  if (dma_palette_bit < 0 || dma_palette_bit > 1) {
    _ERROR(
        "dma_palette_bit_to_color_palette, invalid dma bit, check the argument "
        "given to this function in the creation of the obj!");
  }
  if (dma_palette_bit) {
    value = read_uint8_data(OBP1_PALETTE_DATA_ADDR);
  }
  else {
    value = read_uint8_data(OBP0_PALETTE_DATA_ADDR);
  }

  palette.id_3.color_id = (BIT(value, 7) << 1) & (BIT(value, 6));
  palette.id_2.color_id = (BIT(value, 5) << 1) & (BIT(value, 4));
  palette.id_1.color_id = (BIT(value, 3) << 1) & (BIT(value, 2));
  palette.id_0.color_id =
      0x00;  // obj the lower two bits are ignored, and index is always 0
  return palette;
}

bool verify_scanline_inside_window()
{
  return NULL;  // TODO
}

u16 get_window_tile_map_addr_from_lcdc_bit()
{
  return (get_lcdc_window_tile_map() ? TILE_MAP_ADDR_1 : TILE_MAP_ADDR_0);
}

u16 get_bg_tile_map_addr_from_lcdc_bit()
{
  return (get_lcdc_bg_tile_map() ? TILE_MAP_ADDR_1 : TILE_MAP_ADDR_0);
}

u8 get_lcdc()
{
  return read_uint8_data(LCDC_ADDR);
}

// LCD & PPU enable: 0 = Off; 1 = On
int get_lcdc_lcd_ppu_enable()
{
  return BIT(read_uint8_data(LCDC_ADDR), 7);
}
// Window tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
int get_lcdc_window_tile_map()
{
  return BIT(read_uint8_data(LCDC_ADDR), 6);
}
// Window enable: 0 = Off; 1 = On
int get_lcdc_window_enable()
{
  return BIT(read_uint8_data(LCDC_ADDR), 5);
}
// BG & Window tile data area: 0 = 8800–97FF; 1 = 8000–8FFF
int get_lcdc_bg_window_tiles()
{
  return BIT(read_uint8_data(LCDC_ADDR), 4);
}
// BG tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
int get_lcdc_bg_tile_map()
{
  return BIT(read_uint8_data(LCDC_ADDR), 3);
}
// OBJ size: 0 = 8×8; 1 = 8×16
int get_lcdc_obj_size()
{
  return BIT(read_uint8_data(LCDC_ADDR), 2);
}
// OBJ enable: 0 = Off; 1 = On
int get_lcdc_obj_enable()
{
  return BIT(read_uint8_data(LCDC_ADDR), 1);
}
// BG & Window enable / priority [Different meaning in CGB Mode]: 0 = Off; 1 =
// On
int get_lcdc_bg_window_priority()
{
  return BIT(read_uint8_data(LCDC_ADDR), 0);
}

screen_coordinates get_background_viewport_top_left()
{
  screen_coordinates coord;
  coord.x_coord = read_uint8_data(SCX_REGISTER_ADDR);
  coord.y_coord = read_uint8_data(SCY_REGISTER_ADDR);
  return coord;
}

screen_coordinates get_background_viewport_bottom_right()
{
  screen_coordinates coord;
  u8 x_pos = (read_uint8_data(SCX_REGISTER_ADDR) + 159) % 256;
  u8 y_pos = (read_uint8_data(SCY_REGISTER_ADDR) + 143) % 256;
  coord.x_coord = x_pos;
  coord.y_coord = y_pos;
  return coord;
}

background_viewport_coord get_background_viewport()
{
  background_viewport_coord bg;
  bg.top_left = get_background_viewport_top_left();
  bg.bottom_right = get_background_viewport_bottom_right();
  return bg;
}

screen_coordinates get_window_position_top_left_pos()
{
  screen_coordinates coord;
  coord.x_coord = read_uint8_data(WX_REGISTER_ADDR) + 7;
  coord.y_coord = read_uint8_data(WY_REGISTER_ADDR);
  return coord;
}

// TODO check if this is correct
screen_coordinates get_window_position_bottom_right_pos()
{
  screen_coordinates coord;
  coord.x_coord = read_uint8_data(WX_REGISTER_ADDR) + 7 + 166;
  coord.y_coord = read_uint8_data(WY_REGISTER_ADDR) + 7 + 143;
  return coord;
}

window_position get_window_position()
{
  window_position window;
  window.top_left = get_window_position_top_left_pos();
  window.bottom_right = get_window_position_bottom_right_pos();

  if (!verify_window_coordinates_range()) {
    _ERROR(
        "GB window position is out of range, verify window position have "
        "returned false! link to doc: "
        "https://gbdev.io/pandocs/"
        "Scrolling.html#ff4aff4b--wy-wx-window-y-position-x-position-plus-"
        "7");
  }

  return window;
}

bool verify_window_coordinates_range()
{
  u8 wx = read_uint8_data(WX_REGISTER_ADDR);
  u8 wy = read_uint8_data(WY_REGISTER_ADDR);
  return ((wx >= 0 && wx <= 166) && (wy >= 0 && wy <= 143));
}

void set_vram_tile_data_addr_mode(vram_tile_data_addr_mode mode)
{
  tile_data_addr_mode = mode;
}

// TODO verify if these bits are being read in the correct order
graphics_tile convert_memory_bytes_to_pixels(u8 memory[16])
{
  graphics_tile tile;
  int pixel_pos_x = 0;
  for (int x = 0; x < 8; x++) {
    int pixel_pos_y = 0;
    u8 lsb = memory[x * 2];
    u8 msb = memory[(x * 2) + 1];
    for (int y = 7; y >= 0; y--) {
      u8 bit1 = BIT(msb, y);
      u8 bit2 = BIT(lsb, y);
      u8 pixel = (bit1 << 1) & bit2;
      tile.pixels_colors[pixel_pos_x][pixel_pos_y].pixel_color = pixel;
      pixel_pos_y++;
    }
    pixel_pos_x++;
  }
  return tile;
}

graphics_tile tile_from_index(u8 index)
{
  graphics_tile tile;
  u8 memory[16];
  bool addr_mode_8000 = get_lcdc_bg_window_tiles();

  tile.index = index;

  if (addr_mode_8000) {
    for (int x = 0; x < 16; x++) {
      u16 pos = TILE_DATA_BLOCK0_START + index + x;
      memory[x] = read_uint8_data(pos);
    }
    tile = convert_memory_bytes_to_pixels(memory);
  }
  else {
    for (int x = 0; x < 16; x++) {
      u16 pos = TILE_DATA_BLOCK2_END - index - x;
      memory[x] = read_uint8_data(pos);
    }
    tile = convert_memory_bytes_to_pixels(memory);
  }

  return tile;
}

graphics_tile generate_default_tile(bool from_map_generator)
{
  if (!from_map_generator) {
    _ERROR("Generating default tile, something went wrong!");
  }
  graphics_tile tile;
  tile.index = 0;
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      tile.pixels_colors[x][y].pixel_color = PIXEL_COLOR_WHITE;
    }
  }
  return tile;
}

graphics_tile_map generate_default_map()
{
  _ERROR("Generating default map tile, something went wrong!");
  graphics_tile_map tile_map;
  for (int y = 0; y < TILE_MAP_SIZE; y++) {
    for (int x = 0; x < TILE_MAP_SIZE; x++) {
      tile_map.tiles[x][y] = generate_default_tile(true);
    }
  }
  return tile_map;
}

u16 convert_lcdc_tile_map_bit_to_addr(short lcdc_value)
{
  return (lcdc_value ? TILE_MAP_ADDR_1 : TILE_MAP_ADDR_0);
}

graphics_tile_map generate_map(u16 tile_map_addr)
{
  graphics_tile_map tile_map;

  if (tile_map_addr != TILE_MAP_ADDR_0 && tile_map_addr != TILE_MAP_ADDR_1) {
    _ERROR("Trying to generate tile map from invalid address: %u\n!",
           tile_map_addr);
    tile_map = generate_default_map();
    tile_map.type = kTILE_MAP_ERROR_TYPE;
    return tile_map;
  }

  u16 window_tile_map_addr =
      convert_lcdc_tile_map_bit_to_addr(get_lcdc_window_tile_map());
  u16 bg_tile_map_addr =
      convert_lcdc_tile_map_bit_to_addr(get_lcdc_bg_tile_map());

  if (tile_map_addr == window_tile_map_addr) {
    tile_map.type = kTILE_MAP_WINDOW_TYPE;
  }
  else if (tile_map_addr == bg_tile_map_addr) {
    tile_map.type = kTILE_MAP_BG_TYPE;
  }
  else {
    // TODO review this behavior
    tile_map.type = kTILE_MAP_BOTH_TYPE;
  }

  for (int x = 0; x < TILE_MAP_SIZE; x++) {
    for (int y = 0; y < TILE_MAP_SIZE; y++) {
      graphics_tile tile;
      u16 tile_index_addr = tile_map_addr + (x * y);
      u8 tile_index = read_uint8_data(tile_index_addr);
      tile = tile_from_index(tile_index);
      tile_map.tiles[x][y] = tile;
    }
  }
  return tile_map;
}

void generate_window_and_background(gb_window_context* window_ctx,
                                    gb_background_context* background_ctx)
{
  u16 window_tile_map_addr;
  if (get_lcdc_window_tile_map()) {
    window_tile_map_addr = TILE_MAP_ADDR_1;
  }
  else {
    window_tile_map_addr = TILE_MAP_ADDR_0;
  }
  u16 background_tile_map_addr;
  if (get_lcdc_bg_tile_map()) {
    background_tile_map_addr = TILE_MAP_ADDR_1;
  }
  else {
    background_tile_map_addr = TILE_MAP_ADDR_0;
  }
  window_ctx->tile_map = generate_map(window_tile_map_addr);
  window_ctx->position = get_window_position();
  background_ctx->tiles_map = generate_map(background_tile_map_addr);
  background_ctx->viewport = get_background_viewport();
}

void update_window_and_background_positions(
    gb_window_context* window_ctx, gb_background_context* background_ctx)
{
  window_ctx->position = get_window_position();
  background_ctx->viewport = get_background_viewport();
}

bool oam_flip_bit_to_bool(short bit)
{
  return bit ? true : false;
}

void regenerate_object_attrib(graphics_object* obj, u16 addr)
{
  for (int x = 0; x < 4; x++) {
    u16 attrib_addr = addr + x;
    u8 obj_byte = read_uint8_data(attrib_addr);
    obj->attributes.bytes[x] = obj_byte;
  }
}

void create_graphics_objects()
{
  u16 oam_size = OAM_ADDR_SIZE / 4;
  for (int x = 0; x < oam_size; x++) {
    graphics_object object;
    regenerate_object_attrib(&object, oam_size + 4 * x);
    object.attributes.oam_location = OAM_ADDR_START + (x * 4);
    u8 attrib = object.attributes.bytes[3];
    object.palette = dma_palette_bit_to_color_palette(BIT(attrib, 4));
    object.attributes.y_flip = oam_flip_bit_to_bool(BIT(attrib, 6));
    object.attributes.x_flip = oam_flip_bit_to_bool(BIT(attrib, 5));
    object.attributes.bg_priority = BIT(attrib, 7) ? false : true;
    object.coord.x_coord = object.attributes.bytes[1];
    object.coord.y_coord = object.attributes.bytes[0];
    object.tiles = get_object_tile_index(object);
    object.tiles.tile_top = tile_from_index(object.tiles.tile_top.index);
    object.tiles.tile_bottom = tile_from_index(object.tiles.tile_bottom.index);
    graphics_ctx.objects[x] = object;
  }
}

screen_coordinates get_object_pos(graphics_object object)
{
  screen_coordinates coord;
  coord.y_coord = object.attributes.bytes[0];
  coord.x_coord = object.attributes.bytes[1];
  return coord;
}

object_tile_setup get_object_tile_index(graphics_object object)
{
  object_tile_setup setup;
  u8 tile_index = object.attributes.bytes[2];
  if (get_lcdc_obj_size()) {
    setup.is_8_16 = true;
    setup.tile_top.index = (tile_index & 0xFE);
    setup.tile_bottom.index = (tile_index & 0x01);
  }
  else {
    setup.is_8_16 = false;
    setup.tile_top.index = tile_index;
    setup.tile_bottom.index = 0x00;
  }
  return setup;
}

u8 get_object_flags(graphics_object object)
{
  return object.attributes.bytes[3];
}

short get_object_flag_priority(graphics_object object)
{
  u8 obj_byte = object.attributes.bytes[3];
  return BIT(obj_byte, 7);
}

short get_object_flag_y_flip(graphics_object object)
{
  u8 obj_byte = object.attributes.bytes[3];
  return BIT(obj_byte, 6);
}

short get_object_flag_x_flip(graphics_object object)
{
  u8 obj_byte = object.attributes.bytes[3];
  return BIT(obj_byte, 5);
}

short get_object_flag_dmg_palette(graphics_object object)
{
  u8 obj_byte = object.attributes.bytes[3];
  return BIT(obj_byte, 4);
}

bool object_in_range_x(u8 line, graphics_object* obj)
{
  return (obj->coord.x_coord <= line && obj->coord.x_coord + 8 >= line);
}

bool object_in_range_y(u8 line, graphics_object* obj)
{
  if (get_lcdc_obj_size()) {
    return (obj->coord.y_coord <= line && obj->coord.y_coord + 16 >= line);
  }
  else {
    return (obj->coord.y_coord <= line && obj->coord.y_coord + 8 >= line);
  }
}

bool object_inside_object(graphics_object* obj1, graphics_object* obj2)
{
  screen_coordinates coord1 = obj1->coord;
  screen_coordinates coord2 = obj2->coord;
  int obj1_size_y = obj1->tiles.is_8_16 ? 16 : 8;
  int obj2_size_y = obj2->tiles.is_8_16 ? 16 : 8;
  bool collisionX = coord1.x_coord + 8 >= coord2.x_coord &&
                    coord2.x_coord + 8 >= coord1.x_coord;
  bool collisionY = coord1.y_coord + obj1_size_y >= coord2.y_coord &&
                    coord2.y_coord + obj2_size_y >= coord1.y_coord;
  return collisionX && collisionY;
}
// Check if the scanline overlap the object like
// ----------------------------------
// -----000000000-------------------- scanline overlap
// -----000000000--------------------

object_overlap_context verify_obj_overlap_scanline(graphics_object* obj,
                                                   u8 scanline)
{
  object_overlap_context overlap_ctx;
  size_t obj_size_lcdc = get_lcdc_obj_size() ? 16 : 8;
  size_t y_pos = obj->coord.y_coord;
  for (int x = 0; x < obj_size_lcdc; x++) {
    if ((y_pos + x) == scanline) {
      overlap_ctx.overlap = true;
      overlap_ctx.obj = obj;
      overlap_ctx.reimander_y = obj_size_lcdc - x;
      return overlap_ctx;
    }
  }
  overlap_ctx.overlap = false;
  return overlap_ctx;
}

void object_selection_priority()
{
  u16 oam_size = OAM_ADDR_SIZE / 4;
  u8 ly = read_uint8_data(LY_REGISTER_ADDR);
  int obj_selected_count = 0;
  for (int x = 0; x < oam_size; x++) {
    graphics_object* obj = &graphics_ctx.objects[x];

    if (object_in_range_y(ly, obj) && obj_selected_count <= 10) {
      graphics_ctx.selection_obj.objects[obj_selected_count] = *obj;
      obj_selected_count++;
    }

    graphics_ctx.selection_obj.size_selected = obj_selected_count;
  }
}

graphics_object* object_drawing_priority(graphics_object* obj1,
                                         graphics_object* obj2)
{
  if (obj1->coord.x_coord < obj2->coord.x_coord) {
    return obj1;
  }
  else if (obj2->coord.x_coord < obj1->coord.x_coord) {
    return obj2;
  }
  else {  // have the same x
    return (obj1->attributes.oam_location < obj2->attributes.oam_location)
               ? obj1
               : obj2;
  }
}

bool compare_ly_lcd()
{
  if (read_uint8_data(LY_REGISTER_ADDR) == read_uint8_data(LYC_REGISTER_ADDR)) {
    u8 value = read_uint8_data(LCD_STATUS_ADDR);
    BIT_SET(value, 2, 1);
    write_uint8_data(LCD_STATUS_ADDR, value);
    return true;
  }
  return false;
}

bool verify_ly_vblank()
{
  u8 ly = read_uint8_data(LY_REGISTER_ADDR);
  if (144 <= ly && 153 <= ly) {
    current_period = kVBLANK;
    return true;
  }
  return false;
}

void update_objects_coords()
{
  for (int x = 0; x < 10; x++) {
    graphics_object* obj = &graphics_ctx.selection_obj.objects[x];
    // TODO recheck if the oam_location of the obj is the correct addr to be argument to this func
    regenerate_object_attrib(obj, obj->attributes.oam_location);
    obj->coord.y_coord = obj->attributes.bytes[0];
    obj->coord.x_coord = obj->attributes.bytes[1];
  }
}
