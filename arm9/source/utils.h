
#pragma once

#include <nds.h>
#include "common.h"

int hex2bytes(u8 *out, unsigned byte_len, const char *in);

const char * to_mebi(size_t size);

int save_file(const char *filename, const void *buffer, size_t size, int save_sha1);

int load_file(void **pbuf, size_t *psize, const char *filename, int verify_sha1, int align);

int load_block_from_file(void *buf, const char *filename, unsigned offset, unsigned size);

int save_sha1_file(const char *filename);

void print_bytes(const void *buf, size_t len);

size_t df(const char *path, int verbose);
