#include <string>

using namespace std;

struct BwtEncoded
{
    string str;
    int index;
};

BwtEncoded bwt_encode(const string &str);
