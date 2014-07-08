TOP_DIR = ..

SRCS :=
SRCS += common_actions

SRCS += monitor
SRCS += widget
SRCS += child_widget
SRCS += workspace
SRCS += client
SRCS += action_set
SRCS += action_run
# SRCS += client_util
SRCS += application
# SRCS += shortcut_set
# SRCS += mouse_handler
SRCS += key_grab_handler
SRCS += key_binding_set
SRCS += frontend_theme

LIBNAME := frontend

CXXFLAGS += -I$(TOP_DIR)/common -I$(TOP_DIR)/frontend -I$(TOP_DIR)/frontend_modes

include $(TOP_DIR)/static_library.mk
