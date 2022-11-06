#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#define left(i) (i << 1) + 1
#define right(i) (i + 1) << 1
#define over(i) (i - 1) >> 1
#define ASCII (256)
#define CIRCULAR_BUFFER_CAPACITY (UINT16_MAX)
#define REFERENCE_THRESHOLD (6)

typedef struct heap_t heap_t;

typedef struct huff_node_t huff_node_t;

typedef struct huffman_t huffman_t;

typedef struct circular_buf_t circular_buf_t;

typedef struct lemp_ziv_t lemp_ziv_t;

typedef struct file_data_t file_data_t;

struct file_data_t {
    uint8_t     *data;
    size_t      data_len;
};

struct lemp_ziv_t {
    uint32_t     r_max;
    uint32_t     b_max;
    uint32_t     b_counter;
    uint32_t     r_counter;
    uint8_t      *bytes;
    uint16_t     *references;
};

struct circular_buf_t {
	uint8_t     *buffer;
	size_t      head;
	size_t      tail;
    size_t      max;
    bool        full;
};

struct heap_t {
    int 		len;
    int		 	capacity;
    huff_node_t *nodes[ASCII];
};

struct huff_node_t {
	uint8_t		c;
	int			freq;
	huff_node_t *right;
	huff_node_t *left;
};

struct huffman_t {
    uint8_t     *bytes;
	uint8_t 	*bitsets[ASCII];
	int 		bitset_tot;
	uint8_t		bitset_pos;
	int			*freq;
};

union u32_convertion {
    uint32_t    u32;
    uint8_t     u8[4];
} u32_convertion;

union u16_convertion {
    uint16_t    u16;
    uint8_t     u8[2];
} u16_convertion;

struct heap_t *malloc_heap(int capacity)
{
    struct heap_t   *heap;

    heap = (struct heap_t *)(malloc(sizeof(struct heap_t)));
    heap->capacity = capacity;
    heap->len = 0;
    return heap;
}

void heapify(struct heap_t *heap, int i) {
    int	                l, 
                        r, 
                        smallest;
	struct huff_node_t *node;

	l = left(i);
	r = right(i);


	if (l < heap->len && heap->nodes[l]->freq < heap->nodes[i]->freq) 
		smallest = l;
	else
		smallest = i;
	if (r < heap->len && heap->nodes[r]->freq < heap->nodes[smallest]->freq)
		smallest = r;

	if (smallest != i) {
		node = heap->nodes[i];
		heap->nodes[i] = heap->nodes[smallest];
		heap->nodes[smallest] = node;
		heapify(heap, smallest);
	}
}

void heap_push(struct heap_t *heap, struct huff_node_t *node) {
	int		i;

	heap->len++;
	i = heap->len - 1;
	while ((i > 0) && (heap->nodes[over(i)]->freq > node->freq)) {
		heap->nodes[i] = heap->nodes[over(i)];
		i = over(i);
	}
	heap->nodes[i] = node;
}

struct huff_node_t *heap_pull(struct heap_t *heap)
{
	struct huff_node_t 	*next;

	next = (struct huff_node_t *)
			(malloc(sizeof(struct huff_node_t*)));
	next = heap->nodes[0];
	heap->nodes[0] = heap->nodes[--heap->len];
	heapify(heap, 0);
	
	return next;
}

struct huff_node_t *malloc_huff_node(uint8_t c, int freq)
{
	struct huff_node_t  *res;

    res = (struct huff_node_t*)
            (malloc(sizeof(struct huff_node_t)));
	res->c = c;
	res->freq = freq;
	res->left = NULL;
	res->right = NULL;
	
	return res;
}

struct huff_node_t *malloc_huff_tree(struct huff_node_t *left,
										struct huff_node_t *right)
{
	struct huff_node_t  *res;
    
    res = (struct huff_node_t*)
			(malloc(sizeof(struct huff_node_t)));
	res->freq = left->freq + right->freq;
	res->left = left;
	res->right = right;

	return res;
}

struct huffman_t *malloc_huff()
{
    int         i;
	struct huffman_t *res;

    res = (struct huffman_t *)
			(malloc(sizeof(struct huffman_t)));
    res->bytes = (uint8_t *)(malloc(52428800));
	res->bitset_tot = 0;
	res->bitset_pos = 0;

