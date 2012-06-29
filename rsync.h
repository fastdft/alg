#ifndef RSYNC_H
#define RSYNC_H

struct FileSnap
{
    int block_size;
    int block_num;
    int file_size;
    int week_sig_unit_size;
    int strong_sig_unit_size;
    char *week_sig;
    char *strong_sig;
};

#define FILE_PATCH_TYPE_STR 1
#define FILE_PATCH_TYPE_TOKEN 2

struct FilePatchBase
{
    char type;
};

struct FilePatchStr
{
    char type;
    string str;
};

struct FilePatchBlock
{
    char type;
    int token;
};

struct FilePatch
{
    vector<FilePatchBase *> list;
};
#endif
