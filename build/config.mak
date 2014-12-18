

CC = gcc
CFLAGS += -std=gnu99 -Wall
CFLAGS += $(addprefix -I, ${INC_DIR})

AR = ar
AR_FLAGS = cr

MKDIR_P = @mkdir -p

MOD_EXT = .a
