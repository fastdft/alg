#include "str_alg.h"
#include "math.h"
#include "assert.h"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include "time_statistics.h"

void process_patten(char *patten, int data[]);


void process_patten(char *patten, int data[])
{
    int str_len = strlen(patten);
    data[0] = 0;

    for (int i=1; i<str_len; i++)
    {
        int q = data[i-1];
        while (true)
        {
            if (*(patten + i) == *(patten + q))
            {
                data[i] = q + 1;
                break;
            }
            else
            {
                q = data[q];
                if (q==0)
                {
                    data[i] = 0;
                    break;
                }
            }
        }
    }
}

int kmp_str_match(char *patten, char *str)
{
    int patten_len = strlen(patten);
    int *data = (int *)malloc(patten_len * sizeof(int));
    process_patten(patten, data);
    
    int str_len = strlen(str);
    int q=0;
    for (int i=0; i<str_len; i++)
    {
        if (*(str + i) == *(patten + q))
		{
            if (q == patten_len -1)
                return i-patten_len + 1;
            else
            {
                q++;
                continue;
            }
		}

        while(*(str + i) != *(patten + q))
        {
            q = data[q];
            if (q == 0)
                break;
        }
        if (*(str + i) == *(patten + q))
            q++;
    }

    free(data);
    return 0;
}

struct char_info
{
    char value;
    int index;
};

void sort_str(const string &str, int sa[], int rank[])
{
    vector<struct char_info> char_vector[26];
    for (int i=0; i<str.length(); i++)
    {
        char_info item;
        item.index = i;
        item.value = str[i];
        char_vector[str[i]-'a'].push_back(item);
    }

    int k=0;
    for(int i=0; i<26; i++)
    {
        int diff = k;
        for(int j=0; j<char_vector[i].size(); j++)
        {
            sa[k++] = char_vector[i][j].index;
            rank[char_vector[i][j].index] = diff;
        }
    }

}

void sa_sort_by_rank(int sa[], int l, int r, int rank[], int step, int rank_size)
{
    //sort sa[l] - sa[r] according to rank[sa[i] + step], and update rank
    //this can be optimaize by other sort algorithm
    for (int i=l; i<r; i++)
    {
        for (int j=i+1; j<=r; j++)
        {
            if (sa[i]+step>=rank_size)
                continue;
            else if (sa[j] + step >=rank_size || rank[sa[i]+step] > rank[sa[j]+step])
            {
                int temp = sa[i];
                sa[i]=sa[j];
                sa[j] = temp;
            }
        }
    }

    //update rank arrays
    int new_rank = rank[sa[l]];
    for(int i=l; i<=r; i++)
    {
        rank[sa[i]] = new_rank;
        if (sa[i+1]+step >= rank_size || rank[sa[i+1]+step] > rank[sa[i]+step])
            new_rank = i+1;
    }
}

void suffix_array(const string &str, int sa[], int height[])
{
    int str_len = str.length();
    int bit_len = 0;
    int rank[1000] = {0};
    int h[1000] = {0};

    assert(str_len < 1000);

    int tmp = str_len;
    while(tmp > 0 )
    {
        bit_len++;
        tmp /= 2;
    }

    sort_str(str, sa, rank);

    int step = 1;
    for(int i = 0; i < bit_len; i++)
    {
        int j=0;
        while(j<str.length()-1)
        {
            if(rank[sa[j]] != rank[sa[j+1]])
            {
                j++;
                continue;
            }
            else
            {
                int l_rank = j;
                while(j<str.length()-1 && rank[sa[j]] == rank[sa[j+1]])
                    j++;
                sa_sort_by_rank(sa, l_rank, j, rank, step, str_len);
                j++;
            }
        }
        step *= 2;
    }

    //add assert to let the result right
    for(int i=0; i<str_len; i++)
    {
        assert(rank[sa[i]] == i);
    }

    //compute height
    for(int i=0; i<str_len; i++)
    {
        if (rank[i] == 0)
            h[i] = 0;
        else
        {
            //this use a lema that h(x) = Height(Rank(x)), then h(i) >= h(i-1)-1;
            if (i==0)
                h[i] = 0;
            else
                h[i] = (h[i-1] - 1 > 0 ? h[i-1] - 1 : 0);
            while(i+h[i] < str_len && sa[rank[i]-1] + h[i] < str_len && (str[i+h[i]] == str[sa[rank[i]-1] + h[i]]))
                h[i]++;
        }
    }

    for(int i=0; i<str_len; i++)
    {
        height[i] = h[sa[i]];
    }
}

