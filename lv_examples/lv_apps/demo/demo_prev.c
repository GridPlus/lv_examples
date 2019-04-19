/**
 * @file demo.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "demo.h"
#include "gridplus_lvgl_theme.h"
#if USE_LV_DEMO

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void window_create();
static void list_create();
void home_page_create(void);
// void pairing_list_page_create(void);
// void pairing_info_page_create(uint8_t id);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * win;
static lv_obj_t * list;
static lv_obj_t * btn;
static lv_res_t on_navigate_from_home(lv_obj_t * btn);
// static lv_res_t on_select_pairing(lv_obj_t * btn);

// TEST SETUP
// const char *pairingsNames[] = { "App 1", "App 2" };
// uint8_t pairingsNamesLen = 2;

// Button groups
const char *homeButtons[] = { "My Pairings", "My Wallets", "Settings" };
uint8_t homeButtonsLen = 3;


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a demo application
 */
void demo_create(void)
{
    lv_theme_t * theme = lv_theme_gridplus_init(210, &lv_font_dejavu_20);
    lv_theme_set_current(theme);
    home_page_create();
 }

//=============================
// SCREENS
// These are high-level screens composed of components.
// Each should be labeled with its depth from the home
// screen. It should also contain a navigation button
// in its header (except for the home screen)
//=============================


//-----------------------------
// DEPTH = 0
//-----------------------------

// Home screen (root)
void home_page_create(void)
{
    // Build the components
    window_create();
    list_create();

    for (uint8_t i = 0; i < homeButtonsLen; i++) {
        lv_obj_t * btn = lv_list_add(list, SYMBOL_RIGHT, homeButtons[i], on_navigate_from_home);
        lv_obj_set_free_num(btn, i);
        lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, on_navigate_from_home);
    }
}

//-----------------------------
// DEPTH = 1
//-----------------------------

// List of pairings that have been made for the device
// (depth=1)
// void pairing_list_page_create(void)
// {
//     window_create();
//     lv_win_add_btn(win, SYMBOL_LEFT, home_page_create);
//     list_create();
//     for (uint8_t i = 0; i < pairingsNamesLen; i++) {
//         lv_obj_t * btn = lv_list_add(list, SYMBOL_RIGHT, pairingsNames[i], on_select_pairing);
//         lv_obj_set_free_num(btn, i);
//         lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, on_select_pairing);
//     }
// }

//-----------------------------
// DEPTH = 2
//-----------------------------

// In-depth look at the pairing.
// Includes its name, permissions, and other data
// (depth=2)
// void pairing_info_page_create(uint8_t id)
// {
//     printf("Drawing pairing\n");
//     window_create();
//     lv_win_add_btn(win, SYMBOL_LEFT, pairing_list_page_create);
//     lv_obj_t * txt = lv_label_create(win, NULL);
//     lv_label_set_text(txt, pairingsNames[id]);
// }


//=============================
// SCREEN COMPONENTS
// These elements are drawn on a screen.
// They are standardized building blocks.
//=============================

// The window is the root element of each screen.
// It contains a header title with a navigation button (usually)
// Buttons and text will be drawn inside of the window.
// A scroll bar is included by default.
static void window_create()
{    
    win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "                    Lattice1");
}

// static void list_create(func* func_arr, uint8_t n)
static void list_create()
{
    list = lv_list_create(win, NULL);
    lv_obj_set_size(list, LV_HOR_RES, 0.8*LV_VER_RES);
}

//=============================
// CALLBACKS
// These are actions triggered by interacting
// with screen components (usually buttons)
//=============================

// static lv_res_t on_select_pairing(lv_obj_t * btn)
// {
//     uint8_t id = lv_obj_get_free_num(btn);
//     printf("Buttoned pressed: %d\n", id);
//     // pairing_info_page_create(id);
// }


static lv_res_t on_navigate_from_home(lv_obj_t * btn)
{
    printf("Leaving home page\n");
    uint8_t idx = lv_obj_get_free_num(btn);
    switch (idx) {
        case 0:
            printf("Going to pairing list screen\n");
            // pairing_list_page_create();
            break;
        case 1:
            printf("Going to wallets screen\n");
            break;
        case 2:
            printf("Going to settings screen\n");
            break;
        default:
            break;
    }
    return LV_RES_OK;
}

#endif  /*USE_LV_DEMO*/
