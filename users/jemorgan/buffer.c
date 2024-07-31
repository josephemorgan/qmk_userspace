#include "buffer.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include "progmem.h"
#include "utilities.h"

struct buffer *const create_empty_buffer(void) {
    struct buffer *empty_buffer = (struct buffer *)malloc(sizeof(struct buffer));

    empty_buffer->head       = NULL;
    empty_buffer->tail       = NULL;
    empty_buffer->length     = 0;
    empty_buffer->max_length = 21;

    return empty_buffer;
}

struct buffer *const create_buffer_with_char(const char first_char) {
    struct node *first_node = (struct node *)malloc(sizeof(struct node));
    first_node->data        = first_char;
    first_node->next        = NULL;
    first_node->previous    = NULL;

    struct buffer *new_buffer = (struct buffer *)malloc(sizeof(struct buffer));
    new_buffer->head          = first_node;
    new_buffer->tail          = first_node;
    new_buffer->length        = 1;

    return new_buffer;
}

struct buffer *const add_char_to_buffer(struct buffer *buffer_to_modify, const char char_to_add) {
    if (buffer_to_modify->length == buffer_to_modify->max_length) {
        buffer_to_modify->tail = buffer_to_modify->tail->next;
        free(buffer_to_modify->tail->previous);
        buffer_to_modify->tail->previous = NULL;
        --buffer_to_modify->length;
    }

    struct node *node_to_add = (struct node *)malloc(sizeof(struct node));
    node_to_add->data        = char_to_add;

    if (!buffer_to_modify->head) {
        node_to_add->next      = NULL;
        node_to_add->previous  = NULL;
        buffer_to_modify->head = node_to_add;
        buffer_to_modify->tail = node_to_add;
    } else {
        node_to_add->next            = NULL;
        node_to_add->previous        = buffer_to_modify->head;
        buffer_to_modify->head->next = node_to_add;
        buffer_to_modify->head       = node_to_add;
    }
    ++buffer_to_modify->length;

    return buffer_to_modify;
}

struct buffer *const add_keycode_to_buffer(struct buffer *buffer_to_modify, const uint16_t keycode_to_add) {
    return add_char_to_buffer(buffer_to_modify, pgm_read_byte(&code_to_name[keycode_to_add]));
}

uint8_t get_buffer_as_string(const struct buffer *buffer_to_process, char *string_to_fill) {
    if (buffer_to_process->length) {
        struct node *current_node = buffer_to_process->tail;
        size_t       i            = 0;
        for (; i < buffer_to_process->length && current_node != NULL; ++i) {
            string_to_fill[i] = current_node->data;
            current_node      = current_node->next;
        }
        string_to_fill[i] = '\0';
        return i;
    }
    return 0;
}
