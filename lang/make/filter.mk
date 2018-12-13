SoC_Ver=BG2
SoC_Subversion=Q_A1
# SoC_Subversion=BA
a = $(filter $(SoC_Ver)$(SoC_Subversion), BG2Q_A1 BG2DTV_A0)
$(info a is $(a))

ifneq ($(a),)
$(info not include)
else
$(info include)
endif
