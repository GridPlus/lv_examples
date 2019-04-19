CSRCS += gp_ui.c
CSRCS += window_builders.c
CSRCS += component_builders.c

DEPPATH += --dep-path $(LVGL_DIR)/lv_examples/lv_apps/gp
VPATH += :$(LVGL_DIR)/lv_examples/lv_apps/gp
CFLAGS += "-I$(LVGL_DIR)/lv_examples/lv_apps/gp"