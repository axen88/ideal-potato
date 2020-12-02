// gcc -O3 -Wall -o create_index create_index.c -lpthread


#define _GNU_SOURCE
#include <assert.h>
#include <dirent.h>
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LINE_SIZE (1024 * 1024 * 4)

static unsigned nb_files_per_mram;
static unsigned nb_files_total;
static unsigned nb_files;
static unsigned nb_mrams_total;
static unsigned nb_mrams;
static unsigned nb_words;

static FILE *words_map_file;
static FILE *docs_map_file;
static FILE *map_file;
static FILE *idx_file;

static char *output_file_prefix;

enum xfer_direction {
    xfer_read,
    xfer_write,
};
static inline void xfer_file(uint8_t *buffer, ssize_t size_to_xfer, FILE *f, enum xfer_direction direction)
{
    while (size_to_xfer > 0) {
        ssize_t size_xfer;
        if (direction == xfer_read) {
            size_xfer = fread(buffer, 1, size_to_xfer, f);
        } else {
            size_xfer = fwrite(buffer, 1, size_to_xfer, f);
        }
        assert(size_xfer > 0);
        size_to_xfer -= size_xfer;
        buffer += size_xfer;
    }
}

//#########################################################################################
//# FILE LIST #############################################################################
//#########################################################################################

#define FILES_ARRAY_INIT_SIZE (512)
char **files_array;
unsigned int files_array_size = FILES_ARRAY_INIT_SIZE;

static void init_explore_folder()
{
    files_array = (char **)malloc(sizeof(char *) * files_array_size);
    assert(files_array != NULL);
    nb_files_total = 0;
}

static void rec_explore_folder(const char *dir_name)
{
    DIR *directory = opendir(dir_name);
    if (directory == NULL)
        return;

    struct dirent *curr_dir;
    while ((curr_dir = readdir(directory)) != NULL) {
        const char *curr_name = curr_dir->d_name;
        assert(curr_name != NULL);

        if (curr_dir->d_type == DT_DIR && (strcmp(curr_name, ".") == 0 || strcmp(curr_name, "..") == 0))
            continue;

        char *full_name;
        assert(asprintf(&full_name, "%s/%s", dir_name, curr_name) != 0);

        switch (curr_dir->d_type) {
        case DT_DIR:
            rec_explore_folder(full_name);
            free(full_name);
            break;
        case DT_REG: {
            fprintf(docs_map_file, "%u %s\n", nb_files_total, full_name);
            files_array[nb_files_total++] = full_name;
            if (nb_files_total >= files_array_size - 1) {
                files_array_size *= 2;
                files_array = realloc(files_array, sizeof(char *) * files_array_size);
                assert(files_array != NULL);
            }
        } break;
        default:
            break;
        }
    }

    closedir(directory);
}

static char *get_next_file(unsigned file_id)
{
    if (file_id >= nb_files_total) {
        return NULL;
    }
    __sync_fetch_and_add(&nb_files, 1);
    return files_array[file_id];
}

//#########################################################################################
//# GO THROUGH DICTIONNARY ################################################################
//#########################################################################################

typedef SLIST_HEAD(head_words, word) head_words_t;
typedef struct word {
    char *word_name;
    unsigned word_id;
    SLIST_ENTRY(word) next;
} word_t;

typedef uint8_t hash_t;
#define WORDS_HASHTABLE_SIZE (1 << (CHAR_BIT * sizeof(hash_t)))
head_words_t words_hashtable[WORDS_HASHTABLE_SIZE];

#define MAX_WORD_NUM 1000000
word_t *words_array[MAX_WORD_NUM];

static hash_t word_hash(char *word_name)
{
    size_t size = strlen(word_name);
    hash_t hash = 0;
    for (unsigned int i = 0; i < size; i++) {
        hash += word_name[i];
    }
    return hash;
}

static int word_in_dictionnary(char *word_name)
{
    head_words_t *head = &words_hashtable[word_hash(word_name)];
    word_t *word;
    SLIST_FOREACH(word, head, next)
    {
        if (!strcmp(word_name, word->word_name))
            return word->word_id;
    }
    return -1;
}

