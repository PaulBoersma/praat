/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2000-2009  Josh Coalson
 * Copyright (C) 2011-2025  Xiph.Org Foundation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the Xiph.org Foundation nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FLAC__PROTECTED__STREAM_DECODER_H
#define FLAC__PROTECTED__STREAM_DECODER_H

#include "flac_FLAC_stream_decoder.h"
#if FLAC__HAS_OGG
#include "flac_private_ogg_decoder_aspect.h"
#endif

typedef struct FLAC__StreamDecoderProtected {
	FLAC__StreamDecoderState state;
	FLAC__StreamDecoderInitStatus initstate;
	uint32_t channels;
	FLAC__ChannelAssignment channel_assignment;
	uint32_t bits_per_sample;
	uint32_t sample_rate; /* in Hz */
	uint32_t blocksize; /* in samples (per channel) */
	FLAC__bool md5_checking; /* if true, generate MD5 signature of decoded data and compare against signature in the STREAMINFO metadata block */
#if FLAC__HAS_OGG
	FLAC__OggDecoderAspect ogg_decoder_aspect;
#endif
} FLAC__StreamDecoderProtected;

/*
 * Return the number of input bytes consumed
 */
uint32_t FLAC__stream_decoder_get_input_bytes_unconsumed(const FLAC__StreamDecoder *decoder);

#endif
