#include "ReadData.h"
#include "stdio.h"

int ReadData(void *buf, int size, int count, int RawDataType, const char *name)
{
    const char *mode = NULL;
    switch(RawDataType)
    {
    case RAWDATA_TYPE_STRING:
        mode = "r";
        break;
    case RAWDATA_TYPE_BYTE:
        mode = "rb";
        break;
    default:
        return -1;
    }
    FILE *fp = fopen(name, mode);

    int pos = 0;
    while(fread((char*)buf+pos, size, 1, fp))
    {
        unsigned char flag = 0;
        for (int i=0; i<size; i++)
        {
            flag |= *((char*)buf+pos+i);
        }
        if (flag == 0)
            continue;

        if (pos + size >= size*count)
            break;

        pos += size;
    }

    fclose(fp);
}