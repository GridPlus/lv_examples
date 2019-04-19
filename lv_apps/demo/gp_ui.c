#include "gridplus_lvgl_theme.h"
#include "../lvgl.h"
// #include "gp_debug_console.h"
#define DEBUG_PRINT_INFO printf

//-----------------------------------------
//DO NOT COPY ABOVE BLOCK
//-----------------------------------------


//=============================
// DEFINITIONS
//=============================

// Global attributes. These fix certain style attributes for components.
#define BOX_HEIGHT 704        // 0.88 * LV_VER_RES
#define TEXT_ENTRY_HEIGHT 60  // 0.075 * LV_VER_RES)
#define OUTER_W 480           // LV_HOR_RES
#define INNER_W 450           // LV_HOR_RES - 30

#define SEVEN_TEN_UP -560      // -0.7 * LV_VER_RES
#define SIX_TEN_UP -480       // -0.6 * LV_VER_RES
#define FIVE_TEN_UP -400

#define EIGHT_TEN_TALL 640    // 0.8 * LV_VER_RES
#define SIX_TEN_TALL 480
#define TWO_TEN_TALL 160      // 0.2 * LV_VER_RES
#define ONE_TEN_TALL 80       // 0.1 * LV_VER_RES

#define EIGHT_TEN_WIDE 384     // 0.8* LV_HOR_RES

// Symbols
typedef uint8_t * Icon_t;
static const Icon_t okIcon = SYMBOL_OK;
static const Icon_t closeIcon = SYMBOL_CLOSE;

// Screen indices
typedef enum __screen_indices {
    HOME_SCREEN_IDX         = 0,
    PAIRING_SCREEN_IDX      = 1,
    WALLETS_SCREEN_IDX      = 2,
    SETTINGS_SCREEN_IDX     = 3,
    MAKE_PAIRING_SCREEN_IDX = 4,
} ScreenIdx_t;

// Global objects. These should all be reused and/or referenced in 
// secondary functions. 
static lv_obj_t * box;
static lv_obj_t * win;
static lv_obj_t * list;
static lv_obj_t * ta;

// Component generators. These build components using global objects.
static void window_create(const char * title);
static void list_create(lv_obj_t * window);
static void box_create(lv_obj_t * window);

// Callback functions. These are triggered by events in screens.
static lv_res_t on_navigate_from_home(lv_obj_t * btn);
static lv_res_t on_navigate_from_pairings(lv_obj_t * btn);
static lv_res_t on_delete_pairing(lv_obj_t * btn);
static lv_res_t on_delete_permission(lv_obj_t * btn);
static lv_res_t on_try_pair(void);

// Button callbacks. These are of type lv_action_t and are associated with button presses
static lv_action_t action_ui_draw_home_screen();
static lv_action_t action_ui_draw_pairings_screen();

// Component additions. These return the component that is built.
static lv_obj_t* add_icon(lv_obj_t * bg, Icon_t icon);

// Custom keyboard mappings
// The originals are defined in `lv_kb.c`
static const char * text_kb_map[] = {
    "0", "1", "2", "3", "4", "\n",
    "5", "6", "7", "8", "9", "\n",
    "A", "B", "C", "D", "E", "\n",
    "F", "G", "H", "J", "K", "\n",
    "L", "M", "N", "P", "Q", "\n",
    "\203Bksp",  "\203"SYMBOL_OK, ""
};

// Button groups
static const char *const homeButtons[] = { "My Pairings", "My Wallets", "Settings", "[test] Pair" };
static const uint8_t numHomeButtons = sizeof(homeButtons) / sizeof(homeButtons[0]);

// For testing
// TODO: Add an integration to the filesystem
static const char *const pairingsButtons[] = { "GridPlus", "Netflix", "Spotify" };
static const uint8_t numPairingsButtons = sizeof(pairingsButtons) / sizeof(pairingsButtons[0]);

// Screen drawing functions. Calling one of these will redraw the screen.
static void ui_draw_home_screen(void);
static void ui_draw_pairings_screen(void);
static void ui_draw_pairing(uint32_t idx);
static void ui_draw_make_pairing(void);
static void ui_draw_result(bool success, uint8_t * text);

// Exported init function.
void ui_init(void) {
    // Set UI theme
    lv_theme_t * theme = lv_theme_gridplus_init(210, &lv_font_dejavu_20);
    lv_theme_set_current(theme);

    // Draw the homepage
    ui_draw_home_screen();
 }

//=============================
// SCREENS
// These are high-level screens composed of components.
// Each should be labeled with its depth from the home
// screen. It should also contain a navigation button
// in its header (except for the home screen)
//=============================

// Home screen (root)
void ui_draw_home_screen(void) {
    // Build the components
    window_create("             Lattice1 ");
    list_create(win);

    for (uint8_t i = 0; i < numHomeButtons; i++) {
        lv_obj_t * btn = lv_list_add(list, SYMBOL_RIGHT, homeButtons[i], on_navigate_from_home);
        lv_obj_set_free_num(btn, i+1);
        lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, on_navigate_from_home);
    }
}

