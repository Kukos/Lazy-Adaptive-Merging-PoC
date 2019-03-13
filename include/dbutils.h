#ifndef DBUTILS_H
#define DBUTILS_H

/*
    Common functions for DB Simulation

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL 3.0
*/


#include <common.h>
#include <compiler.h>

typedef enum query_t
{
    QUERY_RANDOM,
    QUERY_ALWAYS_NEW,
    QUERY_SEQUENTIAL_PATTERN,
} query_t;

#define INT_CEIL_DIV(n, k) (((n) + (k) - 1) / (k))

/* capacity calculator */
static ___inline___ size_t db_utils_entries_per_page(size_t page_size, size_t entry_size);
static ___inline___ size_t db_utils_entries_per_block(size_t block_size, size_t page_size, size_t entry_size);
static ___inline___ size_t db_utils_pages_for_entries(size_t page_size, size_t entry_size, size_t entries);
static ___inline___ size_t db_utils_blocks_for_entries(size_t block_size, size_t page_size, size_t entry_size, size_t entries);

static ___inline___ size_t db_utils_entries_per_page(size_t page_size, size_t entry_size)
{
    return page_size / entry_size;
}

static ___inline___ size_t db_utils_entries_per_block(size_t block_size, size_t page_size, size_t entry_size)
{
    const size_t pages = block_size / page_size;
    return db_utils_entries_per_page(page_size, entry_size) * pages;
}

static ___inline___ size_t db_utils_pages_for_entries(size_t page_size, size_t entry_size, size_t entries)
{
    return INT_CEIL_DIV(entries, db_utils_entries_per_page(page_size, entry_size));
}

static ___inline___ size_t db_utils_blocks_for_entries(size_t block_size, size_t page_size, size_t entry_size, size_t entries)
{
    return INT_CEIL_DIV(entries, db_utils_entries_per_block(block_size, page_size, entry_size));
}

#endif