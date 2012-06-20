#ifndef STR_ALG_H
#define STR_ALG_H

#include <string>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
using namespace std;

int kmp_str_match(char *patten, char *str);

void suffix_array(const string &str, int sa[], int h[]);

void RmqTest();

string Lcs(string &s1, string &s2);
#endif
