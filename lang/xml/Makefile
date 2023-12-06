product_type=box
feature_type=ddp dts

$(info $(product_type) $(feature_type))

BUILD_MS12=true

#ddp_=$(or $(findstring ddp,$(feature_type)),empty)
#ms12_=$(or $(findstring ms12,$(feature_type)),empty)
ddp_=$(if $(findstring true,$(BUILD_MS12)),ddp,empty)
dts_=dts
ac4_=ac4
iec_=iec
dev_ms12_=dev_ms12
# target:
# 	sed "s/DDP_OR_EMPTY/$(ddp_)/g" 1.xml > gen.xml
# 	sed -i "s/MS12_OR_EMPTY/$(ms12_)/g" gen.xml;
# 	xmllint -xinclude -format gen.xml > final.xml

DEVICE_AUDIO_DIR=device/amlogic/common/audio
#$(shell sed 's/DDP_OR_EMPTY/$(ddp_)/g;s/MS12_OR_EMPTY/$(ms12_)/g' 1.xml > gen.xml && xmllint -xinclude -format gen.xml > final.xml)
$(shell sed "s/DDP_OR_EMPTY/$(ddp_)/g" $(DEVICE_AUDIO_DIR)/tv.xml >gen.xml )
# NOTE: match dev_ms12 before MS12
$(shell sed -i "s/DEV_MS12_OR_EMPTY/$(dev_ms12_)/g" gen.xml)
$(shell sed -i "s/MS12_OR_EMPTY/$(dts_)/g" gen.xml)
$(shell sed -i "s/AC4_OR_EMPTY/$(ac4_)/g" gen.xml)
$(shell sed -i "s/DTS_OR_EMPTY/$(dts_)/g" gen.xml)
$(shell sed -i "s/IEC_OR_EMPTY/$(iec_)/g" gen.xml)
$(warning "test")
$(shell $(HOST_OUT_EXECUTABLES)/xmllint -xinclude -format gen.xml > final.xml)

target:
	echo empty