//st method
//using recursive method to construct sparse tree
#define MAX_RMG_NUMBER 100
#define MAX_RMG_NUMBER_BITS 7

struct RmgValue
{
    int value;
    int index;
};

void RmgConstructMax(int ar[], int length, RmgValue res[MAX_RMG_NUMBER][MAX_RMG_NUMBER_BITS])
{
    int bits = 0;
    int ar_len = length;
    while(ar_len >>= 1)
        bits++;
    bits++;

    for (int j = 0; j < bits; j++)
        for (int i = 0; i < length; i++)
        {
            if (j==0)
            {
                res[i][j].value = ar[i];
                res[i][j].index = i;
                continue;
            }
            int next = i+(int)pow((float)2, j-1);

            if (next >= length || res[i][j-1].value > res[next][j-1].value)
            {
                res[i][j].value = res[i][j-1].value;
                res[i][j].index = res[i][j-1].index;
            }
            else
            {
                res[i][j].value = res[next][j-1].value;
                res[i][j].index = res[next][j-1].index;
            } 
        }
}

RmgValue Rmg(int ar[], int len, int l, int r, RmgValue res[MAX_RMG_NUMBER][MAX_RMG_NUMBER_BITS])
{
    int diff = r - l;
    int bits = 0;
    while(diff >>= 1)
    {
        bits++;
    }

    if (res[l][bits].value > res[r-(1<<bits)][bits].value)
        return res[l][bits];
    else
        return res[r-(1<<bits)][bits];
}

void RmqTest()
{
    int ar[] = {4,9,3,5,56,45,34,64,72,23,54,36,64,23,12,35,62,93,19, 54, 34, 997, 43, 432, 3465, 986};
    RmgValue res[MAX_RMG_NUMBER][MAX_RMG_NUMBER_BITS] = {0};

    //construct
    RmgConstructMax(ar, sizeof(ar)/sizeof(int), res);

    //search range is [x, y)
    RmgValue ret = Rmg(ar, sizeof(ar)/sizeof(int), 0, 10, res);
    cout<<"value is "<<ret.value<<" index is "<<ret.index<<endl;

    ret = Rmg(ar, sizeof(ar)/sizeof(int), 0, 21, res);
    cout<<"value is "<<ret.value<<" index is "<<ret.index<<endl;

    ret = Rmg(ar, sizeof(ar)/sizeof(int), 0, 22, res);
    cout<<"value is "<<ret.value<<" index is "<<ret.index<<endl;
}

enum Direction
{
    LeftTop = 1,
    Left,
    Top,
};

//the index 0 of ta and tb is used as guard position
void ConstructLcsTable(string &s1, string &s2, int *a, int *b)
{
    int len1 = s1.length()+1;
    int len2 = s2.length()+1;

    for (int i=1; i<len1; i++)
        for(int j=1; j<len2; j++)
        {
            if (s1[i-1] == s2[j-1])
            {
                *(a+j*len1+i) = *(a+(j-1)*len1 +i-1) + 1;
                *(b+j*len1+i) = (int)LeftTop;
            }
            else if (*(a+(j-1)*len1+i) > *(a+j*len1+i-1))
            {
                *(a+j*len1+i) = *(a+(j-1)*len1+i);
                *(b+j*len1+i) = (int)Top;
            }
            else
            {
                *(a+j*len1+i) = *(a+j*len1+i-1);
                *(b+j*len1+i) = (int)Left;
            }
        }
}

string Lcs(string &s1, string &s2)
{
    int len1 = s1.length()+1;
    int len2 = s2.length()+1;
    int *ta = (int *)malloc(len1*len2*sizeof(int));
    int *tb = (int *)malloc(len1*len2*sizeof(int));
    memset(ta, 0, sizeof(int)*len1*len2);
    memset(tb, 0, sizeof(int)*len1*len2);
    ConstructLcsTable(s1, s2, ta, tb);
    
    string res = "";
    int s1_index = len1 - 1;
    int s2_index = len2 - 1;
    int d = 0;

    while((d = *(tb + s2_index * len1 + s1_index)))
    {
        if (d == LeftTop)
        {
            res = res + s1[s1_index-1];
            s1_index--;
            s2_index--;
        } 
        else if (d == Top)
        {
            s2_index--;
        }
        else
        {
            s1_index--;
        }
    }
    reverse(res.begin(), res.end());


    free(ta);
    free(tb);

    return res;
}

