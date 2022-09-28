for f in *.txt; do
	echo $f
	convert -size 360x360 xc:white \
		-font 'FreeMono' \
		-pointsize 20 \
		-draw "text 20,20 '$(cat $f)'" \
		-gravity NorthWest \
		$f.png
done

convert -delay 10 -loop 1 *.png 1.gif
