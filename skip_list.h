#ifndef __SKIPLIST_H
 #define __SKIPLIST_H
 
 #define SKIPLIST_MAXLEVEL 8 
 
 typedef struct skiplistNode {
     double score;
     struct skiplistNode *backward;
     struct skiplistLevel {
         struct skiplistNode *forward;
     }level[];
 }skiplistNode;
 
 typedef struct skiplist {
     struct skiplistNode *header, *tail;
     unsigned long length;
     int level;
 }skiplist;
 
 #endif
