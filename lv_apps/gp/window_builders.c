#include "screens.h"

void draw_home(lv_obj_t ** win, lv_obj_t ** list) 
{
    window_create(win, "Lattice1 Home");
    list_create(win, list);
}

void draw_pairings(lv_obj_t ** win, lv_obj_t ** list)
{
    window_create(win, "Pairings");
    list_create(win, list);
}

void draw_pairing_info(lv_obj_t ** cont)
{
    lv_obj_set_size((*cont), OUTER_W, BOX_HEIGHT);
    // Draw the permission window
    lv_obj_t * explainer = lv_label_create((*cont), NULL);
    lv_label_set_text(explainer, "\nAuto-spending limits:");
    lv_obj_set_pos(explainer, 30, 10);
}


void draw_finish_pairing(lv_obj_t ** win, lv_obj_t ** ta, lv_obj_t ** kb)
{
    window_create(win, "Finish Pairing");
    // lv_win_add_btn(win, SYMBOL_CLOSE, draw_home_action);

    // Create a box to hold the ta and kb components
    // It should take up the whole window
    lv_obj_t *box;
    box_create(win, &box);

    // Create the keyboard with a custom mapping
    (*kb) = lv_kb_create(box, NULL);
    lv_btnm_set_map((*kb), text_kb_map);

    // Create the text area linked to the keyboard
    (*ta) = lv_ta_create(box, NULL);
    lv_ta_set_text((*ta), "\n");
    lv_ta_set_text_align((*ta), LV_LABEL_ALIGN_CENTER);
    lv_ta_set_max_length((*ta), 7);
    lv_kb_set_ta((*kb), (*ta));

    // Set sizes of the components
    lv_obj_set_size((*ta), EIGHT_TEN_WIDE, TEXT_ENTRY_HEIGHT);
    lv_obj_set_size((*kb), OUTER_W, 5 * ONE_TEN_TALL);

    // Align the components
    lv_obj_align((*ta), (*win), LV_ALIGN_OUT_BOTTOM_MID, 0, SEVEN_TEN_UP - 30);
    lv_obj_align((*kb), (*win), LV_ALIGN_OUT_BOTTOM_LEFT, 0, ONE_TEN_UP * 6);

    // Add a label
    lv_obj_t * desc = lv_label_create(box, NULL);
    lv_label_set_text(desc, "\nPlease enter the secret displayed\n        by your application.");
    lv_obj_align(desc, NULL, LV_ALIGN_IN_TOP_MID, -20, 0);
}
