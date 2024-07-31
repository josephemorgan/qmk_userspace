#pragma once

#include <stdint.h>

struct node {
    char data;
    struct node *next;
    struct node *previous;
};

// A referennce to a doubly-linked-list of characters
struct buffer {
    struct node *head;
    struct node *tail;
    uint8_t length;
    uint8_t max_length;
};

// Returns an empty keyboard buffer
struct buffer * const create_empty_buffer(void);
// Returns a keyboard buffer containing a single character.
struct buffer * const create_buffer_with_char(const char);
struct buffer * const add_char_to_buffer(struct buffer*, const char);
struct buffer * const add_keycode_to_buffer(struct buffer*, uint16_t);
uint8_t get_buffer_as_string(const struct buffer*, char *);
void free_buffer(const struct buffer*);
