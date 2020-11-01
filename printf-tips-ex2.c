#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

int main() {

    /* we are going to write the value 0xf8a4c4d7 (chosen randomly) into buffer[1] using only %n
     * 3 bytes of buffer[0] and 3 bytes of buffer[2] will be overwritten
     * we are going to use p as a pivot around memory allocated within buffer[]
     * dump is simply used because snprintf requires a destination buffer as argument
     */
    uint32_t buffer[3];
    void *p = buffer;
    uint8_t dump[1];

    // 0 the memory of buffer
    memset(buffer, 0, sizeof(buffer));

    /* first example
     * here we are going to write each byte in one call to snprintf
     * the value is stored in little endian format so the first byte of buffer[1] is the LSB
     */
    snprintf(dump, sizeof(dump), "%215d%n", 1, p + 4); // 0xd7 = 215
    snprintf(dump, sizeof(dump), "%196d%n", 1, p + 5); // 0xc4 = 196
    snprintf(dump, sizeof(dump), "%164d%n", 1, p + 6); // 0xa4 = 164
    snprintf(dump, sizeof(dump), "%248d%n", 1, p + 7); // 0xf8 = 248

    printf("after first example:  buffer[1] = 0x%x\n", buffer[1]);

    // 0 the memory of buffer again
    memset(buffer, 0, sizeof(buffer));

    /* second example
     * here we are going to write all 4 bytes in one single call to snprintf
     * we cannot use the same values for each byte as above
     */
    snprintf(dump, sizeof(dump), "%215d%n%237d%n%224d%n%84d%n",
        1, p + 4, // we again use 215
        1, p + 5, // we have already printed 215 ( 0xd7 ) characters so in order to write one byte of 0xc4, we need to
                  // print a total of 0x1c4. we calculate 0x1c4 - 0xd7 = 0xed = 237, so we need to print 237 bytes more
        1, p + 6, // 0x2a4 (LSB 0xa4) - 0x1c4 = 0xe0 = 224
        1, p + 7  // 0x2f8 (LSB 0xf8) - 0x2a4 = 0x54 = 84
    );

    printf("after second example: buffer[1] = 0x%x\n", buffer[1]);

    // 0 the memory of buffer again
    memset(buffer, 0, sizeof(buffer));

    /* third example
     * here we are going to follow the same structure as above, but instead use %hn twice
     * again we need to recalculate the values
     */

    snprintf(dump, sizeof(dump), "%50391d%hn%13261d%hn",
        1, p + 4, // 0xc4d7 = 50391
        1, p + 6  // 0xfba4 - 0xc4d7 = 0x33cd = 13261
    );

    printf("after third example:  buffer[1] = 0x%x\n", buffer[1]);

    // 0 the memory of buffer again
    memset(buffer, 0, sizeof(buffer));

    /* fourth example
     * here we are going to follow the same structure as above, but instead use %hhn four times
     * the values are calculated the same as in the second example, but now buffer[0] and buffer[2] are also 0
     */

    snprintf(dump, sizeof(dump), "%215d%hhn%237d%hhn%224d%hhn%84d%hhn",
        1, p + 4,
        1, p + 5,
        1, p + 6,
        1, p + 7
    );

    printf("after third example:  buffer[1] = 0x%x\n", buffer[1]);
    printf("additionally          buffer[0] = 0x%x\n", buffer[0]);
    printf("and                   buffer[2] = 0x%x\n", buffer[2]);

    return 0;
}