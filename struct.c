/*
 * struct.c
 *
 *  Created on: 2011. 5. 2.
 *      Author: Wonseok
 *
 * stolen from https://github.com/svperbeast/struct
 * and modify pack/unpack string(char[])
 */
#include "struct.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define ALIGNMENT 4
#define ALIGNMENT_MASK (ALIGNMENT - 1)
#define ROUNDUP(x) (((x) + ALIGNMENT_MASK) & ~ALIGNMENT_MASK)

enum endian {
    STRUCT_ENDIAN_NOT_SET,
    STRUCT_ENDIAN_BIG,
    STRUCT_ENDIAN_LITTLE
};

int get_endian(void)
{
    int i = 0x00000001;
    if (((char *)&i)[0]) {
        return STRUCT_ENDIAN_LITTLE;
    } else {
        return STRUCT_ENDIAN_BIG;
    }
}
static int myendian = STRUCT_ENDIAN_NOT_SET;

#define CHECK_PREREQUISITE() do {                   \
        if (myendian == STRUCT_ENDIAN_NOT_SET) {    \
            struct_init();                          \
        }                                           \
    } while (0)

#define CHECK_REPETITION(n, i, b) do {          \
        if ((i) > 0) {                          \
            errno = 0;                          \
            (n) = strtol((b), NULL, 10);        \
            if (errno != 0) {                   \
                break;                          \
            }                                   \
        } else {                                \
            (n) = 1;                            \
        }                                       \
        (i) = 0;                                \
        memset((b), 0, sizeof((b)));            \
    } while (0)

typedef union {
    float f;
    unsigned char b[sizeof(float)];
} fsplit_t;

typedef union {
    double d;
    unsigned char b[sizeof(double)];
} dsplit_t;


static void struct_init(void)
{
    myendian = get_endian();
}

static void pack_int16_t(unsigned char **bp, int16_t val, int endian)
{
    if (endian == myendian) {
        *((*bp)++) = val;
        *((*bp)++) = val >> 8;
    } else {
        *((*bp)++) = val >> 8;
        *((*bp)++) = val;
    }
}

static void pack_uint16_t(unsigned char **bp, uint16_t val, int endian)
{
    if (endian == myendian) {
        *((*bp)++) = val;
        *((*bp)++) = val >> 8;
    } else {
        *((*bp)++) = val >> 8;
        *((*bp)++) = val;
    }
}

static void pack_int32_t(unsigned char **bp, int32_t val, int endian)
{
    if (endian == myendian) {
        *((*bp)++) = val;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 24;
    } else {
        *((*bp)++) = val >> 24;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val;
    }
}

static void pack_uint32_t(unsigned char **bp, uint32_t val, int endian)
{
    if (endian == myendian) {
        *((*bp)++) = val;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 24;
    } else {
        *((*bp)++) = val >> 24;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val;
    }
}

static void pack_int64_t(unsigned char **bp, int64_t val, int endian)
{
    if (endian == myendian) {
        *((*bp)++) = val;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 24;
        *((*bp)++) = val >> 32;
        *((*bp)++) = val >> 40;
        *((*bp)++) = val >> 48;
        *((*bp)++) = val >> 56;
    } else {
        *((*bp)++) = val >> 56;
        *((*bp)++) = val >> 48;
        *((*bp)++) = val >> 40;
        *((*bp)++) = val >> 32;
        *((*bp)++) = val >> 24;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val;
    }
}

static void pack_uint64_t(unsigned char **bp, uint64_t val, int endian)
{
    if (endian == myendian) {
        *((*bp)++) = val;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 24;
        *((*bp)++) = val >> 32;
        *((*bp)++) = val >> 40;
        *((*bp)++) = val >> 48;
        *((*bp)++) = val >> 56;
    } else {
        *((*bp)++) = val >> 56;
        *((*bp)++) = val >> 48;
        *((*bp)++) = val >> 40;
        *((*bp)++) = val >> 32;
        *((*bp)++) = val >> 24;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val;
    }
}

