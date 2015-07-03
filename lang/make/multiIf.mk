envtest="third"
ifeq ($(envtest), "first")
	RET="first option"
else ifeq ($(envtest), "second")
	RET="second option"
else
ifeq ($(envtest), "third")
	RET="third option"
else
	RET="default option"
endif
endif


main:
	@echo $(RET)