static void parse_dictionnary(char *dictionnary_file_name)
{
    nb_words = 0;
    for (unsigned int i = 0; i < WORDS_HASHTABLE_SIZE; i++) {
        SLIST_INIT(&words_hashtable[i]);
    }

    FILE *dictionnary_file = fopen(dictionnary_file_name, "r");
    assert(dictionnary_file != NULL);


    char str[MAX_LINE_SIZE];
    while (fgets(str, MAX_LINE_SIZE, dictionnary_file) != NULL) {
        size_t str_len = strlen(str) - 1;
        assert(str_len < MAX_LINE_SIZE - 1);
        str[str_len] = '\0';
        bool skip_word = false;
        for (unsigned each_char = 0; each_char < str_len; each_char++) {
            char curr_char = str[each_char];
            if ((curr_char < 'a' || curr_char > 'z') && (curr_char < 'A' || curr_char > 'Z')) {
                skip_word = true;
                continue;
            } else if (curr_char >= 'A' && curr_char <= 'Z') {
                str[each_char] = 'a' + curr_char - 'A';
            }
        }
        if (skip_word) {
            continue;
        }
        word_t *word = (word_t *)malloc(sizeof(word_t));
	assert(word != NULL);
	words_array[nb_words] = word;
        word->word_id = nb_words++;
	assert(nb_words < MAX_WORD_NUM);
        word->word_name = strdup(str);
        assert(word->word_name != NULL);
        SLIST_INSERT_HEAD(&words_hashtable[word_hash(word->word_name)], word, next);
        fprintf(words_map_file, "%u %s\n", word->word_id, word->word_name);
    }
    fclose(dictionnary_file);
    printf("dictionnary parsed: %u words\n", nb_words);
}

//#########################################################################################
//# TIME ##################################################################################
//#########################################################################################

static double my_clock()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
    return (1.0e-9 * t.tv_nsec + t.tv_sec);
}

static void print_percentage_to_completion()
{
    double start_time = my_clock();
    usleep(1000000);
    while (nb_files != nb_files_total) {
        unsigned curr_nb_files = nb_files;
        unsigned curr_nb_mrams = nb_mrams;

        double curr_time = my_clock() - start_time;
        double time_left = curr_nb_files != 0 ? curr_time * nb_files_total / curr_nb_files : 0.0;

        printf("\r | %.2f%% - %uh%umin%us (ETA %uh%umin%us) (%u/%u mrams)  | ",
            (double)curr_nb_files * 100.0 / (double)nb_files_total, (unsigned)curr_time / 3600,
            (unsigned)curr_time / 60 % 60, (unsigned)curr_time % 60, (unsigned)time_left / 3600,
            (unsigned)time_left / 60 % 60, (unsigned)time_left % 60, curr_nb_mrams, nb_mrams_total);
        fflush(stdout);
        usleep(1000000);
    }
    printf("\n");
}

//#########################################################################################
//# PARSE FILES ###########################################################################
//#########################################################################################

#define BUFFER_INIT_SIZE (16)
typedef TAILQ_HEAD(head_m_file, m_file) head_m_file_t;
typedef struct m_file {
    unsigned file_id;
    uint8_t *buffer;
    unsigned buffer_size;
    unsigned buffer_offset;
    unsigned last_pos;
    TAILQ_ENTRY(m_file) next;
} m_file_t;

#define PRINT_INT(_val, val_to_print, print_statement)                                                                 \
    do {                                                                                                               \
        unsigned _print_int_val = (_val);                                                                              \
        do {                                                                                                           \
            unsigned val_to_print = _print_int_val & 0x7f;                                                             \
            if (_print_int_val < 0x80) {                                                                               \
                val_to_print |= 0x80;                                                                                  \
            }                                                                                                          \
            (print_statement);                                                                                         \
            _print_int_val >>= 7;                                                                                      \
        } while (_print_int_val != 0);                                                                                 \
    } while (0)

static void print_pos(m_file_t *file, unsigned pos)
{
    PRINT_INT(pos - file->last_pos, val_to_print, file->buffer[file->buffer_offset++] = val_to_print);
    if (file->buffer_offset > (file->buffer_size / 2)) {
        file->buffer_size *= 2;
        file->buffer = realloc(file->buffer, file->buffer_size);
        assert(file->buffer != NULL);
    }
    file->last_pos = pos;
}

