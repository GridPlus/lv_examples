/* Defines a collection of screens using little vgl
 * */
#ifndef __GP_SCREENS_H
#define __GP_SCREENS_H

#include "gridplus_lvgl_theme.h"
#include "../lvgl.h"

//-------------------------
// CONSTANTS
//-------------------------
#define BOX_HEIGHT 704        // 0.88 * LV_VER_RES
#define TEXT_ENTRY_HEIGHT 60  // 0.075 * LV_VER_RES)
#define OUTER_W 480           // LV_HOR_RES
#define INNER_W 450           // LV_HOR_RES - 30

#define SEVEN_TEN_UP -560      // -0.7 * LV_VER_RES
#define SIX_TEN_UP -480       // -0.6 * LV_VER_RES
#define FIVE_TEN_UP -400
#define ONE_TEN_UP -80

#define EIGHT_TEN_TALL 640    // 0.8 * LV_VER_RES
#define SIX_TEN_TALL 480
#define TWO_TEN_TALL 160      // 0.2 * LV_VER_RES
#define ONE_TEN_TALL 80       // 0.1 * LV_VER_RES

#define EIGHT_TEN_WIDE 384     // 0.8* LV_HOR_RES

//-------------------------
// SYMBOLS
//-------------------------
typedef uint8_t * Icon_t;
static const Icon_t okIcon = SYMBOL_OK;
static const Icon_t closeIcon = SYMBOL_CLOSE;

//-------------------------
// SCREEN INDICES
//-------------------------
typedef enum __screen_indices {
    HOME_SCREEN_IDX         = 0,
    PAIRING_SCREEN_IDX      = 1,
    WALLETS_SCREEN_IDX      = 2,
    SETTINGS_SCREEN_IDX     = 3,
    MAKE_PAIRING_SCREEN_IDX = 4,
} ScreenIdx_t;

//-------------------------
// CUSTOM KEYBOARD MAPPINGS
// The originals are defined in `lv_kb.c`
//-------------------------
static const char * text_kb_map[] = {
    "0", "1", "2", "3", "4", "\n",
    "5", "6", "7", "8", "9", "\n",
    "A", "B", "C", "D", "E", "\n",
    "F", "G", "H", "J", "K", "\n",
    "L", "M", "N", "P", "Q", "\n",
    "\203Bksp",  "\203"SYMBOL_OK, ""
};

//-------------------------
// RE-USEABLE SCREEN COMPONENTS
// These elements are drawn on a screen.
// They are standardized building blocks.
//-------------------------

// The window is the root element of each screen.
// It contains a header title with a navigation button (usually)
// Buttons and text will be drawn inside of the window.
// A scroll bar is included by default.
static void window_create(lv_obj_t ** win, const char * title)
{    
    (*win) = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title((*win), title);
}

static void list_create(lv_obj_t ** win, lv_obj_t ** list)
{
    (*list) = lv_list_create((*win), NULL);    
    lv_obj_set_size((*list), INNER_W, EIGHT_TEN_TALL);
}

// Create a box inside of the main window
// This is a container that makes it possible to position sub-elements,
// which is very difficult with the main window
static void box_create(lv_obj_t ** window, lv_obj_t ** box)
{
    (*box) = lv_cont_create((*window), NULL);
    lv_obj_set_size((*box), OUTER_W, BOX_HEIGHT);
}
/*
static lv_obj_t* add_icon(lv_obj_t * bg, Icon_t icon)
{
    lv_obj_t * img_sym = lv_img_create(bg, NULL);
     // TODO try to get this colored green
    static lv_style_t style_sym;
    lv_style_copy(&style_sym, &lv_style_plain);
    style_sym.text.font = &lv_font_dejavu_40;
    lv_img_set_style(img_sym, &style_sym);
    lv_img_set_src(img_sym, icon);
    return img_sym;
}
*/

//-------------------------
// SCREEN DRAWS
//-------------------------
void draw_home(lv_obj_t ** win, lv_obj_t ** list);
void draw_pairings(lv_obj_t ** win, lv_obj_t ** list);
void draw_pairing_info(lv_obj_t ** win);
void draw_finish_pairing(lv_obj_t ** win, lv_obj_t ** ta, lv_obj_t ** kb);
lv_obj_t * draw_permission(lv_obj_t ** cont, uint32_t i, const char * desc, const char * remaining);


#endif // __GP_SCREENS_H