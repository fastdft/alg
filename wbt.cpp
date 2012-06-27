#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <string.h>

using namespace std;

struct BwtEncoded
{
    string str;
    int index;
};

BwtEncoded bwt_encode(const string &str)
{
    int str_size = str.size();
    vector<string> str_vec;
    string rolling_str = str;
    BwtEncoded bwt;

    assert(str_size < (1UL << 31));
    
    for (int i=0; i<str_size; i++)
    {
        str_vec.push_back(rolling_str);
        char first = rolling_str.at(0);
        rolling_str.erase(rolling_str.begin());
        rolling_str += first;
    }
    sort(str_vec.begin(), str_vec.end());
    vector<string>::iterator str_iter = find(str_vec.begin(), str_vec.end(), str);
    bwt.index = str_iter - str_vec.begin();

    bwt.str.clear();
    for (vector<string>::iterator iter = str_vec.begin(); iter != str_vec.end(); iter++)
    {
        bwt.str += iter->at(str_size - 1);
    }

    return bwt;
}

struct DecodeSortItem
{
    char first_two[3];
    int index;
};

bool DecodeItemCompare(DecodeSortItem a, DecodeSortItem b)
{
    if(strcmp(a.first_two, b.first_two) < 0)
        return true;
    else
        return false;
}

string bwt_decode(const BwtEncoded *bwt)
{
    string first = bwt->str;
    map<int, int> h_map;
    vector<DecodeSortItem> sort_items;
    char *ret_str = new char[first.size() + 1];

    assert(bwt->str.size() < (1UL << 31));

    //construct the first colomn
    sort(first.begin(), first.end());

    //construct the mapping from LFX to FXL
    for (int i=0; i<first.size(); i++)
    {
        DecodeSortItem item;
        memset(&item, 0, sizeof(DecodeSortItem));
        item.first_two[0] = bwt->str.at(i);
        item.first_two[1] = first.at(i);
        item.index = i;

        sort_items.push_back(item);
    }

    stable_sort(sort_items.begin(), sort_items.end(), DecodeItemCompare);

    for (int i=0; i<first.size(); i++)
    {
        h_map[sort_items[i].index] = i;
    }

    int m = bwt->index;
    for (int i=first.size() - 1; i>=0; i--)
    {
        ret_str[i] = bwt->str.at(m);
        m = h_map[m];
    }

    ret_str[first.size()] = 0;

    return ret_str;
}

int bwt_transform_test()
{
    char str[] = "The SAO/NASA particularly in the Astrophysics Data System (ADS) is a Digital Library portal for researchers in Astronomy and Physics, operated by the Smithsonian Astrophysical Observatory (SAO) under a NASA grant. The ADS maintains three bibliographic databases containing more than 9.0 million records: Astronomy and Astrophysics, Physics, and arXiv e-prints. The main body of data in the ADS consists of bibliographic records, which are searchable through highly customizable query forms, and full-text scans of much of the astronomical literature which can be browsed or searched via our full-text search interface. Integrated in its databases, the ADS provides access and pointers to a wealth of external resources, including electronic articles, data catalogs and archives. We currently have links to over 9.1 million records maintained by our collaborators. Please note that all abstracts and articles in the ADS are copyrighted by the publisher, and their use is free for personal use only. For more information, please read our page detailing the Terms and Conditions regulating the use of our resources. In addition to its databases, the ADS provides the myADS Update Service, a free custom notification service promoting current awareness of the recent technical literature in astronomy and physics based on each individual subscriber's queries. Every week the myADS Update Service will scan the literature added to the ADS since the last update, and will create custom lists of recent papers for each subscriber, formatted to allow quick reading and access. Subscribers are notified by e-mail in html format. As an option, users can elect to receive updates on preprints published on the arXiv e-print archive via daily emails or by subscribing to a custom RSS feed. The importance of ADS's role in supporting the scientific community has been recognized by societies and individuals. If you wish to cknowledge us in a publication, kindly use a phrase such as the following Following the re-establishment of Yugoslavia during World War II, Slovenia became part of Federal Yugoslavia. A socialist state was established, but because of the Tito-Stalin split in 1948, economic and personal freedoms were broader than in the Eastern Bloc. In 1947, Italy ceded most of the Julian March to Yugoslavia, and Slovenia thus regained the Slovenian Littoral. From the 1950s, Slovenia enjoyed a relatively wide autonomy.Between 1945 and 1948, a wave of political repressions took place in Slovenia and in Yugoslavia. By 1947, all private property had been nationalised. Between 1949 and 1953, a forced collectivisation was attempted. After its failure, a policy of gradual liberalisation was followed. A new economic policy, known as workers self-management started to be implemented under the advice and supervision of the main theorist of the Yugoslav Communist Party, the Slovene Edvard Kardelj. In 1956, Josip Broz Tito, together with other leaders, founded the Non-Aligned Movement.Slovenia's economy developed rapidly, particularly in the 1950s when the country was strongly industrialised. Despite restrictive economic and social legislation within Yugoslavia, Slovenia managed to preserve a high level of economic development with a skilled workforce, working discipline and organisation. After the economic reform and further economic decentralisation of Yugoslavia in 1965 and 1966 Slovenia was approaching a market economy. Its domestic product was 2.5 times the average, which strengthened national confidence among the Slovenes. After the death of Tito in 1980, the economic and political situation in Yugoslavia became very strained.[29] Political disputes around economic measures were echoed in the public sentiment, as many Slovenians felt they were being economically exploited, having to sustain an expensive and inefficient federal administration The Peregrine Falcon is a cosmopolitan bird of prey in the family Falconidae. The Peregrine reaches speeds of over 320 km/h (200 mph) during its characteristic hunting stoop, making it the fastest extant member of the animal kingdom. The world's most widespread bird of prey, it can be found nearly everywhere on Earth, except extreme polar regions, very high mountains, and most tropical rainforests; the only major ice-free landmass from which it is entirely absent is New Zealand. While its diet consists almost exclusively of medium-sized birds, the Peregrine Falcon will occasionally hunt small mammals, small reptiles or even insects. Reaching sexual maturity at one year, it mates for life and nests in a scrape, normally on cliff edges or, in recent times, on tall human-made structures. The Peregrine Falcon became an endangered species in many areas due to the use of pe";

    BwtEncoded encode = bwt_encode(str);
    string decode_str = bwt_decode(&encode);

    cout<<"encode_str:"<<endl<<encode.str<<endl;

    if (strcmp(decode_str.c_str(), str) == 0)
        cout<<"bwt transform success"<<endl;
    else
    {
        cout<<"bwt transform failed!"<<endl;
        cout<<"decode_str:"<<endl<<decode_str<<endl;
    }

}
