#ifndef MT_TREE_H
#define MT_TREE_H

#include <stddef.h>
#include <stdbool.h>

#include "mt_pair.h"

#define RED   true
#define BLACK false

typedef struct MtTreeNode
{
  MtPair* pair;
  struct MtTreeNode* left;
  struct MtTreeNode* right;
  struct MtTreeNode* parent;
  bool color;
} MtTreeNode;

typedef struct MtTree
{
  MtTreeNode* root;
  //size_t size;
} MtTree;

MtTree* mt_tree_new();

void mt_tree_insert(MtTree* tree, MtString* key, void* value);

MtPair* mt_tree_search(MtTree* tree, MtString* key);

void mt_tree_traverse(MtTree* tree, bool(*fn)(MtPair*, void*), void* data);

void mt_tree_move_pair(MtTree* tree, MtPair* pair);

void mt_tree_remove(MtTree* tree, MtString* key);

void mt_tree_clear(MtTree* tree);
void mt_tree_free(MtTree* tree);

#endif // MT_TREE_H
