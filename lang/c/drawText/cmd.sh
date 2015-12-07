#! /usr/bin/env bash
str=""
str+=" !\"\#$\%\&'()*+,-./"
str+="0123456789:;<=>?@"
# str+="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# str+="[\\]^_\`"
# str+="abcdefghijklmnopqrstuvwxyz"
# str+="{|}~"

echo "$str"
height=16
width=16
len=${#str}
wwidth=$((len * width))
# font="Ubuntu-Mono"
font="Fixedsys-TTF-V5.00c"
xbm="font.xbm"

convert -resize "$wwidth"x"$height"! -font "$font" -pointsize 14 label:"$str" "$xbm"
{
    echo "#define font_width_per_char    $width";
    echo "// genereated by convert in ImageMagick with $font font, not change";
    echo "// liuyang1";
} >> "$xbm"
