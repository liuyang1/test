#include <stdint.h>
#include <stdio.h>
#define ADTS_HEADER_SIZE 7
void qtmovie_packet_adts_frame_header(uint8_t object_type, uint8_t freq_index, uint8_t channel_info,uint16_t  frame_size)
{
    uint8_t frameheader[7] = {0};
    uint16_t framesize = frame_size;
    uint16_t totalsize = framesize + ADTS_HEADER_SIZE;

    frameheader[0] = 0xff;
    frameheader[1] = 0xf1;
    frameheader[2] = (char)(((object_type << 6) & 0xff) | ((freq_index << 2) & 0xff) | ((channel_info >> 2) & 0xff));
    frameheader[3] = (char)(((channel_info << 6) & 0xff) | ((totalsize >> 11) & 0xff));
    frameheader[4] = (char)((totalsize >> 3) & 0xff);
    frameheader[5] = (char)(((totalsize << 5) & 0xff) | 0x1F);
    frameheader[6] = 0xfc;

    printf("%02x %02x %02x %02x   %02x %02x %02x\n",
           frameheader[0], frameheader[1], frameheader[2], frameheader[3],
           frameheader[4], frameheader[5], frameheader[6]);
}

int main() {
    qtmovie_packet_adts_frame_header(2, 7, 2, 2047);
    return 0;

}
