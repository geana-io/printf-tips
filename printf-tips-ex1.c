#include <stdio.h>
#include <stdlib.h>

int main() {

    /* we are going to test the %n, %hn and %hhn specifiers for the printf family of functions
     * n will be used to store the value from the specifier
     * dump is simply used because snprintf requires a destination buffer as argument
     */
    int n, max;
    char dump[1];

    // set n to 0
    n = 0;

    // maximum size for the %n specifier (searched manually)
    snprintf(dump, sizeof(dump), "%2147483614d%n\n", 1, &n);
    printf("0x%08x\n", n); // should print value different than 0

    /* set n to 0xffffffff and verify only last two bytes are changed
     * we will change the last two bytes to values 0xaabb = 43707 (which is an unsigned 16bit value)
     */
    n = 0xffffffff;
    snprintf(dump, sizeof(dump), "%43707d%hn\n", 1, &n);
    printf("0x%08x\n", n); // should print 0xffffaabb

    /* set n to 0xffffffff and verify only last byte is changed
     * we will change the last byte to value 0xaa = 170 (which is an unsigned 8bit value)
     */
    n = 0xffffffff;
    snprintf(dump, sizeof(dump), "%170d%hhn\n", 1, &n);
    printf("0x%08x\n", n); // should print 0xffffffaa

    return 0;
}