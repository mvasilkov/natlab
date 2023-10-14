/* This file is part of natlab.
 * https://github.com/mvasilkov/natlab
 * MIT+Ethics License | Copyright (c) 2022, 2023 Mark Vasilkov
 * See https://github.com/mvasilkov/natlab/blob/master/LICENSE
 */
#include <stdio.h>

#include "thrust.c"

void print_vector(uint64_t start, int length)
{
    x = start | 1ULL;

    printf("  \"%llu\": [\n", start);
    while (length--)
    {
        printf("    \"%llu\",\n", next());
    }
    printf("  ],\n");
}

int main(void)
{
    printf("export default {\n");
    print_vector(1ULL, 10);
    print_vector(UINT8_MAX, 10);
    print_vector(UINT16_MAX, 10);
    print_vector(UINT32_MAX, 10);
    print_vector(UINT64_MAX, 10);
    printf("}\n");
}