static void print_int(unsigned val, FILE *f) { PRINT_INT(val, val_to_print, fprintf(f, "%c", val_to_print)); }

static m_file_t *get_file(head_m_file_t *files, unsigned file_id)
{
    m_file_t *file;
    if ((!TAILQ_EMPTY(files)) && ((file = TAILQ_LAST(files, head_m_file))->file_id == file_id))
        return file;
    file = (m_file_t *)malloc(sizeof(m_file_t));
    assert(file != NULL);
    file->file_id = file_id;
    file->buffer_size = BUFFER_INIT_SIZE;
    file->buffer = (uint8_t *)malloc(file->buffer_size);
    assert(file->buffer);
    file->buffer_offset = 0;
    file->last_pos = 0;
    TAILQ_INSERT_TAIL(files, file, next);
    return file;
}

static void add_pos(int word_id, unsigned file_id, unsigned pos, head_m_file_t *words)
{
    if (word_id < 0) {
        return;
    }
    print_pos(get_file(&words[word_id], file_id), pos);
}

static void parse_file(char *file_name, unsigned file_id, head_m_file_t *words)
{
    FILE *f = fopen(file_name, "r");
    assert(f != NULL);
    char str[MAX_LINE_SIZE];
    unsigned word_pos = 0;
    while (fgets(str, MAX_LINE_SIZE, f) != NULL) {
        size_t str_len = strlen(str);
        assert(str_len < MAX_LINE_SIZE - 1);
        char *word_start = str;
        for (unsigned each_char = 0; each_char < str_len; each_char++) {
            char curr_char = str[each_char];
            if (curr_char >= 'a' && curr_char <= 'z') {
                continue;
            } else if (curr_char >= 'A' && curr_char <= 'Z') {
                str[each_char] = 'a' + curr_char - 'A';
            } else {
                str[each_char] = '\0';
                if (&str[each_char] != word_start) {
                    add_pos(word_in_dictionnary(word_start), file_id, word_pos++, words);
                }
                word_start = &str[each_char + 1];
            }
        }
    }
    fclose(f);
}

#define DID_GROUP_SIZE 100
static unsigned print_group_block(FILE *f, unsigned nb_docs)
{
    if (nb_docs % DID_GROUP_SIZE == 0) {
        print_int(0, f);
        print_int(0, f);
    }
    return nb_docs + 1;
}

