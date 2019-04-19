// Functions to build smaller, inline components

#include "screens.h"

lv_obj_t * draw_permission(lv_obj_t ** cont, uint32_t i, const char * desc, const char * remaining)
{
    lv_obj_t * perm = lv_cont_create((*cont), NULL);
    lv_obj_t * text = lv_label_create(perm, NULL);
    lv_label_set_text(text, desc);
    lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 15);

    // Usage bar (amount transacted)
    lv_obj_t * usageBar = lv_bar_create(perm, NULL);
    lv_bar_set_value(usageBar, 70);
    lv_obj_set_size(usageBar, 220, 20);
    lv_obj_align(usageBar, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 50);

    // Usage bar label
    lv_obj_t * usageLabel = lv_label_create(perm, NULL);
    lv_label_set_text(usageLabel, remaining);
    lv_obj_align(usageLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 270, 50);

    // Set the size and align
    lv_obj_set_size(perm, OUTER_W, 100);
    lv_obj_align(perm, (*cont), LV_ALIGN_IN_TOP_MID, 0, 70 + i * 100);
    
    return perm;
}