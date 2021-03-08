/*
 * This is an implementation of wcwidth() and wcswidth() (defined in
 * IEEE Std 1002.1-2001) for Unicode.
 *
 * http://www.opengroup.org/onlinepubs/007904975/functions/wcwidth.html
 * http://www.opengroup.org/onlinepubs/007904975/functions/wcswidth.html
 *
 * Markus Kuhn -- 2007-05-26 (Unicode 5.0)
 *
 * Permission to use, copy, modify, and distribute this software
 * for any purpose and without fee is hereby granted. The author
 * disclaims all warranties with regard to this software.
 *
 * Latest version: http://www.cl.cam.ac.uk/~mgk25/ucs/wcwidth.c
 */

/*
 * Changes made for mutt:
 * - Adapted for Mutt by Edmund Grimley Evans.
 * - Changed 'first'/'last' members of combined[] to wchar_t from unsigned short to fix compiler warnings, 2007-11-13, Rocco Rutte
 */
#ifndef WCWIDTH_CJK_COMPAT_H_INCLUDED
#define WCWIDTH_CJK_COMPAT_H_INCLUDED

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wcwidth_cjk_option
{
	uint8_t is_apple = 0;
	uint8_t is_legacy_ja = 0;
	uint8_t is_emoji = 1;
} wcwidth_cjk_option;

extern const wcwidth_cjk_option default_wcwidth_cjk_option;
extern const wcwidth_cjk_option* default_wcwidth_cjk_option_ptr;

/*
 * The following two functions define the column width of an ISO 10646
 * character as follows:
 *
 *    - The null character (U+0000) has a column width of 0.
 *
 *    - Other C0/C1 control characters and DEL will lead to a return
 *      value of -1.
 *
 *    - Non-spacing and enclosing combining characters (general
 *      category code Mn or Me in the Unicode database) have a
 *      column width of 0.
 *
 *    - SOFT HYPHEN (U+00AD) has a column width of 1.
 *
 *    - Other format characters (general category code Cf in the Unicode
 *      database) and ZERO WIDTH SPACE (U+200B) have a column width of 0.
 *
 *    - Hangul Jamo medial vowels and final consonants (U+1160-U+11FF)
 *      have a column width of 0.
 *
 *    - Spacing characters in the East Asian Wide (W) or East Asian
 *      Full-width (F) category as defined in Unicode Technical
 *      Report #11 have a column width of 2.
 *
 *    - All remaining characters (including all printable
 *      ISO 8859-1 and WGL4 characters, Unicode control characters,
 *      etc.) have a column width of 1.
 *
 * This implementation assumes that wchar_t characters are encoded
 * in ISO 10646.
 */

int32_t wcwidth_ucs(uint32_t ucs, const wcwidth_cjk_option* option = default_wcwidth_cjk_option_ptr);
int32_t wcswidth_ucs(const uint32_t* pwcs, size_t n, const wcwidth_cjk_option* option = default_wcwidth_cjk_option_ptr);

/*
 * The following functions are the same as wcwidth_ucs() and
 * wcwidth_cjk(), except that spacing characters in the East Asian
 * Ambiguous (A) category as defined in Unicode Technical Report #11
 * have a column width of 2. This variant might be useful for users of
 * CJK legacy encodings who want to migrate to UCS without changing
 * the traditional terminal character-width behaviour. It is not
 * otherwise recommended for general use.
 */
/*
 * In addition to the explanation mentioned above,
 * several characters in the East Asian Narrow (Na) and Not East Asian
 * (Neutral) category as defined in Unicode Technical Report #11
 * actually have a column width of 2 in CJK legacy encodings.
 */

//#define wcwidth_cjk wcwidth
//#define wcswidth_cjk wcswidth

/* For FreeBSD: wcwidth() is implemented as an inline function */
//#ifdef wcwidth
	//#undef wcwidth
//#endif

int32_t wcwidth_cjk(uint32_t ucs, const wcwidth_cjk_option* option = default_wcwidth_cjk_option_ptr);
int32_t wcswidth_cjk(const uint32_t* pwcs, size_t n, const wcwidth_cjk_option* option = default_wcwidth_cjk_option_ptr);

#ifdef __cplusplus
}
#endif
#endif
