#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

int main(int argc, char **argv) {

    /* we are going to modify the value of n by writing to it via a %n specifier
     * p is a pointer that holds the address of n since %n requires an address
     * sbuff is just a stack buffer to increase the distance between rsp before sprintf is called
     * rsp is used to get the address of the rsp register before sprintf
     */
    int32_t n;
    int32_t *p;
    uint8_t sbuff[100];
    uint64_t rsp;

    // initialize n to 0 and p to address of n
    n = 0;
    p = &n;

    // using inline assembly get the value of rsp
    __asm__ __volatile__(
        ".intel_syntax noprefix;"
        "mov %0, rsp;"
        ".att_syntax;"
        : "=r" (rsp)
        :
    );

    printf("&p = %p\n", &p);
    printf("rsp = 0x%lx\n", rsp);
    printf("run: %s '12345%%%d$n'\nto set n equal to 5\n",
        argv[0],
        ((uint64_t)&p - rsp)    // calculate the distance between rsp and p on the stack
        / 8                     // divide by 8 (size of a register on amd64)
        + 6                     // standard calling convention places first 6 arguments in registers
        - 2                     // first two arguments to snprintf are not for specifiers
    );

    // call snprintf with argv[1] as format string supplied via the command line
    snprintf(sbuff, sizeof(sbuff), argv[1]);

    // print new value of n
    printf("n = %d\n", n);

    return 0;
}