//only handle asic
int PreProcesHorspool(int move[], char pattern[])
{
    int pat_len = strlen(pattern);

    for (int i = 0; i < pat_len-1; ++i)
    {
        move[pattern[i]] = pat_len - 1 - i;
    }
    return 0;
}
int Horspool(char str[], char pattern[])
{
    int move[256] = {0};
    int pat_len = strlen(pattern);
    int str_len = strlen(str);
    if (str_len < pat_len)
        return -1;

    for (int i=0; i<256; i++)
        move[i] = pat_len;
    PreProcesHorspool(move, pattern);
    
    for (int pos = 0; pos < str_len - pat_len + 1; )
    {
        int offset = pat_len - 1;
        while(offset >= 0 && str[pos + offset] == pattern[offset])
            offset--;
        if (offset < 0)
        {
            return pos;
            //printf("str index %d\n", pos);
            //don't consider the overlap 
            //pos += pat_len;
        }
        else
        {
            pos += move[str[pos + pat_len - 1]];
        }
    }

    return 0;
}

//this this type of pre str match searching, like kmp
int PreProcessShiftAnd(int b[], char pattern[])
{
    return 0;
}
int ShiftAndStr(char str[], char pattern[])
{
    return 0;
}

#define WM_MAX_PATTERN_LENGTH 100
#define WM_CHAR_SET_SIZE 26
#define WM_SHIFT_ARRAY_SIZE 256

struct WM_Pattern_List
{
    char *pat;
    WM_Pattern_List *next;
};

//hash_size must be exp(2, x)
unsigned int Hash1WM(char str[], int len, unsigned int hash_size)
{
    unsigned int seed = 31; //31, 131, 1313, 13131, 131313
    unsigned int hash = 0;
    
    for (int i=0; i<len; i++)
    {
        hash = hash * seed + (unsigned char)str[i];
    }

    return (hash & (hash_size - 1));
}

//shift[] and hash[] must have already properly initialized
int PreProcessWM(char pattern[][WM_MAX_PATTERN_LENGTH], int pat_size, int b_size, int shift[], WM_Pattern_List hash_to_pat[])
{
    int pat_index = WM_SHIFT_ARRAY_SIZE;

    for (int i=0; i<pat_size; i++)
    {
        char *p = pattern[i];
        int p_len = strlen(p);
        int hash1 = 0;
        while(*p && (p_len = strlen(p)) >= b_size)
        {
            hash1 = Hash1WM(p, b_size, WM_SHIFT_ARRAY_SIZE);
            shift[hash1] = (shift[hash1] > (p_len - b_size) ? (p_len - b_size) : shift[hash1]);
            p++;
        }

        WM_Pattern_List *list = &hash_to_pat[hash1];
        if (list->pat == NULL)
        {
            list->pat = pattern[i];
            list->next = NULL;
        }
        else
        {
            while(list->next && list->pat != pattern[i])
            {
                list = list->next;
            }
            if (list->pat != pattern[i])
            {
                list->next = &hash_to_pat[pat_index++];
                list = list->next;
                list->pat = pattern[i];
                list->next = NULL;
            }
        }
    }
    return 0;
}

