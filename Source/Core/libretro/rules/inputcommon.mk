INPUTCOMMON_DIR := $(BASE_DIR)/InputCommon
INPUTCOMMON_OBJECTS :=
INPUTCOMMON_OBJECTS += $(INPUTCOMMON_DIR)/ControllerEmu.o
INPUTCOMMON_OBJECTS += $(INPUTCOMMON_DIR)/InputConfig.o
INPUTCOMMON_OBJECTS += $(INPUTCOMMON_DIR)/ControllerInterface/ControllerInterface.o
INPUTCOMMON_OBJECTS += $(INPUTCOMMON_DIR)/ControllerInterface/Device.o
INPUTCOMMON_OBJECTS += $(INPUTCOMMON_DIR)/ControllerInterface/ExpressionParser.o
#INPUTCOMMON_OBJECTS += $(INPUTCOMMON_DIR)/ControllerInterface/Xlib/XInput2.o
INPUTCOMMON_OBJECTS += $(INPUTCOMMON_DIR)/GCAdapter.o
#INPUTCOMMON_OBJECTS += $(INPUTCOMMON_DIR)/ControllerInterface/evdev/evdev.o
#INPUTCOMMON_OBJECTS += $(INPUTCOMMON_DIR)/ControllerInterface/Pipes/Pipes.o
