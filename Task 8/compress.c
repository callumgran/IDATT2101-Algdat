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
#define MAX_BITS (UINT16_MAX)
#define CIRCULAR_BUFFER_CAPACITY (UINT16_MAX)
#define REFERENCE_THRESHOLD (5)

typedef struct heap_t heap_t;

typedef struct huff_node_t huff_node_t;

typedef struct huffman_t huffman_t;

typedef struct circular_buf_t circular_buf_t;

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
	uint8_t 	*bitsets[ASCII];
	int 		bitset_tot;
	uint8_t		bitset_pos;
	int			*freq;
};

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
	res->bitset_tot = 1;
	res->bitset_pos = 0;

	return res;
}

void create_huff_codes(struct huffman_t *huff, huff_node_t *node, uint8_t *code, int depth)
{	
	if ((node->left == NULL) && (node->right == NULL)) {
		code[depth] = 0;
		huff->bitsets[node->c] = strdup(code);
	} else {
		code[depth] = '0';
		create_huff_codes(huff, node->left, code, depth + 1);
		code[depth] = '1';
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

int *find_freq(uint8_t *file_buffer, size_t file_len) {
	size_t 		j;
	int 		*frequency;

	frequency = (int *)(malloc(ASCII * sizeof(int)));

	for (j = 0; j < file_len; j++)
		frequency[file_buffer[j]]++;

	return frequency;
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
    cbuf->buffer[cbuf->head] = data;
    advance_pointer(cbuf);
}

int circular_buf_contains(struct circular_buf_t *cbuf, uint8_t data)
{
    int i;
    for (i = cbuf->tail; i < cbuf->head; i++)
        if (cbuf->buffer[i] == data)
            return i;
    return -1;
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
        j;
        for (j = 0; j < cbuf->head; j++) {
            if (*(cbuf->buffer + j + i) != *(data + (idx + j)) || (idx + j) == file_len - 1) {
                break;
            }
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

void lempel_ziv_encode(char *in_file_name, char *out_file_name)
{
    FILE 		            *input_file, 
				            *output_file;
    size_t 		            file_len, 
                            j, 
                            prev;
    uint16_t 	            buf_idx, 
				            seek_idx,
				            next_reference;
    uint8_t 	            *file_buffer,
                            *circ_buffer,
                            *tmp_buff;
    struct circular_buf_t   *cbuf;

    input_file = fopen(in_file_name, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "The input file was not found.\n");
        return;
    }

    fseek(input_file, 0, SEEK_END);
    file_len = ftell(input_file); 
    rewind(input_file);
    file_buffer = (uint8_t *)(malloc(file_len * sizeof(uint8_t)));
    fread(file_buffer, file_len, 1, input_file);

    fclose(input_file);

    output_file = fopen(out_file_name, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "The output file could not be created / was not found.\n");
        free(file_buffer);
        return;
    }

    remove(in_file_name);

    circ_buffer = malloc(CIRCULAR_BUFFER_CAPACITY * sizeof(uint8_t));
    cbuf = circular_buf_init(circ_buffer);
    tmp_buff = malloc(CIRCULAR_BUFFER_CAPACITY * sizeof(uint8_t));

    next_reference = 0; prev = 0; j = 0; buf_idx = 0; seek_idx = 0;

    while (j < file_len) {
        if (circular_buf_word_index(cbuf, file_buffer, &buf_idx, &seek_idx, j, file_len) != -1) {
            next_reference = j - prev;
            fwrite(&next_reference, 2, 1, output_file);
            fwrite(tmp_buff, next_reference, 1, output_file);
            fwrite(&buf_idx, 2, 1, output_file);
            if (seek_idx > file_len - j)
                seek_idx = file_len - j;
            fwrite(&seek_idx, 2, 1, output_file);
            for (int i = 0; i < seek_idx; i++) {
                circular_buf_put(cbuf, *(cbuf->buffer + buf_idx + i));
                j++;
            }
            memset(tmp_buff, 0, CIRCULAR_BUFFER_CAPACITY);
            prev = j;
        } else {
            circular_buf_put(cbuf, *(file_buffer + j));
            *(tmp_buff + (j - prev)) = *(file_buffer + j);
            j++;
        }
    }
    if (j - prev > 0) {
        next_reference = j - prev;
        fwrite(&next_reference, 2, 1, output_file);
        for (int i = 0; i < next_reference; i++) {
            fwrite(file_buffer + prev + i, 1, 1, output_file);
        }
    }

    free(cbuf);
    free(circ_buffer);
    free(file_buffer);
    free(tmp_buff);
    fclose(output_file);
}

void lempel_ziv_decode(char *in_file_name, char *out_file_name)
{
    FILE 		            *input_file, 
				            *output_file;
    size_t 		            file_len, 
				            j, 
				            prev;
    uint16_t 	            buf_idx, 
				            seek_idx, 
				            next_dupe;
    uint8_t 	            *circ_buffer,
				            ch;
	int 		            i;
    struct circular_buf_t   *cbuf;

    input_file = fopen(in_file_name, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "The input file was not found.\n");
        return;
    }

	next_dupe = 0;

    fseek(input_file, 0, SEEK_END);
    file_len = ftell(input_file); 
    rewind(input_file);
    fread(&next_dupe, 2, 1, input_file);

    circ_buffer = malloc(CIRCULAR_BUFFER_CAPACITY * sizeof(uint8_t));
    cbuf = circular_buf_init(circ_buffer);


    j = 0; buf_idx = 0; seek_idx = 0; ch = 0;
	
    output_file = fopen(out_file_name, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "The output file could not be created / was not found.\n");
        free(circ_buffer);
        free(cbuf);
        return;
    }

    while (j < file_len) {
        for (i = 0; i < next_dupe; i++) {
            fread(&ch, 1, 1, input_file);
            fwrite(&ch, 1, 1, output_file);
            circular_buf_put(cbuf, ch);
            j++;
        }

        fread(&buf_idx, 2, 1, input_file);
        fread(&seek_idx, 2, 1, input_file);

        for (i = 0; i < seek_idx; i++) {
            circular_buf_put(cbuf, *(cbuf->buffer + buf_idx + i));
            fwrite((cbuf->buffer + buf_idx + i), 1, 1, output_file);
        }

        fread(&next_dupe, 2, 1, input_file);
        j += 6;
        buf_idx = 0; seek_idx = 0;
    }

    free(circ_buffer);
    free(cbuf);
    fclose(input_file);
    fclose(output_file);
}

