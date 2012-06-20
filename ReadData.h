#ifndef READDATA_H
#define READDATA_H

#define RAWDATA_TYPE_STRING 1
#define RAWDATA_TYPE_BYTE   2

int ReadData(void *buf, int size, int count, int RawDataType, const char *name);

#endif