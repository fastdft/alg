#include <sys/time.h>
#include <string.h>
#include "time_statistics.h"

struct timeval start_time;
struct timeval end_time;

void time_start()
{
	memset(&start_time, 0, sizeof(struct timeval));
	memset(&end_time, 0, sizeof(struct timeval));

	gettimeofday(&start_time, NULL);
}

unsigned int get_time_used()
{
	gettimeofday(&end_time, NULL);

	return (end_time.tv_sec - start_time.tv_sec)*1000000 + (end_time.tv_usec - start_time.tv_usec);
}