	return res;
}

void create_huff_codes(struct huffman_t *huff, huff_node_t *node, uint8_t *code, int depth)
{	
	if ((node->left == NULL) && (node->right == NULL)) {
		*(code + depth) = 0;
		*(huff->bitsets + node->c) = strdup(code);
	} else {
        *(code + depth) = '0';
		create_huff_codes(huff, node->left, code, depth + 1);
        *(code + depth) = '1';
		create_huff_codes(huff, node->right, code, depth + 1);
	}
}

struct huff_node_t *construct_huff_tree(struct huffman_t *huff)
{
	struct huff_node_t 	*root,
                        *node,
                        *left,
                        *right;
	struct heap_t 		*prioq;
	uint8_t 			 i;

	prioq = malloc_heap(ASCII);

	for (i = 0; i < ASCII - 1; i++) {
		node = malloc_huff_node(i, *(huff->freq + i));
		heap_push(prioq, node);
	}
    node = malloc_huff_node(i, *(huff->freq + (uint8_t)255));
    heap_push(prioq, node);

	while (prioq->len > 1) {
		left = heap_pull(prioq);
		right = heap_pull(prioq);

		struct huff_node_t *tree = malloc_huff_tree(left, right);
		
		heap_push(prioq, tree);
	}

	root = heap_pull(prioq);

	free(prioq);
	return root;
}

int *find_freq(uint8_t *data, size_t data_len) {
	size_t 		j;
	int 		*frequency;

	frequency = (int *)(calloc(ASCII, sizeof(int)));

	for (j = 0; j < data_len; j++)
		(*(frequency + *(data + j)))++;

	return frequency;
}

struct lemp_ziv_t *malloc_lemp_ziv()
{
    struct lemp_ziv_t *res;

    res = (struct lemp_ziv_t *)
            (malloc(sizeof(struct lemp_ziv_t)));
    res->b_max = 52428800;
    res->b_counter = 0;
    res->r_max = 256;
    res->r_counter = 0;
    res->bytes = (uint8_t *)
                        (malloc(res->b_max));
    res->references = (uint16_t *)
                        (malloc(res->r_max * sizeof(uint16_t)));
}

void circular_buf_reset(struct circular_buf_t *cbuf)
{
    cbuf->head = 0;
    cbuf->tail = 0;
}

struct circular_buf_t *circular_buf_init(uint8_t* buffer)
{
	struct circular_buf_t   *cbuf;

    cbuf = (struct circular_buf_t *)
            malloc(sizeof(struct circular_buf_t));
	cbuf->buffer = buffer;
    cbuf->max = CIRCULAR_BUFFER_CAPACITY;
	circular_buf_reset(cbuf);

	return cbuf;
}

static void advance_pointer(struct circular_buf_t *cbuf)
{
	if(cbuf->full) {
		if (++(cbuf->tail) == cbuf->max) { 
            cbuf->tail = 0;
        }
	}
	if (++(cbuf->head) == cbuf->max)  { 
		cbuf->head = 0;
	}
    cbuf->full = (cbuf->head == cbuf->tail);
}

void circular_buf_put(struct circular_buf_t *cbuf, uint8_t data)
{
    *(cbuf->buffer + cbuf->head) = data;
    advance_pointer(cbuf);
}

int circular_buf_word_index(struct circular_buf_t *cbuf, uint8_t *data,
							uint16_t *buf_idx, uint16_t *seek_idx, size_t idx, size_t file_len)
{
    int         match_index,
                match_length,
                i,
                j;

    match_index = -1;
    match_length = REFERENCE_THRESHOLD;

    for (i = 0; i < cbuf->head; i++) {
        
        for (j = 0; j < UINT16_MAX; j++) {
            if (*(cbuf->buffer + j + i) != *(data + (idx + j)) 
                                || (idx + j) == file_len - 1)
                break;
        }

        if ((j - 1) > match_length) {
            match_index = i;
            match_length = (j - 1);
        }
    }
    if (match_index != -1) {
        *buf_idx = match_index;
        *seek_idx = match_length;
        return 0;
    } else {
        return match_index;
    }
}

