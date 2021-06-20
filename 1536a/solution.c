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

// ***** Solution ****************************************************

typedef struct Result Result;

Result* Result_create(int k);

void Result_destroy(Result* r);

void solve_step1(AvlTree** a, AvlTree** root, Result** result);

void solve_step2(AvlTree** a, AvlTree** b, Result** result);

struct Result {
  char* message;
  int k;
};

Result* Result_create(char* message, int k) {
  Result* r = (Result*)malloc(sizeof(Result));
  r->message = message;
  r->k = k;
  return r;
}

void Result_destroy(Result* r) { free(r); }

void solve_step1(AvlTree** a, AvlTree** root, Result** result) {
  if (result->k <= 300) {
    result->message = "NO";
    return;
  }
  if (*a == NULL) {
    return;
  }

  solve_step1(&(*a)->left, root, result);
  solve_step2(a, root, result);
  solve_step1(&(*a)->right, root, result);
}

void solve_step2(AvlTree** a, AvlTree** b, Result** result) {
  if (result->k <= 300) {
    result->message = "NO";
    return;
  }
  if (*b == NULL) {
    return;
  }

  solve_step2(a, &(*b)->left, result);
  int key = abs((*a)->key - (*b)->key);
  if (AvlTree_search(b, key) == NULL) {
    AvlTree_insert(&b, key);
    result->k += 1;
  }
  solve_step2(a, &(*b)->right, result);
}

int main() {
  int t;
  scanf("%d", &t);

  while (t--) {
    int n;
    scanf("%d", &n);

    int key;
    AvlTree* a = NULL;
    for (int i = 0; i < n - 1; i++) {
      scanf("%d ", &key);
      AvlTree_insert(&a, key);
    }
    scanf("%d", &key);
    AvlTree_insert(&a, key);

    Result* result = Result_create("YES", n);
    solve_step1(&a, &a, &result);

    printf("%s\n", result->message);
    if (result->message == "YES") {
      printf("%d\n", result->k);
      AvlTree_print_keys(a);
      printf("\n");
    }

    AvlTree_destroy(a);
  }

  return 0;
}

// void solve_step2(AvlTree** a, AvlTree** b, Result** result) {
//   if (*b == NULL) {
//     return;
//   }
//
//   solve_step2(a, &(*b)->left, result);
//   int key = abs((*a)->key - (*b)->key);
//   if (AvlTree_search(b, key) == NULL) {
//     AvlTree_insert(&b, key);
//   }
//   solve_step2(a, &(*b)->right, result);
// }

// printf("%d %d\n", (*a)->key, (*b)->key);

// void solve(AvlTree **a, int n, Result **result) {
//   int stack_capacity = AvlTree_get_height(*a);
//   AvlTree **stack = (AvlTree *)malloc(sizeof(AvlTree *));
//   int stack_size = 0;
//   while (stack_size != 0 || *a != NULL) {
//     if (*a != NULL) {
//       stack[stack_size++] = *a;
//       *a = (*a)->left;
//     } else {
//       *a = stack[--stack_size];
//
//       AvlTree *b = NULL;
//       int cap = AvlTree_get_height(*a);
//       int s[] while (stack_size[1] != 0 || *a != NULL) {
//         if (*a != NULL) {
//           stack[0][stack_size[1]++] = *a;
//           *a = (*a)->left;
//         } else {
//           *a = stack[--stack_size[1]];
//           //
//           *a = (*a)->right;
//         }
//       }
//
//       *a = (*a)->right;
//     }
//   }
// }
