#include "mt_tree.h"

#include <stdlib.h>
#include <assert.h>

//
// MtTreeNode
//

static MtTreeNode*
mt_tree_node_new(MtString* key, void* value)
{
  MtTreeNode* node = (MtTreeNode*) malloc(sizeof(MtTreeNode));
  assert(node != NULL);

  node->pair = mt_pair_new(key, value);

  node->right = NULL;
  node->left = NULL;

  node->color = RED;

  return node;
}

static MtTreeNode*
mt_tree_node_new_from_pair(MtPair* pair)
{
  assert(pair != NULL);

  MtTreeNode* node = (MtTreeNode*) malloc(sizeof(MtTreeNode));

  node->pair = pair;

  node->left = NULL;
  node->right = NULL;

  node->color = RED;

  return node;
}

static void
mt_tree_node_free(MtTreeNode* node)
{
  assert(node != NULL);

  if (node->left != NULL)
    mt_tree_node_free(node->left);

  if (node->right != NULL)
    mt_tree_node_free(node->right);

  mt_pair_free(node->pair);
  free(node);
}

static void
mt_tree_node_flip_color(MtTreeNode* node)
{
  assert(node != NULL);

  node->color = !node->color;

  if (node->left != NULL)
    node->left->color = !node->left->color;

  if (node->right != NULL)
    node->right->color = !node->right->color;
}

static bool
mt_tree_node_color(MtTreeNode* node)
{
  return ((node != NULL) && node->color);
}

static MtTreeNode*
mt_tree_node_rot_left(MtTreeNode* h)
{
  assert(h != NULL);
  assert(h->right != NULL);

  MtTreeNode* x = h->right;

  h->right = x->left;
  x->left = h;
  x->color = h->color;
  h->color = RED;

  return x;
}

static MtTreeNode*
mt_tree_node_rot_right(MtTreeNode* h)
{
  assert(h != NULL);
  assert(h->left != NULL);

  MtTreeNode* x = h->left;

  h->left = x->right;
  x->right = h;
  x->color = h->color;
  h->color = RED;

  return x;
}

static MtTreeNode*
mt_tree_node_redify_left(MtTreeNode* h)
{
  assert(h != NULL);

  mt_tree_node_flip_color(h);

  if ((h->right != NULL) && mt_tree_node_color(h->right->left))
  {
    h->right = mt_tree_node_rot_right(h->right);
    h = mt_tree_node_rot_left(h);
    mt_tree_node_flip_color(h);
  }

  return h;
}

static MtTreeNode*
mt_tree_node_redify_right(MtTreeNode* h)
{
  assert(h != NULL);

  mt_tree_node_flip_color(h);

  if ((h->left != NULL) && mt_tree_node_color(h->left->left))
  {
    h = mt_tree_node_rot_right(h);
    mt_tree_node_flip_color(h);
  }

  return h;
}

static MtTreeNode*
mt_tree_node_insert(MtTree* tree, MtTreeNode* node, MtString* key, void* value)
{
  assert(key != NULL);

  if (node == NULL)
  {
    ++tree->size;
    return mt_tree_node_new(key, value);
  }

  if (mt_tree_node_color(node->left) && mt_tree_node_color(node->right))
    mt_tree_node_flip_color(node);

  int cmp = mt_string_compare(node->pair->key, key);

  if (cmp == 0)     node->pair->value = value;
  else if (cmp < 0) node->left = mt_tree_node_insert(tree, node->left, key, value);
  else              node->right = mt_tree_node_insert(tree, node->right, key, value);

  if (mt_tree_node_color(node->right) && !mt_tree_node_color(node->left))
    mt_tree_node_rot_left(node);

  if (mt_tree_node_color(node->left) && mt_tree_node_color(node->left->left))
    mt_tree_node_rot_right(node);

  return node;
}

static MtTreeNode*
mt_tree_node_fixup(MtTreeNode* node)
{
  assert(node != NULL);

  if (mt_tree_node_color(node->right))
    node = mt_tree_node_rot_left(node);

  if (mt_tree_node_color(node->left) && mt_tree_node_color(node->left->left))
    node = mt_tree_node_rot_left(node);

  if (mt_tree_node_color(node->left))
    mt_tree_node_flip_color(node);

  return node;
}

static MtTreeNode*
mt_tree_node_remove_min(MtTree* tree, MtTreeNode* node)
{
  if (node->left == NULL)
  {
    --tree->size;
    mt_tree_node_free(node);
    return NULL;
  }

  if (!mt_tree_node_color(node->left) && !mt_tree_node_color(node->left->left))
    node = mt_tree_node_redify_left(node);

  node->left = mt_tree_node_remove_min(tree, node->left);

  return mt_tree_node_fixup(node);
}

static MtTreeNode*
mt_tree_node_find_min(MtTreeNode* node)
{
  while (node->left != NULL)
    node = node->left;

  return node;
}