struct lemp_ziv_t *lempel_ziv_encode(struct file_data_t *fd)
{
    size_t                  j, 
                            prev,
                            tmp_cap;
    uint16_t 	            buf_idx,
                            seek_idx,
                            tmp_len;
    uint8_t 	            *circ_buffer,
                            *tmp_buff;
    struct circular_buf_t   *cbuf;
    struct lemp_ziv_t       *lz;

    lz = malloc_lemp_ziv();

    circ_buffer =  (uint8_t *)(malloc(CIRCULAR_BUFFER_CAPACITY));
    cbuf = circular_buf_init(circ_buffer);

    tmp_buff =  (uint8_t *)(malloc(CIRCULAR_BUFFER_CAPACITY));
    tmp_cap = CIRCULAR_BUFFER_CAPACITY;

    prev = 0; j = 0; buf_idx = 0; seek_idx = 0; tmp_len = 0;

    while (j < fd->data_len) {

        if (circular_buf_word_index(cbuf, fd->data, &buf_idx,
                                     &seek_idx, j, fd->data_len) != -1) {

            if (lz->r_counter + 3 > lz->r_max - 1) {
                lz->r_max *= 2;
                lz->references = realloc(lz->references, 
                                            lz->r_max * sizeof(uint16_t));
            }

            *(lz->references + lz->r_counter++) = tmp_len;

            for (int i = 0; i < tmp_len; i++)
                *(lz->bytes + lz->b_counter++) = *(tmp_buff + i);

            *(lz->references + lz->r_counter++) = buf_idx;

            if (seek_idx > fd->data_len - j)
                seek_idx = fd->data_len - j;
                
            *(lz->references + lz->r_counter++) = seek_idx;

            for (int i = 0; i < seek_idx; i++) {
                circular_buf_put(cbuf, *(cbuf->buffer + buf_idx + i));
                j++;
            }

            tmp_len = 0;
            memset(tmp_buff, 0, CIRCULAR_BUFFER_CAPACITY);
            prev = j;

        } else {
            if (tmp_len == CIRCULAR_BUFFER_CAPACITY - 1) {

                if (lz->r_counter + 3 > lz->r_max - 1) {
                    lz->r_max *= 2;
                    lz->references = realloc(lz->references, lz->r_max * sizeof(uint16_t));
                }

                *(lz->references + lz->r_counter++) = tmp_len;

                for (int i = 0; i < tmp_len; i++)
                    *(lz->bytes + lz->b_counter++) = *(tmp_buff + i);

                buf_idx = 0;
                seek_idx = 0;
                tmp_len = 0;
                memset(tmp_buff, 0, CIRCULAR_BUFFER_CAPACITY);

                *(lz->references + lz->r_counter++) = buf_idx;

                if (seek_idx > fd->data_len - j)
                    seek_idx = fd->data_len - j;
                    
                *(lz->references + lz->r_counter++) = seek_idx;
            }

            circular_buf_put(cbuf, *(fd->data + j));
            *(tmp_buff + (tmp_len)) = *(fd->data + j);
            tmp_len++;
            j++;
        }
    }
    if (tmp_len > 0) {
        if (lz->r_counter == lz->r_max - 1) {
            lz->r_max++;
            lz->references = realloc(lz->references, lz->r_max * sizeof(uint16_t));
        }

        *(lz->references + lz->r_counter++) = tmp_len;

        for (int i = 0; i < tmp_len; i++)
            *(lz->bytes + lz->b_counter++) = *(fd->data + prev + i);
    }

    free(circ_buffer);
    free(cbuf);
    free(tmp_buff);

    return lz;
}

struct lemp_ziv_t *lempel_ziv_decode(struct file_data_t *fd)
{
    uint16_t 	            buf_idx,
                            seek_idx;
    uint8_t 	            *circ_buffer;
    size_t                  cur_pos;
	int 		            i;
    struct circular_buf_t   *cbuf;
    struct lemp_ziv_t       *lz,
                            *out;

    lz = malloc_lemp_ziv();

    out = malloc_lemp_ziv();

    cur_pos = 0;

    for (i = 0; i < 4; i++) {
        u32_convertion.u8[i] = *(fd->data + cur_pos++);
    }
    
    lz->r_max = u32_convertion.u32;

    lz->references = realloc(lz->references, lz->r_max * sizeof(uint16_t));