/*
 * assume that sizeof(float) is 4.
 */
static void pack_float(unsigned char **bp, float val, int endian)
{
    fsplit_t u;

    u.f = val;

    if (endian == myendian) {
        *((*bp)++) = u.b[3];
        *((*bp)++) = u.b[2];
        *((*bp)++) = u.b[1];
        *((*bp)++) = u.b[0];
    } else {
        *((*bp)++) = u.b[0];
        *((*bp)++) = u.b[1];
        *((*bp)++) = u.b[2];
        *((*bp)++) = u.b[3];
    }
}

/*
 * assume that sizeof(double) is 8.
 */
static void pack_double(unsigned char **bp, double val, int endian)
{
    dsplit_t u;

    u.d = val;

    if (endian == myendian) {
        *((*bp)++) = u.b[7];
        *((*bp)++) = u.b[6];
        *((*bp)++) = u.b[5];
        *((*bp)++) = u.b[4];
        *((*bp)++) = u.b[3];
        *((*bp)++) = u.b[2];
        *((*bp)++) = u.b[1];
        *((*bp)++) = u.b[0];
    } else {
        *((*bp)++) = u.b[0];
        *((*bp)++) = u.b[1];
        *((*bp)++) = u.b[2];
        *((*bp)++) = u.b[3];
        *((*bp)++) = u.b[4];
        *((*bp)++) = u.b[5];
        *((*bp)++) = u.b[6];
        *((*bp)++) = u.b[7];
    }
}

/*
 * only packing the string of terminal '\0'
 */
static void pack_string(unsigned char **bp, char *str, int endian)
{
    int32_t len = strlen(str);
    int fill = 0, i;
    pack_int32_t(bp, len, endian);
    if (len) {
        strncpy((char *)*bp, str, len);
        *bp += len;
        fill = (int)ROUNDUP((int)*bp) - (int)*bp;
        for (i = 0; i < fill; i++)
            *((*bp)++) = 0;
    }
}

static void unpack_int16_t(unsigned char **bp, int16_t *dst, int endian)
{
    if (endian == myendian) {
        *dst = *((*bp)++);
        *dst |= *((*bp)++) << 8;
    } else {
        *dst = *((*bp)++) << 8;
        *dst |= *((*bp)++);
    }
}

static void unpack_uint16_t(unsigned char **bp, uint16_t *dst, int endian)
{
    if (endian == myendian) {
        *dst = *((*bp)++);
        *dst |= *((*bp)++) << 8;
    } else {
        *dst = *((*bp)++) << 8;
        *dst |= *((*bp)++);
    }
}

static void unpack_int32_t(unsigned char **bp, int32_t *dst, int endian)
{
    if (endian == myendian) {
        *dst = *((*bp)++);
        *dst |= *((*bp)++) << 8;
        *dst |= *((*bp)++) << 16;
        *dst |= *((*bp)++) << 24;
    } else {
        *dst = *((*bp)++) << 24;
        *dst |= *((*bp)++) << 16;
        *dst |= *((*bp)++) << 8;
        *dst |= *((*bp)++);
    }
}

static void unpack_uint32_t(unsigned char **bp, uint32_t *dst, int endian)
{
    if (endian == myendian) {
        *dst = *((*bp)++);
        *dst |= *((*bp)++) << 8;
        *dst |= *((*bp)++) << 16;
        *dst |= *((*bp)++) << 24;
    } else {
        *dst = *((*bp)++) << 24;
        *dst |= *((*bp)++) << 16;
        *dst |= *((*bp)++) << 8;
        *dst |= *((*bp)++);
    }
}