// Pairings (depth=1)
void ui_draw_pairings_screen(void)
{
    window_create("Pairings");
    lv_win_add_btn(win, SYMBOL_LEFT, action_ui_draw_home_screen); 
    list_create(win);

    for (uint8_t i = 0; i < numPairingsButtons; i++) {
        lv_obj_t * btn = lv_list_add(list, SYMBOL_RIGHT, pairingsButtons[i], on_navigate_from_pairings);
        lv_obj_set_free_num(btn, i);
        lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, on_navigate_from_pairings);
    }
}

// Single pairing info (depth=2)
void ui_draw_pairing(uint32_t idx)
{
    // Draw the main window header
    window_create("Pairing Info");
    lv_win_add_btn(win, SYMBOL_LEFT, action_ui_draw_pairings_screen);

    lv_obj_t * subWin = lv_cont_create(win, NULL);
    lv_obj_set_size(subWin, OUTER_W, BOX_HEIGHT);

    // Use a label for the title
    lv_obj_t * pairingTitle = lv_label_create(subWin, NULL);
    lv_label_set_text(pairingTitle, pairingsButtons[idx]);
    static lv_style_t titleStyle;
    lv_style_copy(&titleStyle, &lv_style_plain);
    titleStyle.text.font = &sans_serif_40;
    lv_obj_set_style(pairingTitle, &titleStyle);
    lv_obj_align(pairingTitle, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);
    
    // Give the user an option to delete the pairing
    lv_obj_t * deletePairingBtn = lv_btn_create(subWin, NULL);
    lv_obj_t * closeLabel = lv_label_create(deletePairingBtn, NULL);
    lv_label_set_text(closeLabel, SYMBOL_TRASH);
    lv_obj_set_size(deletePairingBtn, 30, 30);
    lv_obj_align(deletePairingBtn, NULL, LV_ALIGN_IN_TOP_RIGHT, -30, 25);
    lv_obj_set_free_num(deletePairingBtn, idx);
    lv_btn_set_action(deletePairingBtn, LV_BTN_ACTION_CLICK, on_delete_pairing);

    // Draw the permission window
    lv_obj_t * explainer = lv_label_create(subWin, NULL);
    lv_label_set_text(explainer, "\nPermission statuses are shown below.");
    lv_obj_set_pos(explainer, 60, 70);

    // Draw the permissions data
    for (uint8_t i = 0; i < 3; i++) {
        lv_obj_t * perm = lv_cont_create(subWin, NULL);
        lv_obj_t * text = lv_label_create(perm, NULL);
        lv_label_set_text(text, "1.00 ETH per 7.00 days");
        lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 15);

        // Usage bar (amount transacted)
        lv_obj_t * usageBar = lv_bar_create(perm, NULL);
        lv_bar_set_value(usageBar, 70);
        lv_obj_set_size(usageBar, 220, 20);
        lv_obj_align(usageBar, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 50);

        // Usage bar label
        lv_obj_t * usageLabel = lv_label_create(perm, NULL);
        lv_label_set_text(usageLabel, "0.30 ETH left");
        lv_obj_align(usageLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 270, 50);
        
        // Add a delete button
        lv_obj_t * closeBtn = lv_btn_create(perm, NULL);
        lv_obj_t * closeLabel = lv_label_create(closeBtn, NULL);
        lv_label_set_text(closeLabel, SYMBOL_TRASH);
        lv_obj_set_size(closeBtn, 30, 30);
        lv_obj_align(closeBtn, NULL, LV_ALIGN_IN_TOP_RIGHT, 260, 15);
        lv_obj_set_free_num(closeBtn, i);
        lv_btn_set_action(closeBtn, LV_BTN_ACTION_CLICK, on_delete_permission);

        // Set the size and align
        lv_obj_set_size(perm, OUTER_W, 100);
        lv_obj_align(perm, subWin, LV_ALIGN_IN_TOP_MID, 0, 150 + i * 100);
    }

}

