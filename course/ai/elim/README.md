# AI project for 'Kuaikuai Xiaochu Dashi'

It's 8x8 board.

User could fill random block(1x1, 1x2, 2x1, ... and other shapes) to board.

It will elimute when one columns or one rows is filled with block

- User get score N after filling n blocks
- User get score 10 after one elimute
- User get more 10 after combo or more rows/columns at one step

## code

- brd_open
- while (brd_fill(brd, shp_rand()))
    - brd_show(brd)
    - brd_elim(brd)
    - brd_show(brd)
- brd_close

## benchmark

random with seed [0, 1000)

- first_fit: 195.6
- corner: 166.8 <<< BAD
- center: 144.5 <<< BAD
- random: 68 <<< WORST

- score_first: 355.9
- score(greedy score, length): 384 (2022-09-28 best)

- max score+depth capability search
order 0: 384 (same with greedy score+length)
order 1: 456.8/463.0
- max score+fit counter search
order 1: 780.3 (2022-09-29 best)

max fit case:                               332.667
max fit case + max score:                   425.954
max fit case + max score with try-elim:     456.796
max capability:                             777.783
max capability + max score:                 777.783
max capability + max score with try-elim:   780.291
