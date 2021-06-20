#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// ***** AVL Tree implementation *************************************

#define MAX(x, y) ((x) > (y) ? (x) : (y))

struct AvlTree {
  int key;
  int height;
  struct AvlTree* left;
  struct AvlTree* right;
};

AvlTree* AvlTree_create(int key) {
  AvlTree* node = (AvlTree*)malloc(sizeof(AvlTree));
  node->key = key;
  node->height = 1;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void AvlTree_destroy(AvlTree* node) {
  if (node == NULL) {
    return;
  }

  AvlTree_destroy(node->left);
  AvlTree_destroy(node->right);
  free(node);
}

int AvlTree_get_height(AvlTree* node) {
  if (node == NULL) {
    return 0;
  }

  return node->height;
}

static void AvlTree_update_height(AvlTree* node) {
  node->height =
      1 + MAX(AvlTree_get_height(node->left), AvlTree_get_height(node->right));
}

static void AvlTree_left_rotate(AvlTree** root) {
  AvlTree* old_root = *root;
  AvlTree* new_root = old_root->right;
  AvlTree* old_middle = new_root->left;

  old_root->right = old_middle;
  new_root->left = old_root;
  *root = new_root;

  AvlTree_update_height((*root)->left);
  AvlTree_update_height(*root);
}

static void AvlTree_right_rotate(AvlTree** root) {
  AvlTree* old_root = *root;
  AvlTree* new_root = old_root->left;
  AvlTree* old_middle = new_root->right;

  old_root->left = old_middle;
  new_root->right = old_root;
  *root = new_root;

  AvlTree_update_height((*root)->right);
  AvlTree_update_height(*root);
}

int AvlTree_get_balance(AvlTree* node) {
  if (node == NULL) {
    return 0;
  }

  return AvlTree_get_height(node->left) - AvlTree_get_height(node->right);
}

static void AvlTree_rebalance(AvlTree** root) {
  if (*root == NULL) {
    return;
  }

  int balance = AvlTree_get_balance(*root);
  if (balance > 1) {
    if (AvlTree_get_balance((*root)->left) >= 0) {
      AvlTree_right_rotate(root);
    } else {
      AvlTree_left_rotate(&(*root)->left);
      AvlTree_right_rotate(root);
    }
  } else if (balance < -1) {
    if (AvlTree_get_balance((*root)->right) <= 0) {
      AvlTree_left_rotate(root);
    } else {
      AvlTree_right_rotate(&(*root)->right);
      AvlTree_left_rotate(root);
    }
  }

  AvlTree_update_height(*root);
}

int AvlTree_get_min(AvlTree* node) {
  AvlTree* current = node;

  while (current->left != NULL) {
    current = current->left;
  }

  return current->key;
}

int AvlTree_get_max(AvlTree* node) {
  AvlTree* current = node;

  while (current->right != NULL) {
    current = current->right;
  }

  return current->key;
}

AvlTree* AvlTree_search(AvlTree* node, int key) {
  if (node == NULL || node->key == key) {
    return node;
  }

  if (key < node->key) {
    return AvlTree_search(node->left, key);
  }

  return AvlTree_search(node->right, key);
}

void AvlTree_insert(AvlTree** root, int key) {
  if (*root == NULL) {
    *root = AvlTree_create(key);
  } else if (key < (*root)->key) {
    AvlTree_insert(&(*root)->left, key);
    AvlTree_rebalance(root);
  } else if (key > (*root)->key) {
    AvlTree_insert(&(*root)->right, key);
    AvlTree_rebalance(root);
  }
}

void AvlTree_delete(AvlTree** root, int key) {
  if (*root == NULL) {
    return;
  } else if (key < (*root)->key) {
    AvlTree_delete(&(*root)->left, key);
  } else if (key > (*root)->key) {
    AvlTree_delete(&(*root)->right, key);
  } else {
    if ((*root)->left == NULL || (*root)->right == NULL) {
      AvlTree* old_root = *root;
      *root = (*root)->left ? (*root)->left : (*root)->right;
      free(old_root);
    } else {
      int min = AvlTree_get_min((*root)->right);
      (*root)->key = min;
      AvlTree_delete(&(*root)->right, min);
    }
  }

  AvlTree_rebalance(root);
}

void AvlTree_print_keys(AvlTree* node) {
  if (node == NULL) {
    return;
  }

  AvlTree_print_keys(node->left);
  printf("%d ", node->key);
  AvlTree_print_keys(node->right);
}

void AvlTree_sanity_check(AvlTree* node) {
  if (node == NULL) {
    return;
  }

  AvlTree_sanity_check(node->left);
  AvlTree_sanity_check(node->right);
  assert(node->height == 1 + MAX(AvlTree_get_height(node->left),
                                 AvlTree_get_height(node->right)));
}

// ***** Possible solutions ******************************************
//    Experiments and tests of possible ways
//    to better solve the problem.
// *******************************************************************

void AvlTree_in_order(AvlTree* node) {
  if (node == NULL) {
    return;
  }

  AvlTree_in_order(node->left);
  AvlTree_in_order(node->right);
}

void AvlTree_iterative_in_order(AvlTree* node) {
  int capacity = AvlTree_get_height(node);
  AvlTree* stack[capacity];
  int stack_size = 0;
  while (stack_size != 0 || node != NULL) {
    if (node != NULL) {
      stack[stack_size++] = node;  // stack.push
      node = node->left;
    } else {
      node = stack[--stack_size];  // stack.pop
      node = node->right;
    }
  }
}
