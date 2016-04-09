/* x &= x - 1
 * delete rightmost 1-bit
 * WHY?
 * x - 1 turn right part 0 to 1
 *     then &(AND) mask right part and rightmost 1-bit
 * For exmaple: 12 = 0b1100, 12 - 1 = 11 = 0b1011
 *     => 0b1100 & 0b1011 = 0b1000
 */
int bitcount(unsigned int x) {
    int cnt;
    for (cnt = 0; x != 0; x &= x - 1) {
        cnt++;
    }
    return cnt;
}
