TARGET := libciai.a
ALL ?= $(wildcard *.c)
MEM:=BASE

ifeq ($(MEM), BASE)
$(info base mem component)
SRCS := $(filter-out memchk.c,$(ALL))
else ifeq ($(MEM), MEMCHK)
$(info memchk component)
SRCS := $(filter-out mem.c,$(ALL))
endif

$(info $(SRCS))

CFLAGS += -pg -Wno-unused-function

include ../template.mk