bool CheckMatch(char str[], char pat[])
{
    int i=0;
    int pat_len = strlen(pat);
    while(pat[i] && str[i] == pat[i])
    {
        i++;
    }

    if (pat[i])
        return false;
    else
        return true;
}
int WMMultiPattern(char str[], char pattern[][WM_MAX_PATTERN_LENGTH], int pattern_size)
{
    //preprecess
    int l_min = WM_MAX_PATTERN_LENGTH;
    int l_max = 0;
    for (int i=0; i<pattern_size; i++)
    {
        int pat_len = strlen(pattern[i]);
        l_min = (l_min > pat_len ? pat_len : l_min);
        l_max = (l_max < pat_len ? pat_len : l_max);
    }

    int b_size = log((double)2*l_min*pattern_size)/log((double)WM_CHAR_SET_SIZE);
    //just try
    b_size = 2;
    int shift[WM_SHIFT_ARRAY_SIZE] = {0};
    for (int i=0; i<WM_SHIFT_ARRAY_SIZE; i++)
    {
        shift[i] = l_min - b_size + 1;
    }
    WM_Pattern_List hash_to_pat[WM_SHIFT_ARRAY_SIZE * 2];
    memset(hash_to_pat, 0, sizeof(hash_to_pat));
    PreProcessWM(pattern, pattern_size, b_size, shift, hash_to_pat);

    //search match
    char *str_p = str + l_min - b_size;
    int str_len = strlen(str);
    char *str_end = str + str_len;
    while(*str_p && (str_end - str_p) >= b_size)
    {
        int hash = Hash1WM(str_p, b_size, WM_SHIFT_ARRAY_SIZE);
        assert(hash < WM_SHIFT_ARRAY_SIZE);

        int move = shift[hash];
        if (move == 0)
        {
            //this may be a match
            WM_Pattern_List *pat_list = &hash_to_pat[hash];
            while(pat_list)
            {
                int pat_len = strlen(pat_list->pat);
                char *str_beg = str_p - pat_len + b_size;
                if (CheckMatch(str_beg, pat_list->pat))
                {
                    printf("match one pattern : %s\n", pat_list->pat);
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

void StrMatchTest()
{
    char str[] = "The SAO/NASA particularly in the Astrophysics Data System (ADS) is a Digital Library portal for researchers in Astronomy and Physics, operated by the Smithsonian Astrophysical Observatory (SAO) under a NASA grant. The ADS maintains three bibliographic databases containing more than 9.0 million records: Astronomy and Astrophysics, Physics, and arXiv e-prints. The main body of data in the ADS consists of bibliographic records, which are searchable through highly customizable query forms, and full-text scans of much of the astronomical literature which can be browsed or searched via our full-text search interface. Integrated in its databases, the ADS provides access and pointers to a wealth of external resources, including electronic articles, data catalogs and archives. We currently have links to over 9.1 million records maintained by our collaborators. Please note that all abstracts and articles in the ADS are copyrighted by the publisher, and their use is free for personal use only. For more information, please read our page detailing the Terms and Conditions regulating the use of our resources. In addition to its databases, the ADS provides the myADS Update Service, a free custom notification service promoting current awareness of the recent technical literature in astronomy and physics based on each individual subscriber's queries. Every week the myADS Update Service will scan the literature added to the ADS since the last update, and will create custom lists of recent papers for each subscriber, formatted to allow quick reading and access. Subscribers are notified by e-mail in html format. As an option, users can elect to receive updates on preprints published on the arXiv e-print archive via daily emails or by subscribing to a custom RSS feed. The importance of ADS's role in supporting the scientific community has been recognized by societies and individuals. If you wish to cknowledge us in a publication, kindly use a phrase such as the following Following the re-establishment of Yugoslavia during World War II, Slovenia became part of Federal Yugoslavia. A socialist state was established, but because of the Tito-Stalin split in 1948, economic and personal freedoms were broader than in the Eastern Bloc. In 1947, Italy ceded most of the Julian March to Yugoslavia, and Slovenia thus regained the Slovenian Littoral. From the 1950s, Slovenia enjoyed a relatively wide autonomy.Between 1945 and 1948, a wave of political repressions took place in Slovenia and in Yugoslavia. By 1947, all private property had been nationalised. Between 1949 and 1953, a forced collectivisation was attempted. After its failure, a policy of gradual liberalisation was followed. A new economic policy, known as workers self-management started to be implemented under the advice and supervision of the main theorist of the Yugoslav Communist Party, the Slovene Edvard Kardelj. In 1956, Josip Broz Tito, together with other leaders, founded the Non-Aligned Movement.Slovenia's economy developed rapidly, particularly in the 1950s when the country was strongly industrialised. Despite restrictive economic and social legislation within Yugoslavia, Slovenia managed to preserve a high level of economic development with a skilled workforce, working discipline and organisation. After the economic reform and further economic decentralisation of Yugoslavia in 1965 and 1966 Slovenia was approaching a market economy. Its domestic product was 2.5 times the average, which strengthened national confidence among the Slovenes. After the death of Tito in 1980, the economic and political situation in Yugoslavia became very strained.[29] Political disputes around economic measures were echoed in the public sentiment, as many Slovenians felt they were being economically exploited, having to sustain an expensive and inefficient federal administration The Peregrine Falcon is a cosmopolitan bird of prey in the family Falconidae. The Peregrine reaches speeds of over 320 km/h (200 mph) during its characteristic hunting stoop, making it the fastest extant member of the animal kingdom. The world's most widespread bird of prey, it can be found nearly everywhere on Earth, except extreme polar regions, very high mountains, and most tropical rainforests; the only major ice-free landmass from which it is entirely absent is New Zealand. While its diet consists almost exclusively of medium-sized birds, the Peregrine Falcon will occasionally hunt small mammals, small reptiles or even insects. Reaching sexual maturity at one year, it mates for life and nests in a scrape, normally on cliff edges or, in recent times, on tall human-made structures. The Peregrine Falcon became an endangered species in many areas due to the use of pe";
    char pattern[] = "Slovenia was approaching a market economy. Its domestic product was 2.5 times the average";
    int ret = 0;
    printf("\n/***************************kmp string match****************************/\n");
	time_start();
    for (int i=0; i<400; i++)
        ret = kmp_str_match(pattern, str);
    printf("ret : %d, time %d", ret, get_time_used());

    //printf("\n/***************************suffix array****************************/\n");
    ////string sa_string = "aabcdefaaaba";
    //string sa_string1 ="yeshowmuchiloveyoumydearmotherreallyicannotbelieveit";
    //string sa_stirng2 = "yeaphowmuchiloveyoumydearmother";
    //string sa_string =  sa_string1 + sa_stirng2; 
    //int sa_int[100] = {0};
    //int height[100] = {0};
    //suffix_array(sa_string, sa_int, height);
    //for(int i=0; i<sizeof(sa_int)/sizeof(int); i++)
    //    cout<<sa_int[i]<<"\t";
    //cout<<endl;
    //for(int i=0; i<sizeof(height)/sizeof(int); i++)
    //    cout<<height[i]<<"\t";
    //cout<<endl;
    //printf("\n/**********************using suffix array to get huiwen***********************/\n");
    ////string huiwen = "kskalioejkolkakdnsowpabcdefghgfedcbakdlsieda";
    //string huiwen = "abcowabawcc";
    //int hui_sa[100] = {100};
    //int hui_height[100] = {100};
    //string re_huiwen = huiwen;
    //reverse(re_huiwen.begin(), re_huiwen.end());
    //suffix_array(huiwen + re_huiwen, hui_sa, hui_height);
    //int hui_max_index = 0;
    //int hui_max = 0;
    //for(int i=0; i<100; i++)
    //{
    //    if (hui_height[i] > hui_max)
    //    {
    //        hui_max = hui_height[i];
    //        hui_max_index = i;
    //    }
    //}
    //if (hui_sa[hui_max_index] < huiwen.length())
    //    cout<<"max hui wen is "<<hui_max<<" index is "<<hui_sa[hui_max_index]<<endl;
    //else
    //    cout<<"max hui wen is "<<hui_max<<" index is "<<2*huiwen.length() - hui_sa[hui_max_index] - hui_max<<endl;


    //printf("\n/***************************LCS****************************/\n");
    //string lcs_1 = "abceefas";
    //string lcs_2 = "aabcdaedaaefassdas";
    //string res = Lcs(lcs_1, lcs_2);
    //cout<<res<<endl;

    printf("\n/***************************Horspool****************************/\n");
	time_start();
    for (int i=0; i<400; i++)
        ret = Horspool(str, pattern);
    printf("ret : %d, time %d", ret, get_time_used());

    printf("\n/***************************Wu-Manber****************************/\n");
    char wm_pattern[][WM_MAX_PATTERN_LENGTH] = {"particularly in the", "normally on", "nationalised. ", "bibliographic databases", "Non-Aligned Movement", "Yugoslavia in 1965"};
    time_start();
    //for (int i=0; i<400; i++)
        ret = WMMultiPattern(str, wm_pattern, sizeof(wm_pattern)/sizeof(wm_pattern[0]));
    printf("ret : %d, time %d", ret, get_time_used());
}
