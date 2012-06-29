#include <stdio.h>
#include <assert.h>
#include "bwt.h"

//TODO: not finished
//bwt transform


int week_signature(char *data, int size, char *sig)
{}

int strong_signature(char *data, int size, char *sig)
{}

int calc_block_size(int size)
{}

FileSnap* server_generate_filesnap(FILE *fd)
{
    fseek(fd, 0, SEEK_END);
    int file_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    FileSnap *snap = (FileSnap *)malloc(sizeof(FileSnap));
    memset(snap, 0, sizeof(FileSnap));
    int block_size = calc_block_size(file_size);
    int block_num = file_size%block_size : file_size/block_size + 1 : file_size/block_size;
    char *buff = (char *)malloc(block_size);

    snap->block_size = block_size;
    snap->block_num = block_num;
    snap->file_size = file_size;
    snap->week_sig_unit_size = ;
    snap->strong_sig_unit_size = ;
    snap->week_sig = (char *)malloc(block_num * snap->week_sig_unit_size);
    snap->strong_sig = (char *)malloc(block_num * snap->strong_sig_unit_size);
    for (int i=0; i<block_num; i++)
    {
        int read_size = fread(buff, 1, block_size, fd);
        week_signature(buff, read_size, snap->week_sig + i * snap->week_sig_unit_size);
        strong_signature(buff, read_size, snap->strong_sig + i * snap->strong_sig_unit_size);
    }

    free(buff);

    printf("size of file snap is %d\n", sizeof(FileSnap) + block_num * (snap->week_sig_unit_size + snap->strong_sig_unit_size));

    return snap;
}

void server_destroy_filesnap(FileSnap *snap)
{
    free(snap->week_sig);
    free(snap->strong_sig);
    free(snap);
}

int server_merge_filepatch(FileSnap *snap, FilePatch *patch, FILE *src, FILE *des)
{
    vector<FilePatchBase *>::iterator iter;
    char *buff = (char *)malloc(snap->block_size);
    for (iter = patch->list.begin(); iter != patch->list.end(); iter++)
    {
        if (iter->type == FILE_PATCH_TYPE_STR)
        {
            FilePatchStr *str_patch = static_cast<FilePatchStr *>(iter);
            fwrite(str_patch->str.c_str(), 1, str_patch->str.size(), des);
        }
        else if (iter->type == FILE_PATCH_TYPE_TOKEN)
        {
            FilePatchToken *token_patch = static_cast<FilePatchToken *>(iter);
            fseek(src, token_patch->token * snap->block_size, SEEK_SET);
            fread(buff, 1, snap->block_size, src);
            fwrite(buff, 1, snap->block_size, des);
        }
        else
        {
            cout<<"type error!"<<endl;
            return;
        }
    }

    free(buff);
}

FilePatch *client_generate_filepatch(FILE *fd, FileSnap *snap)
{
    int buff_size = snap->block_size * 10;
    char *buff = (char *)malloc(buff_size);;
    int read_size, offset;
    char weak_sig[128];
    char strong_sig[128];

    FilePatch *file_patch = new FilePatch;

    do
    {
        read_size = fread(buff, buff_size, 1, fd);
        int str_begin_offset = -1;

        for (offset = 0; offset <= read_size - snap->block_size; )
        {
            weak_signature(buff + offset, snap->block_size, weak_sig);
            if (find_weak_sig(weak_sig) < 0)
            {
                if (str_begin_offset == -1)
                    str_begin_offset = offset;
                offset++;
            }
            else
            {
                int pos;
                strong_signature(buff + offset, snap->block_size, strong_sig);
                if (pos = find_strong_sig(strong_sig) < 0)
                {
                    if (str_begin_offset == -1)
                        str_begin_offset = offset;
                    offset++;
                }
                else
                {
                    if (str_begin_offset != -1)
                    {
                        FilePatchStr *patch = new FilePatchStr;
                        patch->type = FILE_PATCH_TYPE_STR;
                        patch->str = string(buff, str_begin_offset, offset);
                        file_patch.list.push_back((FilePatchBase *)patch);
                    }
                    FilePatchToken *token_patch = new FilePatchToken;
                    token_patch->type = FILE_PATCH_TYPE_TOKEN;
                    token_patch->token = pos;
                    file_patch.list.push_back((FilePatchBase *)patch);

                    offset += snap->block_size;
                    str_begin_offset = -1;
                }
            }
        }

        //TODO:for simple, the rest unmatch 
        if (str_begin_offset != -1)
        {
            FilePatchStr *patch = new FilePatchStr;
            patch->type = FILE_PATCH_TYPE_STR;
            patch->str = string(buff, str_begin_offset, read_size);
            file_patch.list.push_back((FilePatchBase *)patch);
        }

    } while(read_size == buff_size);

    //TODO: should sig the whole file
    whole_file_signature();

    return file_patch
}

void client_destroy_filepatch(FilePatch *patch)
{
    delete patch;
}

int rsync_local(FILE *fd1, FILE *fd2, FILE *fd3)
{
    while(1)
    {
        FileSnap *snap = server_generate_filesnap(fd1);
        FilePatch *patch = client_generate_filepatch(fd2, snap);

        ret = server_merge_filepatch(fd1, patch, fd3);

        server_destroy_filesnap(snap);
        client_destroy_filepatch(patch);

        if (ret >= 0)
            break;
    }
}

int rsync_test()
{
    FILE *f_server = fopen("rsync_1.txt", "r");
    FILE *f_client = fopen("rsync_2.txt", "r");
    FILE *f_new = fopen("rsync.txt", "w");

    assert(f_server && f_client && f_new);

    rsync_local(f_server, f_client, f_new);

    fclose(f_server);
    fclose(f_client);
    fclose(f_new);

    return 0;
}
