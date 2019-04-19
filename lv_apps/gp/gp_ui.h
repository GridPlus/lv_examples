/**
 * @file demo.h
 *
 */

#ifndef DEMO_H
#define DEMO_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lvgl.h"
#include "lv_ex_conf.h"
#else
#include "../../../lvgl/lvgl.h"
#include "../../../lv_ex_conf.h"
#endif
#if USE_LV_DEMO

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a demo application
 */
void ui_init(void);

// SCREEN ACTIONS
static lv_res_t on_navigate_from_home(lv_obj_t * btn);

static lv_action_t draw_home_action();
static lv_action_t draw_pairings_action();
static lv_action_t draw_pairing_info_action(lv_obj_t * btn);
static lv_action_t draw_finish_pairing_action();

// SCREEN RESPONSES
static lv_res_t on_try_pair(void);
static lv_res_t on_delete_pairing(lv_obj_t * btn);
static lv_res_t on_delete_permission(lv_obj_t * btn);

/**********************
 *      MACROS
 **********************/

#endif /*USE_LV_DEMO*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DEMO_H*/
