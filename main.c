#include "struct.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char my_isprint(char c)
{
    if (c < 127 && c > 0 && isprint(c))
        return c;
    else
        return '.';
}
void print_hex(unsigned char *blob, int len)
{
    int offset = 0, i, j;
    len = len > 0 ? len : 0;
    if (len)
    {
        printf("OFFSET  HEX BYTES    ASCII\n");
        printf("------  -----------  -----\n");
        for (j = 0; j < len; j += 4)
        {
            printf(" %.04d   ", j / 4 * 4);
            for (i = j; i < j + 4 && i < len; i++)
                printf("%02x ", blob[i]);
            for (; i < j + 4; i++)
                printf("   ");
            printf(" ");
            for (i = j; i < j + 4 && i < len; i++)
                printf("%c", my_isprint(blob[i]));
            printf("\n");
        }
    }
}

int main()
{
    int len;
    char blob_pack[35];
    unsigned char buf[200];
    char *str1, *str2, *blob;
    strcpy(&blob_pack[8], "Xsoda");
    len = struct_pack(buf, "!2so", "Xsodaaaa", "Hello, World!", blob_pack, 21);
    print_hex(buf, len);
    struct_unpack(buf, "!2so", &str1, &str2, &blob, &len);
    printf("-------------------------------------------\n");
    printf("unpack 1: %s\n", str1);
    printf("unpack 2: %s\n", str2);
    printf("-------------------------------------------\n");
    print_hex((unsigned char *)blob, len);
    free(str1);
    free(str2);
    free(blob);
    system("pause");
    return 1;
}
