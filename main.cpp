#include <iostream>
#include <string>
#include <time.h>
#include "sort.h"
#include "priqueue.h"
#include "recursive.h"
#include "palindrome.h"
#include "tree.h"
#include "interval_tree.h"
#include "utils.h"
#include "time_statistics.h"
#include "numbers.h"

using namespace std;
#define L_TEST_SIZE  10

void SBTreeTest();
void dpTest();
void StateMachine();
void StrMatchTest();
void PackageTest();
void LlrbTest();
int bwt_transform_test();
void number_test();
void heap_test();
void number_trick_test();

int main(int argc, char *argv[])
{
    //SortTest();

    //printf("\n/**************************hanoi recursive***************************/\n");
    //hanoi(3, -1);
    //struct KnapItem items[] = {{10, 2}, {16, 3}, {19, 4}, {25, 7}, {30, 9}};

    // printf("\n/**************************knap recursive***************************/\n");
	// time_start();
    //int max = knap(items, sizeof(items)/sizeof(struct KnapItem), 40);
    //printf("\nthe max value can take is %d, time is %d\n", max, get_time_used());

	//time_start();
    //max = dynamic_knap(items, sizeof(items)/sizeof(struct KnapItem), 40);
    //end = GetTickCount();
    //printf("\nthe max value can take is %d, time is %d\n", max, get_time_used());

    //printf("\n/**************************palindrome*****************************/\n");
    //struct PaliInfo pali_info;
    //const string pali_str = "abdksldkdladkksllskk";
    //palindrome(pali_str, &pali_info);
    //printf("pali: %s pos: %d,len %d\n", pali_str.c_str(), pali_info.pos, pali_info.len);

    //printf("\n/**************************tree*****************************/\n");
    //test_traverse();

    //printf("\n/**************************RMG test*****************************/\n");
    //RmqTest();

    //printf("\n/**************************interval tree test*****************************/\n");
    //IntervalTreeTest();

    //printf("\n/**************************sb tree test*****************************/\n");
    //SBTreeTest();
    //StateMachine();

    //printf("\n/**************************string algorithm test*****************************/\n");
    //StrMatchTest();

    //printf("\n/**************************package algorithm test*****************************/\n");
    //PackageTest();

    //printf("\n/**************************left leaning red-black tree algorithm test*****************************/\n");
    //LlrbTest();

    //printf("\n/**************************utils test*****************************/\n");
    //UtilsTest();
    //bwt_transform_test();

    //printf("\n/*************************numbers test*****************************/\n");
    //number_test();

    //printf("\n/*************************heap test*****************************/\n");
    //heap_test();
    //printf("\n/*************************dp test*****************************/\n");
    //dpTest();
    printf("\n/*************************number trick test*****************************/\n");
    number_trick_test();
}