#define MRAM_SIZE (62<<20)
#define NB_DPU_THREAD (16)
static void generate_mram(head_m_file_t *words, unsigned mram_id)
{
    char mram_filename[FILENAME_MAX];
    sprintf(mram_filename, "%s/%u.bin", output_file_prefix, mram_id);
    FILE *f_index = fopen(mram_filename, "wb");
    unsigned mram_header_size = sizeof(uint32_t) * NB_DPU_THREAD * nb_words;
    uint32_t *mram_header = (uint32_t *)malloc(mram_header_size);
    assert(mram_header);
    FILE *thread_files[NB_DPU_THREAD];
    for (unsigned each_thread = 0; each_thread < NB_DPU_THREAD; each_thread++) {
        sprintf(mram_filename, "%s/%u.bin.%u", output_file_prefix, mram_id, each_thread);
        thread_files[each_thread] = fopen(mram_filename, "wb+");
        assert(thread_files[each_thread] != NULL);
        assert(unlink(mram_filename) == 0);
    }

    unsigned mram_start_id = mram_id * nb_files_per_mram;
    for (unsigned each_word = 0; each_word < nb_words; each_word++) {
        unsigned last_doc_id = (mram_id + 1) * nb_files_per_mram;

        unsigned nb_docs[NB_DPU_THREAD] = { 0 };
        unsigned last_file_id[NB_DPU_THREAD] = { 0 };

        for (unsigned each_thread = 0; each_thread < NB_DPU_THREAD; each_thread++) {
            mram_header[each_word * NB_DPU_THREAD + each_thread] = ftell(thread_files[each_thread]);
            print_int(last_doc_id, thread_files[each_thread]);
        }
        m_file_t *file;
        TAILQ_FOREACH(file, &words[each_word], next)
        {
            unsigned thread_id = (file->file_id - mram_start_id) * NB_DPU_THREAD / nb_files_per_mram;
            nb_docs[thread_id] = print_group_block(thread_files[thread_id], nb_docs[thread_id]);
            print_int(file->file_id - last_file_id[thread_id], thread_files[thread_id]);
            last_file_id[thread_id] = file->file_id;
            print_int(file->buffer_offset, thread_files[thread_id]);
            xfer_file(file->buffer, file->buffer_offset, thread_files[thread_id], xfer_write);
            free(file->buffer);
        }
        for (unsigned each_thread = 0; each_thread < NB_DPU_THREAD; each_thread++) {
            print_group_block(thread_files[each_thread], nb_docs[each_thread]);
            print_int(last_doc_id - last_file_id[each_thread], thread_files[each_thread]);
            print_int(0, thread_files[each_thread]);
        }
        while (!TAILQ_EMPTY(&words[each_word])) {
            file = TAILQ_FIRST(&words[each_word]);
            TAILQ_REMOVE(&words[each_word], file, next);
            free(file);
        }
    }

    unsigned int offset = mram_header_size;
    for (unsigned each_thread = 0; each_thread < NB_DPU_THREAD; each_thread++) {
        if (each_thread != 0) {
            offset += ftell(thread_files[each_thread - 1]);
        }
        for (unsigned each_word = 0; each_word < nb_words; each_word++) {
            mram_header[each_word * NB_DPU_THREAD + each_thread] += offset;
        }
    }
    xfer_file((uint8_t *)mram_header, mram_header_size, f_index, xfer_write);
    uint8_t *tmp_buffer = (uint8_t *)malloc(64 << 20);
    for (unsigned each_thread = 0; each_thread < NB_DPU_THREAD; each_thread++) {
        unsigned file_size = ftell(thread_files[each_thread]);
        fseek(thread_files[each_thread], 0, SEEK_SET);
        xfer_file(tmp_buffer, 1, thread_files[each_thread], xfer_read);
        xfer_file(tmp_buffer, file_size, f_index, xfer_write);
        fclose(thread_files[each_thread]);
    }
    free(tmp_buffer);
    free(mram_header);
    unsigned mram_final_size = ftell(f_index);
    if (mram_final_size > MRAM_SIZE) {
        fprintf(stderr, "MRAM#%u is too big to fit in DPU\n", mram_id);
    } else {
        unsigned fill_size = MRAM_SIZE - mram_final_size;
        uint8_t *fill = malloc(fill_size);
        assert(fill != NULL);
        xfer_file(fill, fill_size, f_index, xfer_write);
        free(fill);
    }
    fclose(f_index);
}

static void *generate_mrams(__attribute__((unused)) void *arg)
{
    for (unsigned each_mram = __sync_fetch_and_add(&nb_mrams, 1); each_mram < nb_mrams_total;
         each_mram = __sync_fetch_and_add(&nb_mrams, 1)) {

        head_m_file_t *words = (head_m_file_t *)malloc(sizeof(head_m_file_t) * nb_words);
        assert(words != NULL);
        for (unsigned i = 0; i < nb_words; i++) {
            TAILQ_INIT(&words[i]);
        }

        for (unsigned int each_file = 0; each_file < nb_files_per_mram; each_file++) {
            unsigned file_id = each_file + nb_files_per_mram * each_mram;
            char *file_name = get_next_file(file_id);
            if (file_name == NULL)
                break;
            parse_file(file_name, file_id, words);
        }

        generate_mram(words, each_mram);

        free(words);
    }
    return NULL;
}

//#########################################################################################
//# MAIN ##################################################################################
//#########################################################################################

void init_files()
{
    char *words_map_file_name, *docs_map_file_name;

    assert(mkdir(output_file_prefix, 0744) == 0);

    assert(asprintf(&words_map_file_name, "%s/words.txt", output_file_prefix) != 0);
    assert((words_map_file = fopen(words_map_file_name, "w")) != NULL);

    assert(asprintf(&docs_map_file_name, "%s/docs.txt", output_file_prefix) != 0);
    assert((docs_map_file = fopen(docs_map_file_name, "w")) != NULL);

    assert(asprintf(&docs_map_file_name, "%s/%s.map", output_file_prefix, output_file_prefix) != 0);
    assert((map_file = fopen(docs_map_file_name, "w")) != NULL);

    assert(asprintf(&docs_map_file_name, "%s/%s.idx", output_file_prefix, output_file_prefix) != 0);
    assert((idx_file = fopen(docs_map_file_name, "w")) != NULL);
}