    for (i = 0; i < lz->r_max; i++) {
        for (int j = 0; j < 2; j++)
            u16_convertion.u8[j] = *(fd->data + cur_pos++);
        *(lz->references + i) = u16_convertion.u16;
    }

    for (i = 0; i < 4; i++) {
        u32_convertion.u8[i] = *(fd->data + cur_pos++);
    }
    
    lz->b_max = u32_convertion.u32;

    lz->bytes = realloc(lz->bytes, lz->b_max);

    for (int i = 0; i < lz->b_max; i++)
        *(lz->bytes + i) = *(fd->data + cur_pos++);

    circ_buffer = malloc(CIRCULAR_BUFFER_CAPACITY);
    cbuf = circular_buf_init(circ_buffer);

    buf_idx = 0; 
    seek_idx = 0;

    while (lz->b_counter < lz->b_max) {

        for (i = 0; i < *(lz->references + lz->r_counter); i++) {
            *(out->bytes + out->b_counter++) = *(lz->bytes + lz->b_counter);
            circular_buf_put(cbuf, *(lz->bytes + lz->b_counter++));
        }

        if (lz->b_counter == lz->b_max)
            break;

        lz->r_counter++;

        buf_idx = *(lz->references + lz->r_counter++);

        seek_idx = *(lz->references + lz->r_counter++);
        
        for (i = 0; i < seek_idx; i++) {
            circular_buf_put(cbuf, *(cbuf->buffer + buf_idx + i));
            *(out->bytes + out->b_counter++) = *(cbuf->buffer + buf_idx + i);
        }
    }

    free(lz->bytes);
    free(lz->references);
    free(lz);
    free(circ_buffer);
    free(cbuf);

    return out;
}

struct huffman_t *huff_encode(struct file_data_t *fd)
{
	uint8_t 			*s,
	                    code[ASCII],
                        cur_byte;
	size_t 				j,
                        i;
	struct huffman_t 	*huff;
	struct huff_node_t 	*root;

	huff = malloc_huff();

	huff->freq = find_freq(fd->data, fd->data_len);

	root = construct_huff_tree(huff);

	create_huff_codes(huff, root, code, 0);

	cur_byte = 0;

	for (j = 0; j < fd->data_len; j++) {
        for (s = *(huff->bitsets + *(fd->data + j)); *s; s++) {
            cur_byte <<= 1;

            if (*s == '1') 
                cur_byte |= 1;

            huff->bitset_pos++;

            if (huff->bitset_pos == 8) {
                *(huff->bytes + huff->bitset_tot) = cur_byte;
                huff->bitset_tot++;
                huff->bitset_pos = 0;
                cur_byte = 0;
            }
        }
    }

	while (huff->bitset_pos) {
		cur_byte <<= 1;

		huff->bitset_pos++;

		if (huff->bitset_pos == 8)
            *(huff->bytes + huff->bitset_tot++) = cur_byte;
	}

	free(root);

    return huff;
}

struct huffman_t *huff_decode(struct file_data_t *fd)
{
	uint8_t 			last_pos;
	size_t 				j,
                        i,
                        cur_pos;
	struct huffman_t 	*huff;
	struct huff_node_t 	*root,
                        *curr;

	huff = malloc_huff();

	huff->freq = (int *)(malloc(ASCII * sizeof(int)));

    cur_pos = 0;

    for (i = 0; i < ASCII; i++) {
        for (j = 0; j < 4; j++) {
            u32_convertion.u8[j] = *(fd->data + cur_pos++);
        }
        *(huff->freq + i) = u32_convertion.u32;
    }
    	
	root = construct_huff_tree(huff);

    uint8_t **bits = (uint8_t **)(malloc(fd->data_len * sizeof(uint8_t *)));
    for (i = 0; i < fd->data_len; i++)
        *(bits + i) = (uint8_t *)(calloc(8, 1));
		
    curr = root;

	for (j = cur_pos; j < fd->data_len; j++) {
		for (int i = 7; i >= 0; --i) {
			*(*(bits + j) + i) = *(fd->data + j) & (1 << i)  ? '1' : '0';
		}
	}

