#include "struct.h"
#include <stdio.h>
#include <stdint.h>
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
    int i, j;
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
    uint64_t magic;
    unsigned char buf[200] = {0};
    char *str1 = NULL, *str2 = NULL, *blob = NULL;
	for (len = 0; len < sizeof(blob_pack); len++)
		blob_pack[len] = 0xCD;
    strcpy(&blob_pack[8], "Xsoda");
    len = struct_pack(buf, "!Q2so", 0x58736F6461000000LL,"Xsodaaaa", "Hello, World!", blob_pack, 20);
    printf("pack success, length: %d\n", len);
    print_hex(buf, len);
    struct_unpack(buf, "!Q2so", &magic, &str1, &str2, &blob, &len);
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