void ui_draw_make_pairing(void)
{
    window_create("Pair");
    lv_win_add_btn(win, SYMBOL_CLOSE, action_ui_draw_home_screen);

    // Create a box to hold the ta and kb components
    // It should take up the whole window
    box_create(win);

    // Create the keyboard with a custom mapping
    lv_obj_t *kb = lv_kb_create(box, NULL);
    lv_btnm_set_map(kb, text_kb_map);

    // Create the text area linked to the keyboard
    ta = lv_ta_create(box, NULL);
    lv_ta_set_text(ta, "\n");
    lv_ta_set_text_align(ta, LV_LABEL_ALIGN_CENTER);
    lv_ta_set_max_length(ta, 7);
    lv_kb_set_ta(kb, ta);

    // Set sizes of the components
    lv_obj_set_size(ta, EIGHT_TEN_WIDE, TEXT_ENTRY_HEIGHT);
    lv_obj_set_size(kb, OUTER_W, SIX_TEN_TALL);

    // Align the components
    lv_obj_align(ta, win, LV_ALIGN_OUT_BOTTOM_MID, 0, SEVEN_TEN_UP - 30);
    lv_obj_align(kb, win, LV_ALIGN_OUT_BOTTOM_LEFT, 0, SIX_TEN_UP);

    // Define callback for hitting "ok"
    lv_kb_set_ok_action(kb, on_try_pair);

    // Add a label
    lv_obj_t * desc = lv_label_create(box, NULL);
    lv_label_set_text(desc, "\nPlease enter the secret displayed\n        by your application.");
    lv_obj_align(desc, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
}

void ui_draw_result(bool success, uint8_t * text)
{
    const char * result = "Oops";
    if (success == true) result = "Success";
    window_create(result);
    // JUST FOR TESTING. This should eventually be an ephermeral screen displayed for ~1 sec
    lv_win_add_btn(win, SYMBOL_HOME, action_ui_draw_home_screen); 

    box_create(win);

    // Text box with description of what succeeded
    lv_obj_t * desc = lv_label_create(win, NULL);
    lv_label_set_text(desc, text);
    lv_obj_align(desc, NULL, LV_ALIGN_IN_TOP_MID, 0, 100);


    lv_obj_t * img_sym;
    if (success == true) img_sym = add_icon(box, okIcon);
    else                 img_sym = add_icon(box, closeIcon);
    lv_obj_align(img_sym, win, LV_ALIGN_IN_BOTTOM_MID, 0, SIX_TEN_UP);
}

//=============================
// SCREEN COMPONENTS
// These elements are drawn on a screen.
// They are standardized building blocks.
//=============================

// The window is the root element of each screen.
// It contains a header title with a navigation button (usually)
// Buttons and text will be drawn inside of the window.
// A scroll bar is included by default.
static void window_create(const char * title)
{    
    win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, title);
}

static void list_create(lv_obj_t * window)
{
    list = lv_list_create(window, NULL);    
    lv_obj_set_size(list, INNER_W, EIGHT_TEN_TALL);
}

// Create a box inside of the main window
// This is a container that makes it possible to position sub-elements,
// which is very difficult with the main window
static void box_create(lv_obj_t * window)
{
    box = lv_cont_create(window, NULL);
    lv_obj_set_size(box, OUTER_W, BOX_HEIGHT);
}

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


//=============================
// CALLBACKS
// These are actions triggered by interacting
// with screen components (usually buttons)
//======================== =====
static lv_res_t on_navigate_from_home(lv_obj_t * btn)
{
    DEBUG_PRINT_INFO("Leaving home page\n\r");
    ScreenIdx_t idx = lv_obj_get_free_num(btn);
    switch (idx) {
        case PAIRING_SCREEN_IDX:
            DEBUG_PRINT_INFO("Going to pairing list screen\n\r");
            ui_draw_pairings_screen();
            break;
        case WALLETS_SCREEN_IDX:
            DEBUG_PRINT_INFO("Going to wallets screen\n\r");
            break;
        case SETTINGS_SCREEN_IDX:
            DEBUG_PRINT_INFO("Going to settings screen\n\r");
            break;
        case MAKE_PAIRING_SCREEN_IDX:
            DEBUG_PRINT_INFO("Making pairing\n\r");
            ui_draw_make_pairing();
            break;
        default:
            break;
    }
    return LV_RES_OK;
}

static lv_res_t on_navigate_from_pairings(lv_obj_t * btn)
{
    DEBUG_PRINT_INFO("Leaving pairings screen\n\r");
    uint32_t idx = lv_obj_get_free_num(btn);
    ui_draw_pairing(idx);
}

static lv_res_t on_delete_pairing(lv_obj_t * btn)
{
    uint32_t idx = lv_obj_get_free_num(btn);
    DEBUG_PRINT_INFO("Deleting Pairing: %s \n\r", pairingsButtons[idx]);
}

static lv_res_t on_delete_permission(lv_obj_t * btn)
{
    // NOTE: Unfortunately, we can only assign one number to a button
    // We need access to both the current pairing and the current permission
    // in order to delete the permission.
    // We may be able to pass a memory address for the permission, which would
    // avoid the problem.
    uint32_t id = lv_obj_get_free_num(btn);
    DEBUG_PRINT_INFO("Delting permission: %d \n\r", id);
}


static lv_res_t on_try_pair(void)
{
    const char * secret = lv_ta_get_text(ta);
    DEBUG_PRINT_INFO("Trying pairing with secret: %s\n\r", secret);
    ui_draw_result(true, "\nSuccessfully made pairing.\nRedirecting you home.");
    // msTimerDelay(2000);
    // ui_draw_home_screen();
}


static lv_action_t action_ui_draw_home_screen()
{
    ui_draw_home_screen();
}

static lv_action_t action_ui_draw_pairings_screen()
{
    ui_draw_pairings_screen();
}