# mediaTool

## 32pulldown.py
demo 3:2 pulldown on 23.976 fps to 60 fps.

- first break 3:2 pulldown at 503 output isr.
- second break at 1001 output isr. after (1001-503) = 498 isr.

That means every 503/60=8.341 seconds or 498/60 = 8.3 seconds.

## ivf.py
IVF is simple container for VP8, VP9 format.
This tool use for parse IVF format.

    python ivf.py [ES filename]
