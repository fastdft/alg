#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define PATTERN_BUFFER_SIZE 1000000
#define PATTERN_CHARSET_SIZE 256
#define PATTERN_MAX_LENGTH  256
#define PATTERN_HASH_SIZE 65536
#define STR_BUFFER_SIZE (10000000 + 100)

struct PatternList
{
    const char *pat;
    PatternList *next;
    int pri;
};

unsigned int HashTwoBytes(const char *str)
{
    unsigned short hash = *(const unsigned short *)str;
    return hash;
}

//we assume that the shift distance should less than 256 for all pattern, so the shift can be "unsigned char"
int PreprocessPattern(const char *pattern, int b_size, unsigned char shift[], PatternList hash_to_pat[])
{
    const char *pat_p = pattern;
    int next_index = PATTERN_HASH_SIZE;

    while(*pat_p)
    {
        int p_len = 0;
        const char *p = pat_p;
        int hash1 = 0;

        while(*p && (p_len = strlen(p)) >= b_size)
        {
            hash1 = HashTwoBytes(p);
            shift[hash1] = (shift[hash1] > (p_len - b_size) ? (p_len - b_size) : shift[hash1]);
            p++;
        }

        PatternList *list = &hash_to_pat[hash1];
        if (list->pat == NULL)
        {
            list->pat = pat_p;
            list->next = NULL;
            list->pri = 0;
        }
        else
        {
            while(list->next && list->pat != pat_p)
            {
                list = list->next;
            }
            if (list->pat != pat_p)
            {
                list->next = &hash_to_pat[next_index++];
                list = list->next;
                list->pat = pat_p;
                list->next = NULL;
                list->pri = 0;
            }
        }

        pat_p += (strlen(pat_p) + 1);
    }

    return 0;
}

int ReadPattern(FILE *file_handle, char *pattern, int size, int &l_min, int &l_max)
{
    int pattern_size = 0;
    char buf[PATTERN_MAX_LENGTH] = {0};
    int pattern_count = 0;
    int min = PATTERN_MAX_LENGTH;
    int max = 0;

    while(fgets(buf, PATTERN_MAX_LENGTH, file_handle))
    {
        pattern_count++;
        int buf_len = strlen(buf);
        //for windows, remove redundant '\r' or '\n'
        while (buf[buf_len-1] == '\n' || buf[buf_len-1] == '\r')
        {
            --buf_len;
            buf[buf_len] = 0;
        }
        strncpy(pattern + pattern_size, buf, buf_len);

        min = (min > buf_len ? buf_len : min);
        max = (max < buf_len ? buf_len : max);

        pattern_size += buf_len;
        *(pattern + pattern_size) = 0;
        pattern_size++;

        if (pattern_size >= size)
        {
            printf("the pattern size is not enough\n");
            return -1;
        }
        memset(buf, 0, sizeof(buf));
    }
    *(pattern + pattern_size) = 0;

    l_min = min;
    l_max = max;
    return pattern_count;
}

bool CheckMatch(const char *str, const char *des)
{
    int i=0;
    while(*(des+i) && *(str+i) == *(des+i))
        i++;

    if (*(des+i))
        return false;
    else
        return true;
}

int DoMultiMatch(const char *str, const char *str_end, int b_size, int l_min, unsigned char shift[], PatternList hash_to_pat[])
{
    if (str_end - str < l_min)
        return -1;

    const char *str_p = str + l_min - b_size;
    while(str_p + b_size <= str_end)
    {
        int hash1 = HashTwoBytes(str_p);
        assert(hash1 < PATTERN_HASH_SIZE);

        int move = shift[hash1];
        if (move == 0)
        {
            //this may be a match
            PatternList *pat_list = &hash_to_pat[hash1];
            while(pat_list && pat_list->pat)
            {
                int pat_len = strlen(pat_list->pat);
                const char *str_beg = (str_p - pat_len + b_size < str ? str : str_p - pat_len + b_size);
                if (CheckMatch(str_beg, pat_list->pat))
                {
                    pat_list->pri++;
                    break;
                }
                pat_list = pat_list->next;
            }
            move = 1;
        }

        str_p += move;
    }

    return 0;
    
}

int main(int argc, const char *argv[])
{
    //argc = 4;
    //argv[1] = "wordfilter.txt";
    //argv[2] = "chinese_new.txt";
    //argv[3] = "out.txt";

    if (argc != 4)
    {
        printf("Usage: bin.exe pattern_file word_file out_file\n");
        return 0;
    }

    FILE *pattern_handle = fopen(argv[1], "r");
    if (pattern_handle == NULL)
    {
        printf("Can't open pattern file\n");
        return -1;
    }
    fseek(pattern_handle, 0, SEEK_END);
    int file_size = ftell(pattern_handle);
    fseek(pattern_handle, 0, SEEK_SET);

    //read pattern, l_min, l_max, pattern_count
    char *patterns = (char *)malloc(file_size + 10);
    memset(patterns, 0, file_size + 10);
    int l_min = 0, l_max = 0;
    int patterns_count = ReadPattern(pattern_handle, patterns, file_size + 10, l_min, l_max);
    if (patterns_count < 0)
        return 0;
    fclose(pattern_handle);

    //get b_size
    int b_size = log((double)2*l_min*patterns_count)/log((double)PATTERN_CHARSET_SIZE);
    b_size = 2;

    //set shift, hash_to_pattern
    unsigned char *shift = (unsigned char *)malloc(PATTERN_HASH_SIZE);
    for (int i=0; i<PATTERN_HASH_SIZE; i++)
    {
        shift[i] = l_min - b_size + 1;
    }
    PatternList *hash_to_pattern = (PatternList *)malloc(PATTERN_HASH_SIZE*2*sizeof(PatternList));
    memset(hash_to_pattern, 0, PATTERN_HASH_SIZE*2*sizeof(PatternList));
    PreprocessPattern(patterns, b_size, shift, hash_to_pattern);

    FILE *str_handle = fopen(argv[2], "r");
    if (str_handle == NULL)
    {
        printf("Can't open scentence file\n");
        return -1;
    }
    char *str_buffer = (char *)malloc(STR_BUFFER_SIZE);
    int already_read = 0;

    bool reach_end = false;
    do
    {
        int got_read_size = fread(str_buffer + already_read, 1, STR_BUFFER_SIZE - already_read, str_handle);
        if (got_read_size != STR_BUFFER_SIZE - already_read)
            reach_end = true;

        int last_enter_index = already_read + got_read_size - 1;
        if (!reach_end)
        {
            while(last_enter_index > 0 && *(str_buffer + last_enter_index) != '\n')
            {
                last_enter_index--;
            }
        }

        int match_ret = DoMultiMatch(str_buffer, str_buffer + last_enter_index + 1, b_size, l_min, shift, hash_to_pattern);
        if (match_ret < 0)
        {
            printf("DoMultiMatch error happens\n");
        }
        memcpy(str_buffer, str_buffer + last_enter_index + 1, already_read + got_read_size - last_enter_index - 1);
        already_read = already_read + got_read_size - last_enter_index - 1;
    }while(reach_end == false);

    FILE *out = fopen(argv[3], "w");
    if (out == NULL)
    {
        printf("Can't create output file\n");
        return -1;
    }
    for (int i=0; i<PATTERN_HASH_SIZE; i++)
    {
        PatternList *list = hash_to_pattern + i;
        while(list && list->pat)
        {
            fprintf(out, "%s %d\n", list->pat, list->pri);
            list = list->next;
        }
    }
    fclose(out);

    free(str_buffer);
    free(patterns);
    free(shift);
    free(hash_to_pattern);

    return 0;
}