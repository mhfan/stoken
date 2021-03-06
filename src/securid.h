/*
 * securid.h - SecurID-related definitions
 *
 * Copyright 2012 Kevin Cernekee <cernekee@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __STOKEN_SECURID_H__
#define __STOKEN_SECURID_H__

#include "config.h"

#include <stdint.h>
#include <time.h>

#include "common.h"

#define AES_BLOCK_SIZE		16
#define AES_KEY_SIZE		16

#define MIN_PIN			4
#define MAX_PIN			8

#define MAX_PASS		40
#define MAGIC_LEN		6

#define VER_CHARS			1
#define SERIAL_CHARS		12

#define CHECKSUM_BITS		15
#define CHECKSUM_CHARS		(CHECKSUM_BITS / TOKEN_BITS_PER_CHAR)

#define BINENC_BITS		189
#define BINENC_CHARS		(BINENC_BITS / TOKEN_BITS_PER_CHAR)

#define BINENC_OFS		(VER_CHARS + SERIAL_CHARS)
#define CHECKSUM_OFS		(BINENC_OFS + BINENC_CHARS)

#define DEVID_CHARS		40

#define TOKEN_BITS_PER_CHAR	3
#define MIN_TOKEN_BITS		189
#define MAX_TOKEN_BITS		255
#define MAX_TOKEN_CHARS		(MAX_TOKEN_BITS / TOKEN_BITS_PER_CHAR)
#define MIN_TOKEN_CHARS		((MIN_TOKEN_BITS / TOKEN_BITS_PER_CHAR) + \
				 SERIAL_CHARS + VER_CHARS + CHECKSUM_CHARS)

#define BIT(x)			(1 << (x))

#define FL_128BIT		BIT(14)
#define FL_PASSPROT		BIT(13)
#define FL_SNPROT		BIT(12)
#define FL_FEAT3		BIT(11)
#define FL_FEAT4		BIT(10)
#define FL_FEAT5		BIT(9)
#define FLD_DIGIT_SHIFT		6
#define FLD_DIGIT_MASK		(0x07 << FLD_DIGIT_SHIFT)
#define FL_FEAT6		BIT(5)
#define FLD_PINMODE_SHIFT	3
#define FLD_PINMODE_MASK	(0x03 << FLD_PINMODE_SHIFT)
#define FLD_NUMSECONDS_SHIFT	0
#define FLD_NUMSECONDS_MASK	(0x03 << FLD_NUMSECONDS_SHIFT)

/* UNIX time_t for 2000/01/01 00:00:00 GMT */
#define SECURID_EPOCH		946684800

struct securid_token {
	char			serial[SERIAL_CHARS + 1];
	uint16_t		flags;
	uint16_t		exp_date;
	int			is_smartphone;

	int			has_enc_seed;
	uint8_t			enc_seed[AES_KEY_SIZE];

	uint16_t		dec_seed_hash;
	uint16_t		device_id_hash;

	int			has_dec_seed;
	uint8_t			dec_seed[AES_KEY_SIZE];

	int			pinmode;
	char			pin[MAX_PIN + 1];
	char			*enc_pin_str;
};

int securid_decode_token(const char *in, struct securid_token *t);
int securid_decrypt_seed(struct securid_token *t, const char *pass,
	const char *devid);
void securid_compute_tokencode(struct securid_token *t, time_t now,
	char *code_out);
void securid_token_info(const struct securid_token *t,
	void (*callback)(const char *key, const char *value));
int securid_encode_token(const struct securid_token *t, const char *pass,
	const char *devid, char *out);
int securid_random_token(struct securid_token *t);
int securid_check_exp(struct securid_token *t, time_t now);

char *securid_encrypt_pin(const char *pin, const char *password);
int securid_decrypt_pin(const char *enc_pin, const char *password, char *pin);

int securid_pin_format_ok(const char *pin);
int securid_pin_required(const struct securid_token *t);
int securid_pass_required(const struct securid_token *t);
int securid_devid_required(const struct securid_token *t);

#endif /* !__STOKEN_SECURID_H__ */
