TOP_DIR = ..

SRCS :=
SRCS += common_actions

SRCS += monitor
SRCS += widget
SRCS += child_widget
SRCS += workspace
# SRCS += container
SRCS += client
# SRCS += container_container
# SRCS += client_container
SRCS += action_set
SRCS += action_run
# SRCS += container_container_layout
# SRCS += client_container_layout
# SRCS += client_util
SRCS += application
# SRCS += container_util
# SRCS += shortcut_set
# SRCS += mouse_handler
SRCS += key_grab_handler
SRCS += key_binding_set

LIBNAME := frontend

CXXFLAGS += -I$(TOP_DIR)/common -I$(TOP_DIR)/frontend -I$(TOP_DIR)/frontend_modes

include $(TOP_DIR)/static_library.mk
