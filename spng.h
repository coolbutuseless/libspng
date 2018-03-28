#ifndef SPNG_H
#define SPNG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#define SPNG_OK 0
#define SPNG_EINVAL 1
#define SPNG_EMEM 2
#define SPNG_EOVERFLOW 3
#define SPNG_ESIGNATURE 4
#define SPNG_EWIDTH 5
#define SPNG_EHEIGHT 6
#define SPNG_EUSER_WIDTH 7
#define SPNG_EUSER_HEIGHT 8
#define SPNG_EBIT_DEPTH 9
#define SPNG_ECOLOUR_TYPE 10
#define SPNG_ECOMPRESSION_METHOD 11
#define SPNG_EFILTER_METHOD 12
#define SPNG_EINTERLACE_METHOD 13
#define SPNG_EIHDR_SIZE 14
#define SPNG_ENOIHDR 15
#define SPNG_ECHUNK_POS 50
#define SPNG_ECHUNK_SIZE 51
#define SPNG_ECHUNK_CRC 52
#define SPNG_ECHUNK_TYPE 53
#define SPNG_ECHUNK_UNKNOWN_CRITICAL 54
#define SPNG_EDUP_PLTE 55
#define SPNG_EDUP_CHRM 56
#define SPNG_EDUP_GAMA 57
#define SPNG_EDUP_ICCP 58
#define SPNG_EDUP_SBIT 59
#define SPNG_EDUP_SRGB 60
#define SPNG_EDUP_BKGD 61
#define SPNG_EDUP_HIST 62
#define SPNG_EDUP_TRNS 63
#define SPNG_EDUP_PHYS 64
#define SPNG_EDUP_TIME 65
#define SPNG_ESBIT 66
#define SPNG_EPLTE_IDX 67
#define SPNG_ETRNS_COLOUR_TYPE 68
#define SPNG_EIDAT_TOO_SHORT 80
#define SPNG_EIDAT_TOO_LONG 81
#define SPNG_EDATA_AFTER_IEND 82
#define SPNG_EIDAT_STREAM 83
#define SPNG_EZLIB 84
#define SPNG_EFILTER 85
#define SPNG_EBKGD_NO_PLTE 100
#define SPNG_EBKGD_PLTE_IDX 101
#define SPNG_EBUFSIZ 120
#define SPNG_EIO 130
#define SPNG_EOF 131
#define SPNG_EBUF_SET 140
#define SPNG_EBADSTATE 141
#define SPNG_EFMT 142

#define SPNG_COLOUR_TYPE_GRAYSCALE 0
#define SPNG_COLOUR_TYPE_TRUECOLOR 2
#define SPNG_COLOUR_TYPE_INDEXED_COLOUR 3
#define SPNG_COLOUR_TYPE_GRAYSCALE_WITH_ALPHA 4
#define SPNG_COLOUR_TYPE_TRUECOLOR_WITH_ALPHA 6

#define SPNG_IO_EOF -1
#define SPNG_IO_ERROR -2

#define SPNG_FMT_PNG 1
#define SPNG_FMT_RGBA8 2
#define SPNG_FMT_RGBA16 3


struct spng_ihdr
{
    uint32_t width;
    uint32_t height;
    uint8_t bit_depth;
    uint8_t colour_type;
    uint8_t compression_method;
    uint8_t filter_method;
    uint8_t interlace_method;
};

struct spng_plte_entry
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct spng_trns_type2
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
};

struct spng_sbit_type2_3
{
    uint8_t red_bits;
    uint8_t green_bits;
    uint8_t blue_bits;
};

struct spng_sbit_type4
{
    uint8_t greyscale_bits;
    uint8_t alpha_bits;
};

struct spng_sbit_type6
{
    uint8_t red_bits;
    uint8_t green_bits;
    uint8_t blue_bits;
    uint8_t alpha_bits;
};

struct spng_bkgd_type2_6
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
};

struct spng_chrm
{
    uint32_t white_point_x;
    uint32_t white_point_y;
    uint32_t red_x;
    uint32_t red_y;
    uint32_t green_x;
    uint32_t green_y;
    uint32_t blue_x;
    uint32_t blue_y;
};

struct spng_time
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

struct spng_chunk
{
    size_t offset;
    uint32_t length;
    uint8_t type[4];
    uint32_t crc;
};

struct spng_decoder;

/* A read callback function should copy "n" bytes to *data and return 0 or
   spng_io_eof/spng_io_error on error. */
typedef int (*spng_read_fn)(struct spng_decoder *dec, void *user, void *data, size_t n);


struct spng_decoder
{
    size_t data_size;
    unsigned char *data;

    int valid_state;

    struct spng_chunk first_idat, last_idat;

    int8_t have_ihdr;
    struct spng_ihdr ihdr;

    int8_t have_plte;
    size_t plte_offset;
    uint32_t n_plte_entries;
    struct spng_plte_entry plte_entries[256];

    uint8_t have_chrm;
    struct spng_chrm chrm;

    uint8_t have_gama;
    uint32_t gama;

    uint8_t have_iccp;

    uint8_t have_sbit;
    union
    {
        uint8_t sbit_type0_greyscale_bits;
        struct spng_sbit_type2_3 sbit_type2_3;
        struct spng_sbit_type4 sbit_type4;
        struct spng_sbit_type6 sbit_type6;
    };

    uint8_t have_srgb;
    uint8_t srgb_rendering_intent;

    uint8_t have_bkgd;
    union
    {
        uint16_t bkgd_type0_4_greyscale;
        struct spng_bkgd_type2_6 bkgd_type2_6;
        uint8_t bkgd_type3_plte_index;
    };

    uint8_t have_hist;
    uint16_t hist_frequency[256];

    uint8_t have_trns;
    uint32_t n_trns_type3_entries;
    union
    {
        uint16_t trns_type0_grey_sample;
        struct spng_trns_type2 trns_type2;
        uint8_t trns_type3_alpha[256];
    };

    uint8_t have_phys;
    uint32_t phys_ppu_x, phys_ppu_y;
    uint32_t phys_unit_specifier;

    uint8_t have_time;
    struct spng_time time;
};


extern struct spng_decoder *spng_decoder_new(void);
extern void spng_decoder_free(struct spng_decoder *dec);

/* Sets a full input buffer for decoding. */
extern int spng_decoder_set_buffer(struct spng_decoder *dec, void *buf, size_t size);

/* Copies image information to *ihdr, requires an input buffer to be set. */
extern int spng_get_ihdr(struct spng_decoder *dec, struct spng_ihdr *ihdr);

/* Sets the value of *out to the required size, fmt must be one of SPNG_FMT_*
*/
extern int spng_get_output_image_size(struct spng_decoder *dec, int fmt, size_t *out);

extern int spng_decode_image(struct spng_decoder *dec, int fmt, unsigned char *out, size_t out_size, int flags);

#ifdef __cplusplus
}
#endif

#endif /* SPNG_H */