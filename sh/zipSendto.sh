# 海力网关程序的压缩包脚本
# liuyang,2011-10-14 11:13:01
MYDATE=`date +%Y.%m.%d`
FILENAME=$1.$MYDATE.Sendto.zip

zip -r $FILENAME \
	./ACE-mips64/lib/libACE.so.5.5.7 \
	./Ice-3.4.2/lib/libIce.so.30 \
	./Ice-3.4.2/lib/libIceXML.so.30 \
	./Ice-3.4.2/lib/libIceUtil.so.30 \
	./Ice-3.4.2/lib/libIceStorm.so.30 \
	./expat-mips64/lib/libexpat.so.0 \
	./Agent/agent \
	./Agent/agent.conf \
	./Agent/run \
	./Agent/stop \
	./Xing-sendto/lib/* \
	./install.sh \
	./streaming.zip
echo $FILENAME
./ver.sh $FILENAME