static MtTreeNode* mt_tree_node_remove(MtTree* tree, MtTreeNode* node, MtString* key)
{
  assert(node != NULL);
  assert(key != NULL);

  if (mt_string_compare(key, node->pair->key))
  {
    if (node->left != NULL)
    {
      if (!mt_tree_node_color(node->left) && mt_tree_node_color(node->left->left))
        node = mt_tree_node_redify_left(node);

      node->left = mt_tree_node_remove(tree, node->left, key);
    }
  }
  else
  {
    if (mt_tree_node_color(node->left))
      node = mt_tree_node_rot_right(node);

    if (mt_string_compare(key, node->pair->key) && (node->right == NULL))
    {
      --tree->size;
      mt_tree_node_free(node);
      return NULL;
    }

    if (node->right != NULL)
    {
      if (!mt_tree_node_color(node->right) && !mt_tree_node_color(node->right->left))
        node = mt_tree_node_redify_right(node);

      if (mt_string_compare(key, node->pair->key) == 0)
      {
        node->pair->value = mt_tree_node_find_min(node->right)->pair->value;
        node->right = mt_tree_node_remove_min(tree, node->right);
      }
      else
      {
        node->right = mt_tree_node_remove(tree, node->right, key);
      }
    }
  }

  return mt_tree_node_fixup(node);
}

// Morris Traversal
// Source: http://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion-and-without-stack/
static void
mt_tree_node_traverse(MtTreeNode* root, bool(*fn)(MtPair*, void*), void* data)
{
  assert(root != NULL);

  MtTreeNode* current;
  MtTreeNode* pre;

  if (root == NULL)
    return;

  current = root;
  while(current != NULL)
  {
    if(current->left == NULL)
    {
      fn(current->pair, data);
      current = current->right;
    }
    else
    {
      // Find the in order predecessor of the current node
      pre = current->left;
      while(pre->right != NULL && pre->right != current)
      {
        pre = pre->right;
      }

      // Make the current node the right child of its in order predecessor
      if(pre->right == NULL)
      {
        pre->right = current;
        current = current->left;
      }
      // Restore the tree
      else
      {
        pre->right = NULL;
        fn(current->pair, data);
        current = current->right;
      }
    }
  }
}

static MtTreeNode* mt_tree_node_move_pair(MtTreeNode* node, MtPair* pair)
{
  assert(pair != NULL);

  if (node == NULL)
    return mt_tree_node_new_from_pair(pair);

  if (mt_tree_node_color(node->left) && mt_tree_node_color(node->right))
    mt_tree_node_flip_color(node);

  int cmp = mt_string_compare(node->pair->key, pair->key);
  if (cmp == 0)     assert(false); // We only move into empty trees
  else if (cmp < 0) node->left = mt_tree_node_move_pair(node->left, pair);
  else              node->right = mt_tree_node_move_pair(node->right, pair);

  if (mt_tree_node_color(node->right) && !mt_tree_node_color(node->left))
    node = mt_tree_node_rot_left(node);

  if (mt_tree_node_color(node->left) && mt_tree_node_color(node->left->left))
    node = mt_tree_node_rot_right(node);

  return node;
}

//
// MtTree
//

MtTree*
mt_tree_new()
{
  MtTree* tree = (MtTree *) malloc(sizeof(MtTree));
  assert(tree != NULL);

  tree->root = NULL;
  tree->size = 0;

  return tree;
}

MtPair*
mt_tree_search(MtTree* tree, MtString* key)
{
  assert(tree != NULL);
  assert(key != NULL);

  MtTreeNode* node = tree->root;
  int cmp = 0;

  while (node != NULL)
  {
    cmp = mt_string_compare(node->pair->key, key);

    if (cmp == 0)     return node->pair;
    else if (cmp < 0) node = node->left;
    else              node = node->right;
  }

  return NULL;
}

void
mt_tree_insert(MtTree* tree, MtString* key, void* value)
{
  assert(tree != NULL);
  assert(key != NULL);

  tree->root = mt_tree_node_insert(tree, tree->root, key, value);
  tree->root->color = BLACK;
}

void
mt_tree_remove(MtTree* tree, MtString* key)
{
  assert(tree != NULL);
  assert(key != NULL);

  if (tree->root != NULL)
  {
    tree->root = mt_tree_node_remove(tree, tree->root, key);

    if (tree->root != NULL)
      tree->root->color = BLACK;
  }
}

void
mt_tree_clear(MtTree* tree)
{
  assert(tree != NULL);

  if (tree->root != NULL)
    mt_tree_node_free(tree->root);

  tree->size = 0;
  tree->root = NULL;
}

void
mt_tree_free(MtTree* tree)
{
  assert(tree != NULL);

  mt_tree_clear(tree);
  free(tree);
}

void
mt_tree_traverse(MtTree* tree, bool(*fn)(MtPair*, void*), void* data)
{
  assert(tree != NULL);

  if (tree->root != NULL)
    mt_tree_node_traverse(tree->root, fn, data);
}


void
mt_tree_move_pair(MtTree* tree, MtPair* pair)
{
  assert(tree != NULL);
  assert(pair != NULL);

  tree->root = mt_tree_node_move_pair(tree->root, pair);
  tree->root->color = BLACK;
}