static void unpack_int64_t(unsigned char **bp, int64_t *dst, int endian)
{
    if (endian == myendian) {
        *dst = *((*bp)++);
        *dst |= (*((*bp)++) & 0xffLL) << 8;
        *dst |= (*((*bp)++) & 0xffLL) << 16;
        *dst |= (*((*bp)++) & 0xffLL) << 24;
        *dst |= (*((*bp)++) & 0xffLL) << 32;
        *dst |= (*((*bp)++) & 0xffLL) << 40;
        *dst |= (*((*bp)++) & 0xffLL) << 48;
        *dst |= (*((*bp)++) & 0xffLL) << 56;
    } else {
        *dst = (*((*bp)++) & 0xffLL) << 56;
        *dst |= (*((*bp)++) & 0xffLL) << 48;
        *dst |= (*((*bp)++) & 0xffLL) << 40;
        *dst |= (*((*bp)++) & 0xffLL) << 32;
        *dst |= (*((*bp)++) & 0xffLL) << 24;
        *dst |= (*((*bp)++) & 0xffLL) << 16;
        *dst |= (*((*bp)++) & 0xffLL) << 8;
        *dst |= *((*bp)++);
    }
}

static void unpack_uint64_t(unsigned char **bp, uint64_t *dst, int endian)
{
    if (endian == myendian) {
        *dst = *((*bp)++);
        *dst |= (*((*bp)++) & 0xffULL) << 8;
        *dst |= (*((*bp)++) & 0xffULL) << 16;
        *dst |= (*((*bp)++) & 0xffULL) << 24;
        *dst |= (*((*bp)++) & 0xffULL) << 32;
        *dst |= (*((*bp)++) & 0xffULL) << 40;
        *dst |= (*((*bp)++) & 0xffULL) << 48;
        *dst |= (*((*bp)++) & 0xffULL) << 56;
    } else {
        *dst = (*((*bp)++) & 0xffULL) << 56;
        *dst |= (*((*bp)++) & 0xffULL) << 48;
        *dst |= (*((*bp)++) & 0xffULL) << 40;
        *dst |= (*((*bp)++) & 0xffULL) << 32;
        *dst |= (*((*bp)++) & 0xffULL) << 24;
        *dst |= (*((*bp)++) & 0xffULL) << 16;
        *dst |= (*((*bp)++) & 0xffULL) << 8;
        *dst |= *((*bp)++);
    }
}

static void unpack_float(unsigned char **bp, float *dst, int endian)
{
    fsplit_t u;

    if (endian == myendian) {
        u.b[3] = *((*bp)++);
        u.b[2] = *((*bp)++);
        u.b[1] = *((*bp)++);
        u.b[0] = *((*bp)++);
    } else {
        u.b[0] = *((*bp)++);
        u.b[1] = *((*bp)++);
        u.b[2] = *((*bp)++);
        u.b[3] = *((*bp)++);
    }

    *dst = u.f;
}

static void unpack_double(unsigned char **bp, double *dst, int endian)
{
    dsplit_t u;

    if (endian == myendian) {
        u.b[7] = *((*bp)++);
        u.b[6] = *((*bp)++);
        u.b[5] = *((*bp)++);
        u.b[4] = *((*bp)++);
        u.b[3] = *((*bp)++);
        u.b[2] = *((*bp)++);
        u.b[1] = *((*bp)++);
        u.b[0] = *((*bp)++);
    } else {
        u.b[0] = *((*bp)++);
        u.b[1] = *((*bp)++);
        u.b[2] = *((*bp)++);
        u.b[3] = *((*bp)++);
        u.b[4] = *((*bp)++);
        u.b[5] = *((*bp)++);
        u.b[6] = *((*bp)++);
        u.b[7] = *((*bp)++);
    }

    *dst = u.d;
}

