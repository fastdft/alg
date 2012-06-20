#include <string>
#include "palindrome.h"

using namespace std;

void palindrome(const string &s, struct PaliInfo *info)
{
    unsigned int max_pos = 0, max_len = 0;
    unsigned int string_pos = 0;
    while(string_pos < s.length())
    {
		//palindrome have 2 cases: xyx, xyyx
        int l = string_pos - 1;
        int r = string_pos + 1;
        int len = 1;
        while(l >=0 && r<=s.length()-1)
        {
            if(s[l] == s[r])
            {
                l--;
                r++;
                len+=2;
            }
            else
                break;
        }
        if (len>max_len)
        {
            max_pos = l+1;
            max_len = len;
        }

        l = string_pos - 1;
        r = string_pos;
        len = 0;
        while(l>=0 && r<=s.length()-1)
        {
            if(s[l] == s[r])
            {
                l--;
                r++;
                len+=2;
            }
            else
                break;
        }
        if (len>max_len)
        {
            max_pos = l+1;
            max_len = len;
        }
        string_pos++;
    }

    info->pos = max_pos;
    info->len = max_len;
}
