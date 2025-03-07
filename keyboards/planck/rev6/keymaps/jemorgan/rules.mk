ifeq ($(strip $(AUDIO_ENABLE)), yes)
    SRC += muse.c
endif
LEADER_ENABLE = yes
EXTRAKEY_ENABLE = no # Audio control and System control
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