void huff_encode(char *in_file_name, char *out_file_name)
{
    FILE				*input_file,
                        *output_file;
	uint8_t 			*file_buffer,
						*s,
	                    code[ASCII],
                        cur_byte;
	size_t 				file_len,
						j,
                        i;
	struct huffman_t 	*huff;
	struct huff_node_t 	*root;

    input_file = fopen(in_file_name, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "The input file was not found.\n");
        return;
    }

	fseek(input_file, 0, SEEK_END);
    file_len = ftell(input_file); 
    rewind(input_file);
    file_buffer = (uint8_t *)(malloc(file_len * sizeof(uint8_t)));
    fread(file_buffer, file_len, 1, input_file);
    fclose(input_file);

	huff = malloc_huff();

	huff->freq = find_freq(file_buffer, file_len);

	root = construct_huff_tree(huff);

	create_huff_codes(huff, root, code, 0);

	output_file = fopen(out_file_name, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "The output file could not be created / was not found.\n");
        free(huff->freq);
        free(huff);
        free(root);
        free(file_buffer);
        return;
    }

    remove(in_file_name);

	fwrite(huff->freq, 4, ASCII, output_file);

	cur_byte = 0;

	for (j = 0; j < file_len; j++) {
        for (s = *(huff->bitsets + *(file_buffer + j)); *s; s++) {
            cur_byte <<= 1;

            if (*s == '1') 
                cur_byte |= 1;

            huff->bitset_pos++;

            if (huff->bitset_pos == 8) {
                fputc(cur_byte, output_file);
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
			fputc(cur_byte, output_file);
	}

	fclose(output_file);
	free(file_buffer);
    free(huff->freq);
    free(huff);
	free(root);
}

void huff_decode(char *in_file_name, char *out_file_name)
{
    FILE				*input_file,
                        *output_file;
	uint8_t 			*file_buffer,
                        last_pos;
	size_t 				file_len,
						j,
                        i;
	struct huffman_t 	*huff;
	struct huff_node_t 	*root,
                        *curr;


    input_file = fopen(in_file_name, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "The input file was not found.\n");
        return;
    }

	
    fseek(input_file, 0, SEEK_END);

    file_len = ftell(input_file) - (4 * ASCII);

    rewind(input_file);

    file_buffer = (uint8_t *)(malloc(file_len * sizeof(uint8_t)));

	huff = malloc_huff();

	huff->freq = (int *)(malloc(ASCII * sizeof(int)));

	fread(huff->freq, ASCII, 4, input_file);
    
	fread(file_buffer, file_len, 1, input_file);

    fclose(input_file);
	
	root = construct_huff_tree(huff);

	uint8_t bits[file_len][8];

    output_file = fopen(out_file_name, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "The output file could not be created / was not found.\n");
        return;
    }
	
    remove(in_file_name);

	j = 0;
	
    curr = root;

	for (j = 0; j < file_len; j++) {
		for (int i = 7; i >= 0; --i) {
			bits[j][i] = *(file_buffer + j) & (1 << i)  ? '1' : '0';
		}
	}

	j = 0; i = 7;
	while (j < file_len) {
		if (bits[j][i] == '1')
			curr = curr->right;
		else
			curr = curr->left;

		if (curr->left == NULL && curr->right == NULL) {
			fputc(curr->c, output_file);
			curr = root;
		}

		i--;
		if (i == -1) {
			i = 7;
			j++;
		}
	}

    fclose(output_file);
    free(huff->freq);
    free(huff);
	free(file_buffer);
	free(root);
}


int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "commands: lzc\t|\tlzd\t|\thmc\t|\thmd\n");
        fprintf(stderr, "usage: %s <path_to_input_file> <path_to_output_file> <command> \n", argv[0]);
        fprintf(stderr, "example: %s file.txt file.lz.bin lz-c\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[2], "lzc") == 0)
        lempel_ziv_encode(argv[1], argv[2]);
    if (strcmp(argv[2], "hmc") == 0)
        huff_encode(argv[1], argv[2]);
    if (strcmp(argv[2], "hmd") == 0)
        huff_decode(argv[1], argv[2]);
    if (strcmp(argv[2], "lzd") == 0)
        lempel_ziv_decode(argv[1], argv[2]);
    return 0;
}