static void unpack_string(unsigned char **bp, char *str, int endian)
{
    int32_t len = 0;
    unpack_int32_t(bp, &len, endian);
    if (len)
    {
        strncpy(str, (char *)*bp, len);
        str[len] = '\0';
        *bp += len;
        *bp = (unsigned char *)ROUNDUP((int32_t)*bp);
    }
}
static int pack_va_list(unsigned char *buf, int offset, const char *fmt,
                        va_list args)
{
    const char *p;
    unsigned char *bp;
    int *ep = &myendian;
    int endian;
    char num_buf[32];
    int num_buf_idx;
    int num;
    int i;

    char b;
    unsigned char B;
    int16_t h;
    uint16_t H;
    int32_t l;
    uint32_t L;
    int64_t q;
    uint64_t Q;
    float f;
    double d;
    char *s;

    CHECK_PREREQUISITE();

    memset(num_buf, 0, sizeof(num_buf));
    num_buf_idx = 0;

    /*
     * 'char' and 'short' values, they must be extracted as 'int's,
     * because C promotes 'char' and 'short' arguments to 'int' when they are
     * represented by an ellipsis ... parameter.
     */

    bp = buf + offset;
    for (p = fmt; *p != '\0'; p++) {
        switch (*p) {
        case '=': /* native */
            ep = &myendian;
            break;
        case '<': /* little-endian */
            endian = STRUCT_ENDIAN_LITTLE;
            ep = &endian;
            break;
        case '>': /* big-endian */
            endian = STRUCT_ENDIAN_BIG;
            ep = &endian;
            break;
        case '!': /* network (= big-endian) */
            endian = STRUCT_ENDIAN_BIG;
            ep = &endian;
            break;
        case 'b':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                b = va_arg(args, int);
                *bp++ = b;
            }
            break;
        case 'B':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                B = va_arg(args, unsigned int);
                *bp++ = B;
            }
            break;
        case 'h':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                h = va_arg(args, int);
                pack_int16_t(&bp, h, *ep);
            }
            break;
        case 'H':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                H = va_arg(args, int);
                pack_uint16_t(&bp, H, *ep);
            }
            break;
        case 'i': /* fall through */
        case 'l':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                l = va_arg(args, int32_t);
                pack_int32_t(&bp, l, *ep);
            }
            break;
        case 'I': /* fall through */
        case 'L':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                L = va_arg(args, uint32_t);
                pack_uint32_t(&bp, L, *ep);
            }
            break;
        case 'q':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                q = va_arg(args, int64_t);
                pack_int64_t(&bp, q, *ep);
            }
            break;
        case 'Q':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                Q = va_arg(args, uint64_t);
                pack_uint64_t(&bp, Q, *ep);
            }
            break;
        case 'f':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                f = va_arg(args, double);
                pack_float(&bp, f, *ep);
            }
            break;
        case 'd':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                d = va_arg(args, double);
                pack_double(&bp, d, *ep);
            }
            break;
        case 's':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                s = va_arg(args, char*);
                pack_string(&bp, s, *ep);
            }
            break;
        default:
            if (isdigit((int)*p)) {
                num_buf[num_buf_idx++] = *p;
            } else {
                return -1;
            }
        }
    }
    return (bp - buf);
}

