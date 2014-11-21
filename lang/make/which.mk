CCACHE := $(shell which ccache  2>/dev/null)
ifneq ($(CCACHE), )
all:
	echo "found" $(CCACHE)
else
all:
	echo "not found"
endif
