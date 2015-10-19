envtest="third"
sndtest="more"
RET="default option"
ifeq ($(envtest), "first")
	RET="first option"
else ifeq ($(envtest), "second")
	RET="second option"
else
ifeq ($(envtest), "third")
	RET="third option"
ifeq ($(sndtest), "more")
	RETMORE="more return"
endif
endif
endif


main:
	@echo $(RET)
	@echo $(RETMORE)
