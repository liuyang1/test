envtest="test"
ifeq ($(envtest),)
p ?= $(envtest)
else
p ?= "else"
endif


all:
	echo $(envtest)
	echo $(p)