void generate_index(void)
{
	int len = 0;
        head_m_file_t *words = (head_m_file_t *)malloc(sizeof(head_m_file_t) * nb_words);
        assert(words != NULL);
        for (unsigned i = 0; i < nb_words; i++) {
            TAILQ_INIT(&words[i]);
        }

	printf("parsing files...\n");

        for (unsigned int each_file = 0; each_file < nb_files_total; each_file++) {
            char *file_name = files_array[each_file];
            assert(file_name != NULL);
	    printf("\r%*c", len, ' ');
	    len = printf("\r %u/%u, parsing file: %s", each_file+1, nb_files_total, file_name);
	    fflush(stdout);
            parse_file(file_name, each_file, words);
        }

	printf("\nwriting data...\n");

	unsigned pos = 0;
	for (unsigned each_word = 0; each_word < nb_words; each_word++) {
	    m_file_t *file;
	    unsigned size = 0;
	    unsigned last_file_id = 0;
	    printf("\r%*c",len,' ');
	    len = printf("\r %u/%u, writing data of word: %s", each_word+1, nb_words, words_array[each_word]->word_name);
	    fflush(stdout);
	    TAILQ_FOREACH(file, &words[each_word], next)
	    {
	        if (file->buffer_offset != 0) {
		    unsigned tmp_size;
		    unsigned char tmp_val[4];
		    unsigned delta = file->file_id - last_file_id;
		    assert(file->file_id >= last_file_id);
		    last_file_id = file->file_id;

		    // 1. write document id
		    tmp_size = 0;
		    PRINT_INT(delta, val_to_print, tmp_val[tmp_size++] = val_to_print);
		    assert(tmp_size <= 4);
		    xfer_file(tmp_val, tmp_size, idx_file, xfer_write);
		    size += tmp_size;

		    // 2. write len
		    tmp_size = 0;
		    PRINT_INT(file->buffer_offset, val_to_print, tmp_val[tmp_size++] = val_to_print);
		    assert(tmp_size <= 4);
		    xfer_file(tmp_val, tmp_size, idx_file, xfer_write);
		    size += tmp_size;

		    // 3. write position info
		    xfer_file(file->buffer, file->buffer_offset, idx_file, xfer_write);
		    size += file->buffer_offset;
		}

		free(file->buffer);
	    }

	    if (size) {
	        fprintf(map_file, "%10u %10s %10u %10u\n", each_word, words_array[each_word]->word_name, pos, size);
		pos += size;
	    }
	}
	printf("\nfinished\n");
}

#define NB_THREAD (16)
int main(int argc, char **argv)
{
    if (argc != 4 ) {
        fprintf(stderr,
            "Usage: %s <input_directory_name> <output_file_prefix> <dictionnary_file_name>\n", argv[0]);
        exit(-1);
    }

    char *input_directory_name, *dictionnary_file_name;
    input_directory_name = argv[1];
    output_file_prefix = argv[2];
    dictionnary_file_name = argv[3];
    //nb_mrams_total = atoi(argv[4]);

    init_files();

    parse_dictionnary(dictionnary_file_name);
    init_explore_folder();
    rec_explore_folder(input_directory_name);

    if (0)
    {
    nb_files_per_mram = (nb_files_total + nb_mrams_total - 1) / nb_mrams_total;
    nb_mrams = 0;
    nb_files = 0;

    printf("%u files, %u files/mram\n", nb_files_total, nb_files_per_mram);

    pthread_t tid[NB_THREAD];
    for (unsigned each_thread = 0; each_thread < NB_THREAD; each_thread++) {
        assert(pthread_create(&tid[each_thread], NULL, generate_mrams, NULL) == 0);
    }
    print_percentage_to_completion();
    for (unsigned each_thread = 0; each_thread < NB_THREAD; each_thread++) {
        assert(pthread_join(tid[each_thread], NULL) == 0);
    }
    }
    else
    {
    	generate_index();
    }

    fclose(docs_map_file);
    fclose(words_map_file);
    fclose(idx_file);
    fclose(map_file);

    return 0;
}