	j = cur_pos; i = 7;
	while (j < fd->data_len) {
		if (*(*(bits + j) + i) == '1')
			curr = curr->right;
		else
			curr = curr->left;

		if (curr->left == NULL && curr->right == NULL) {
            *(huff->bytes + huff->bitset_tot) = curr->c;
            huff->bitset_tot++;
			curr = root;
		}

		i--;
		if (i == -1) {
			i = 7;
			j++;
		}
	}

    for (i = cur_pos; i < fd->data_len; i++)
        free(*(bits + i));

    free(bits);
	free(root);

    return huff;
}

struct file_data_t *get_file_data(FILE *input)
{
    struct file_data_t *res = (struct file_data_t *)
                                (malloc(sizeof(struct file_data_t)));

    fseek(input, 0, SEEK_END);

    res->data_len = ftell(input);

    rewind(input);

    res->data = (uint8_t *)(malloc(res->data_len * sizeof(uint8_t)));

    fread(res->data, res->data_len, 1, input);

    return res;
}

int main(int argc, char **argv)
{
    FILE        *input,
                *output;
    
    if (argc != 4 || strcmp(*(argv + 1), *(argv + 2)) == 0) {
        fprintf(stderr, "commands: cmp\t|\tdcmp\t|\tlz-cmp\t|\tlz-dcmp\t|\thm-cmp\t|\thm-dcmp\n");
        fprintf(stderr, "usage: %s <path_to_input_file> <path_to_output_file> <command> \n", *(argv));
        fprintf(stderr, "example: %s file.txt file.lz lz-cmp\n", *(argv));
        return 1;
    }

    if (strcmp(*(argv + 3), "lz-cmp") == 0) {

        struct lemp_ziv_t   *lz;
        struct file_data_t  *fd;
        
        input = fopen(*(argv + 1), "rb");
        if (input == NULL) {
            fprintf(stderr, "The input file was not found.\n");
            return 1;
        }

        fd = get_file_data(input);

        fclose(input);

        lz = lempel_ziv_encode(fd);

        output = fopen(*(argv + 2), "wb");
        if (output == NULL) {
            fprintf(stderr, "The output file could not be created / was not found.\n");
            return 1;
        }

        fwrite(&lz->r_counter, 4, 1, output);

        for (int i = 0; i < lz->r_counter; i++)
            fwrite(lz->references + i, 2, 1, output);

        fwrite(&lz->b_counter, 4, 1, output);

        for (int i = 0; i < lz->b_counter; i++)
            fputc(*(lz->bytes + i), output);
        
        fclose(output);

        free(fd->data);
        free(fd);
        free(lz->bytes);
        free(lz->references);
        free(lz);

    } else if (strcmp(*(argv + 3), "lz-dcmp") == 0) {
        
        struct lemp_ziv_t   *lz;
        struct file_data_t  *fd;

        input = fopen(*(argv + 1), "rb");
        if (input == NULL) {
            fprintf(stderr, "The input file was not found.\n");
            return 1;
        }

        fd = get_file_data(input);

        fclose(input);

        lz = lempel_ziv_decode(fd);

        output = fopen(*(argv + 2), "wb");
        if (output == NULL) {
            fprintf(stderr, "The output file could not be created / was not found.\n");
            return 1;
        }

        for (int i = 0; i < lz->b_counter; i++)
            fputc(*(lz->bytes + i), output);

        fclose(output);

        free(lz->references);
        free(lz->bytes);
        free(lz);
        free(fd->data);
        free(fd);

    } else if (strcmp(*(argv + 3), "hm-cmp") == 0) {

        struct huffman_t    *huff;
        struct file_data_t  *fd;

        input = fopen(*(argv + 1), "rb");
        if (input == NULL) {
            fprintf(stderr, "The input file was not found.\n");
            return 1;
        }

        fd = get_file_data(input);

        fclose(input);

        huff = huff_encode(fd);

        output = fopen(*(argv + 2), "wb");
        if (output == NULL) {
            fprintf(stderr, "The output file could not be created / was not found.\n");
            return 1;
        }

        fwrite(huff->freq, 4, ASCII, output);

        for(int i = 0; i < huff->bitset_tot; i++)
            fputc(*(huff->bytes + i), output);

        fclose(output);
        free(huff->bytes);
        free(huff->freq);
        free(fd->data);
        free(fd);

    } else if (strcmp(*(argv + 3), "hm-dcmp") == 0) {

        struct huffman_t    *huff;
        struct file_data_t  *fd;

        input = fopen(*(argv + 1), "rb");
        if (input == NULL) {
            fprintf(stderr, "The input file was not found.\n");
            return 1;
        }

        fd = get_file_data(input);

        fclose(input);

        huff = huff_decode(fd);

        output = fopen(*(argv + 2), "wb");
        if (output == NULL) {
            fprintf(stderr, "The output file could not be created / was not found.\n");
            return 1;
        }

        fwrite(huff->bytes, 1, huff->bitset_tot, output);

        fclose(output);

        free(fd->data);
        free(fd);
        free(huff->bytes);
        free(huff->freq);
        free(huff);

    } else if (strcmp(*(argv + 3), "cmp") == 0) {

        struct lemp_ziv_t   *lz;
        struct huffman_t    *huff;
        struct file_data_t  *fd;
        size_t              cur_pos;

        input = fopen(*(argv + 1), "rb");
        if (input == NULL) {
            fprintf(stderr, "The input file was not found.\n");
            return 1;
        }

        fd = get_file_data(input);

        fclose(input);

        lz = lempel_ziv_encode(fd);

        free(fd->data);

        fd->data_len = (size_t)(4 + 4 + lz->r_counter * 2 + lz->b_counter);

        fd->data = (uint8_t *)(malloc(fd->data_len));

        cur_pos = 0;

        u32_convertion.u32 = lz->r_counter;

        for (int i = 0; i < 4; i++)
            *(fd->data + cur_pos++) = u32_convertion.u8[i];

        for (int i = 0; i < lz->r_counter; i++) {
            u16_convertion.u16 = *(lz->references + i);
            for (int j = 0; j < 2; j++)
                *(fd->data + cur_pos++) = u16_convertion.u8[j];
        }

        u32_convertion.u32 = lz->b_counter;

        for (int i = 0; i < 4; i++)
            *(fd->data + cur_pos++) = u32_convertion.u8[i];

        for (int i = 0; i < lz->b_counter; i++)
            *(fd->data + cur_pos++) = *(lz->bytes + i);

        free(lz->bytes);
        free(lz->references);
        free(lz);

        huff = huff_encode(fd);

        output = fopen(*(argv + 2), "wb");
        if (output == NULL) {
            fprintf(stderr, "The output file could not be created / was not found.\n");
            return 1;
        }

        for (int i = 0; i < ASCII; i++)
            fwrite(huff->freq + i, 4, 1, output);

        for(int i = 0; i < huff->bitset_tot; i++)
            fputc(*(huff->bytes + i), output);
        
        free(fd->data);
        free(fd);
        free(huff->bytes);
        free(huff->freq);
        free(huff);

        fclose(output);

    } else if (strcmp(*(argv + 3), "dcmp") == 0) {

        struct lemp_ziv_t   *lz;
        struct huffman_t    *huff;
        struct file_data_t  *fd;

        input = fopen(*(argv + 1), "rb");
        if (input == NULL) {
            fprintf(stderr, "The input file was not found.\n");
            return 1;
        }

        fd = get_file_data(input);

        fclose(input);

        huff = huff_decode(fd);

        fd->data = huff->bytes;
        fd->data_len = huff->bitset_tot;

        lz = lempel_ziv_decode(fd);

        printf("Bytes: %d\n", lz->b_counter);

        output = fopen(*(argv + 2), "wb");
        if (output == NULL) {
            fprintf(stderr, "The output file could not be created / was not found.\n");
            return 1;
        }

        for (int i = 0; i < lz->b_counter; i++)
            fputc(*(lz->bytes + i), output);

        fclose(output);

        free(huff->bytes);
        free(huff->freq);
        free(huff);
        free(lz->bytes);
        free(lz->references);
        free(lz);

    } else {
        fprintf(stderr, "commands: lz-compress\t|\thm-compress\t|\tlz-unpack\t|\thm-unpack\n");
        fprintf(stderr, "usage: %s <path_to_input_file> <path_to_output_file> <command> \n", *(argv));
        fprintf(stderr, "example: %s file.txt file.lz.bin lz-c\n", *(argv));
        return 1;
    }
    return 0;
}