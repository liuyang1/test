PREFIX=prefix
# change SUFFIX value to make experiment
SUFFIX=suffix
#  SUFFIX=suffixmore
RET="cond default"

$(warning prefix=$(PREFIX) suffix=$(SUFFIX))
ifeq ($(PREFIX)$(SUFFIX), prefixsuffix)
	RET="cond 0"
endif

main:
	echo $(RET)
