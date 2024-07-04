// Copyright 2018-2021 VMware, Inc.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "splinterdb/public_platform.h"

/*
 * A slice is just a const pointer with a length.  Slices do not
 * manage the memory to which they point, i.e. slices do not allocate,
 * free, realloc, or do anything else to the underlying memory
 * allocation.
 *
 * The best way to think of slices is: they're just const pointers.
 *
 * Slices are not disk resident.  They are used to pass-around
 * references to keys, values, etc, of different lengths in memory.
 *
 * Avoid accessing these fields directly.
 * Instead, use the slice_length and slice_data accessor functions.
 */
typedef struct slice {
   uint64      length;
   const void *data;
} slice;

#define NULL_SLICE    ((slice){.length = 0, .data = NULL})
#define INVALID_SLICE ((slice){.length = (uint64)-1, .data = NULL})

static inline _Bool
slice_is_null(const slice b)
{
   return b.length == 0 && b.data == NULL;
}

static inline slice
slice_create(uint64 len, const void *data)
{
   return (slice){.length = len, .data = data};
}

static inline uint64
slice_length(const slice b)
{
   return b.length;
}

static inline const void *
slice_data(const slice b)
{
   return b.data;
}

typedef enum perf_level {
  k_disable,
  k_enable
} perf_level;

typedef struct perf_context {
   // Get related stats
   uint64_t get_from_memtable_nanos;         // total nanos spent on querying memtables
   uint64_t filter_and_index_lookup_nanos;   // total nanos spent on looking up index/filter
   uint64_t cache_lookup_nanos;              // total nanos spent on looking up cache
   uint64_t io_read_nanos;                   // total nanos spent on io read
   uint64_t io_submit_nanos;                 // total nanos spent on io submit
   uint64_t io_poll_nanos;                   // total nanos spent on io poll

   // Put related stats
   uint64_t write_wal_nanos;                 // total nanos spent on writing to WAL
   uint64_t write_memtable_nanos;            // total nanos spent on writing to memtables
} perf_context;

static inline void
perf_context_reset(struct perf_context *ctx)
{
   // Reset get related stats
   ctx->get_from_memtable_nanos = 0;
   ctx->filter_and_index_lookup_nanos = 0;
   ctx->cache_lookup_nanos = 0;
   ctx->io_read_nanos = 0;
   ctx->io_submit_nanos = 0;
   ctx->io_poll_nanos = 0;

   // Rest put related  stats
   ctx->write_wal_nanos = 0;
   ctx->write_memtable_nanos = 0;
}

extern perf_context *
get_perf_context();

extern perf_level
get_perf_level();

extern void
set_perf_level(perf_level level);