static int unpack_va_list(unsigned char *buf, int offset, const char *fmt,
                          va_list args)
{
    const char *p;
    unsigned char *bp;
    int *ep = &myendian;
    int endian;
    char num_buf[32];
    int num_buf_idx;
    int num;
    int i;

    char *b;
    unsigned char *B;
    int16_t *h;
    uint16_t *H;
    int32_t *l;
    uint32_t *L;
    int64_t *q;
    uint64_t *Q;
    float *f;
    double *d;
    char *s;

    CHECK_PREREQUISITE();

    memset(num_buf, 0, sizeof(num_buf));
    num_buf_idx = 0;

    bp = buf + offset;
    for (p = fmt; *p != '\0'; p++) {
        switch (*p) {
        case '=': /* native */
            ep = &myendian;
            break;
        case '<': /* little-endian */
            endian = STRUCT_ENDIAN_LITTLE;
            ep = &endian;
            break;
        case '>': /* big-endian */
            endian = STRUCT_ENDIAN_BIG;
            ep = &endian;
            break;
        case '!': /* network (= big-endian) */
            endian = STRUCT_ENDIAN_BIG;
            ep = &endian;
            break;
        case 'b':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                b = va_arg(args, char*);
                *b = *bp++;
            }
            break;
        case 'B':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                B = va_arg(args, unsigned char*);
                *B = *bp++;
            }
            break;
        case 'h':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                h = va_arg(args, int16_t*);
                unpack_int16_t(&bp, h, *ep);
            }
            break;
        case 'H':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                H = va_arg(args, uint16_t*);
                unpack_uint16_t(&bp, H, *ep);
            }
            break;
        case 'i': /* fall through */
        case 'l':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                l = va_arg(args, int32_t*);
                unpack_int32_t(&bp, l, *ep);
            }
            break;
        case 'I': /* fall through */
        case 'L':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                L = va_arg(args, uint32_t*);
                unpack_uint32_t(&bp, L, *ep);
            }
            break;
        case 'q':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                q = va_arg(args, int64_t*);
                unpack_int64_t(&bp, q, *ep);
            }
            break;
        case 'Q':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                Q = va_arg(args, uint64_t*);
                unpack_uint64_t(&bp, Q, *ep);
            }
            break;
        case 'f':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                f = va_arg(args, float*);
                unpack_float(&bp, f, *ep);
            }
            break;
        case 'd':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                d = va_arg(args, double*);
                unpack_double(&bp, d, *ep);
            }
            break;
        case 's':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            for (i = 0; i < num; i++) {
                s = va_arg(args, char*);
                unpack_string(&bp, s, *ep);
            }
            break;
        default:
            if (isdigit((int)*p)) {
                num_buf[num_buf_idx++] = *p;
            } else {
                return -1;
            }
        }
    }
    return (bp - buf);
}

/*
 * EXPORT
 *
 * preifx: struct_
 *
 */
int struct_pack(unsigned char *buf, const char *fmt, ...)
{
    va_list args;
    int packed_len = 0;

    va_start(args, fmt);
    packed_len = pack_va_list(buf, 0, fmt, args);
    va_end(args);

    return packed_len;
}

int struct_pack_into(int offset, unsigned char *buf, const char *fmt, ...)
{
    va_list args;
    int packed_len = 0;

    va_start(args, fmt);
    packed_len = pack_va_list(buf, offset, fmt, args);
    va_end(args);

    return packed_len;
}

int struct_unpack(unsigned char *buf, const char *fmt, ...)
{
    va_list args;
    int unpacked_len = 0;

    va_start(args, fmt);
    unpacked_len = unpack_va_list(buf, 0, fmt, args);
    va_end(args);

    return unpacked_len;
}

int struct_unpack_from(int offset, unsigned char *buf, const char *fmt, ...)
{
    va_list args;
    int unpacked_len = 0;

    va_start(args, fmt);
    unpacked_len = unpack_va_list(buf, offset, fmt, args);
    va_end(args);

    return unpacked_len;
}

int struct_calcsize(const char *fmt)
{
    int ret;
    const char *p;
    char num_buf[32];
    int num_buf_idx;
    int num;

    CHECK_PREREQUISITE();

    ret = 0;
    memset(num_buf, 0, sizeof(num_buf));
    num_buf_idx = 0;

    for (p = fmt; *p != '\0'; p++) {
        switch (*p) {
        case '=': /* fall through */
        case '<': /* fall through */
        case '>': /* fall through */
        case '!': /* ignore endian characters */
            break;
        case 'b':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(char));
            break;
        case 'B':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(unsigned char));
            break;
        case 'h':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(int16_t));
            break;
        case 'H':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(uint16_t));
            break;
        case 'i': /* fall through */
        case 'l':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(int32_t));
            break;
        case 'I': /* fall through */
        case 'L':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(uint32_t));
            break;
        case 'q':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(int64_t));
            break;
        case 'Q':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(uint64_t));
            break;
        case 'f':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(float));
            break;
        case 'd':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(double));
            break;
        case 's':
            CHECK_REPETITION(num, num_buf_idx, num_buf);
            ret += (num * sizeof(int32_t));
            break;
        default:
            if (isdigit((int)*p)) {
                num_buf[num_buf_idx++] = *p;
            } else {
                return -1;
            }
        }
    }
    return ret;
}

