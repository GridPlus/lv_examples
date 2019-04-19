#include "gridplus_lvgl_theme.h"
#include "../lvgl.h"
#include "gp_ui.h"
#include "screens.h"
#define DEBUG_PRINT_INFO printf

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
// Task scheduler state
// static GpTaskId_t taskID;
static const uint32_t uiRefreshTime = 10; // 10ms

// Graphic Objects
lv_obj_t * win;
lv_obj_t * list;
lv_obj_t * ta;
lv_obj_t * kb;

// BUTTON GROUPS FOR TESTING
static const char *homeButtons[] = { "My Pairings", "[TEST] Finish Pairing" };
static const size_t numHomeButtons = sizeof(homeButtons) / sizeof(homeButtons[0]);
static const char *const pairingsButtons[] = { "GridPlus", "Netflix", "Spotify" };
static const uint8_t numPairingsButtons = sizeof(pairingsButtons) / sizeof(pairingsButtons[0]);

/*******************************************************************************
 * Callback Functions
 ******************************************************************************/
// static void ui_refresh_timer_callback(void) {
//     // Update UI time
//     lv_tick_inc(uiRefreshTime);

//     // Request execution time from the scheduler
//     GpTaskErr_t err = task_request(taskID);

//     // Fatal error if task_request() was not successful
//     if(err != GP_TASK_SUCCESS) {
//         DEBUG_PRINT_FATAL("Error calling task_request()\n\r");

//         // Halt and catch fire
//         assert(false);
//     }
// }

/*******************************************************************************
 * Helper Functions
 ******************************************************************************/
// Start a timer to trigger a UI refresh
// void restart_refresh_timer() {
//     // First see if we already have a timer instance running
//     if(get_timer_count(ui_refresh_timer_callback) > 0) {
//         // This probably shouldn't be happening...
//         DEBUG_PRINT_WARN("Repeated calls to start UI refresh timer.\n\r");

//         // Cancel the old timer(s)
//         stop_timers(ui_refresh_timer_callback);
//     }

//     // Create a timer
//     create_timer(uiRefreshTime, ui_refresh_timer_callback);
// }


/*******************************************************************************
 * UI Core State-machine
 ******************************************************************************/
// static void ui_core(void) {
//     // Render graphics updates
//     lv_task_handler();

//     // Set timer for next refresh
//     restart_refresh_timer();
// }


/*******************************************************************************
 * UI API
 ******************************************************************************/
void ui_init(void) {
    // Set UI theme
    lv_theme_t * theme = lv_theme_gridplus_init(210);
    lv_theme_set_current(theme);

    // Draw the homepage
    draw_home_action();

    // Register UI state-machine with task scheduler, run immediately
    // taskID = task_register(&ui_core, true);
}

//----------------------------
// Draw actions
//----------------------------
static lv_action_t draw_home_action() {
    draw_home(&win, &list);
    const char list_item_text[30];
    for (uint8_t i = 0; i < numHomeButtons; i++) {
        sprintf(list_item_text, "%s %s", SYMBOL_RIGHT, homeButtons[i]);
        lv_obj_t * btn = lv_list_add(list, NULL, list_item_text, on_navigate_from_home);

        lv_obj_set_free_num(btn, i);
        lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, on_navigate_from_home);
    }

}

static lv_action_t draw_pairings_action() {
    draw_pairings(&win, &list);
    const char list_item_text[30];
    const char window_button_label[3];
    sprintf(window_button_label, "%s", SYMBOL_LEFT);
    lv_win_add_btn(win, window_button_label, draw_home_action); 
    for (uint8_t i = 0; i < numPairingsButtons; i++) {
        sprintf(list_item_text, "%s %s", SYMBOL_RIGHT, pairingsButtons[i]);
        lv_obj_t * btn = lv_list_add(list, NULL, list_item_text, draw_pairing_info_action);
        lv_obj_set_free_num(btn, i);
        lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, draw_pairing_info_action);
    }
}

static lv_action_t draw_pairing_info_action(lv_obj_t * btn) {
    // Get the title of the permission
    uint32_t idx = lv_obj_get_free_num(btn);
    const char * title = pairingsButtons[idx];

    // Create the outer window as well as a sub window (used for easier styling)
    window_create(&win, title);
    lv_obj_t * cont = lv_cont_create(win, NULL);
    draw_pairing_info(&cont);
    lv_win_add_btn(win, SYMBOL_LEFT, draw_pairings_action); 

    // Give the user an option to delete the pairing
    lv_obj_t * deletePairingBtn = lv_btn_create(cont, NULL);
    lv_obj_t * closeLabel = lv_label_create(deletePairingBtn, NULL);
    lv_label_set_text(closeLabel, SYMBOL_TRASH);
    lv_obj_set_size(deletePairingBtn, 30, 30);
    lv_obj_align(deletePairingBtn, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, 25);
    lv_obj_set_free_num(deletePairingBtn, idx);
    lv_btn_set_action(deletePairingBtn, LV_BTN_ACTION_CLICK, on_delete_pairing);


    // Draw the permissions.
    // NOTE: THIS IS JUST DUMMY DATA FOR NOW
     // Draw the permissions data
    for (uint8_t i = 0; i < 3; i++) {
        lv_obj_t * perm = draw_permission(&cont, i, "1.00 ETH per 7.00 days", "30% left");
        // Add a delete button
        lv_obj_t * closeBtn = lv_btn_create(perm, NULL);
        lv_obj_t * closeLabel = lv_label_create(closeBtn, NULL);
        lv_label_set_text(closeLabel, SYMBOL_TRASH);
        lv_obj_set_size(closeBtn, 30, 30);
        lv_obj_align(closeBtn, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, 40);
        // Specify a number for a unique id of the permission:
        // [XXYY] - {XX = pairingIndex, YY = permissionIndex (within pairing) }
        uint32_t uniqueId = 1000 * (idx + 1) + i;
        lv_obj_set_free_num(closeBtn, uniqueId);
        lv_btn_set_action(closeBtn, LV_BTN_ACTION_CLICK, on_delete_permission);
    }
}

static lv_action_t draw_finish_pairing_action() {
    draw_finish_pairing(&win, &ta, &kb);
    lv_win_add_btn(win, SYMBOL_CLOSE, draw_home_action);
    lv_kb_set_ok_action(kb, on_try_pair);
}

//----------------------------
// Screen responses
//----------------------------

static lv_res_t on_navigate_from_home(lv_obj_t * btn) {
    DEBUG_PRINT_INFO("Leaving home page\n\r");
    uint8_t idx = lv_obj_get_free_num(btn);
    switch (idx) {
        case 0:
            DEBUG_PRINT_INFO("Going to pairing list screen\n\r");
            draw_pairings_action();
            break;
        case 1:
            draw_finish_pairing_action();
            break;
        default:
            break;
    }
    return LV_RES_OK;
}

static lv_res_t on_delete_pairing(lv_obj_t * btn)
{
    uint32_t idx = lv_obj_get_free_num(btn);
    DEBUG_PRINT_INFO("Deleting Pairing: %s \n\r", idx);
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

// Get the value from the provided text area and use it to validate
// that a pairing was successfully completed
static lv_res_t on_try_pair(void)
{
    const char * secret = lv_ta_get_text(ta);
    DEBUG_PRINT_INFO("Trying pairing with secret: %s\n\r", secret);
    // ui_draw_result(true, "\nSuccessfully made pairing.\nRedirecting you home.");
}