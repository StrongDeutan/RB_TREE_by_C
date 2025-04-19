#include "rbtree.h"

#include <stdlib.h>

void right_rotate(rbtree* t, node_t* cur){
    node_t* left_child = cur->left;

    cur->left = left_child->right;
    if(left_child->right != t->nil) left_child->right->parent = cur;
    
    left_child->parent = cur->parent;
    if (cur->parent == t->nil) {
      t->root = left_child;
    }
    else if(cur == cur->parent->left){
        cur->parent->left = left_child;
    }
    else{
        cur->parent->right = left_child;
    }
    

    left_child->right = cur;
    cur->parent = left_child;

  
}

void left_rotate(rbtree* t, node_t* cur){
  node_t* right_child = cur->right;

  cur->right = right_child->left;
  if(right_child->left != t->nil) right_child->left->parent = cur;

  right_child->parent = cur->parent;

  if(cur->parent == t->nil){
    t->root = right_child;
  }
  else if(cur == cur->parent->left){
      cur->parent->left = right_child;
  }
  else{
      cur->parent->right = right_child;
  }
  

  right_child->left = cur;
  cur->parent = right_child;
}

void insert_bottom_up(rbtree* t, node_t* cur){
  node_t* uncle;
  node_t* par = cur->parent;
  node_t* g_par = par->parent;

  if(cur == t->root){
    cur->color = RBTREE_BLACK;
    return;
  }

  if(par->color == RBTREE_RED){
    uncle = (par == g_par->left) ? g_par->right : g_par->left;
    if(uncle->color == RBTREE_RED){
      uncle->color = par->color = RBTREE_BLACK;
      g_par->color = RBTREE_RED;
      insert_bottom_up(t, g_par);
      return;
    }
    else{                           // uncle's color is black
      if(par == g_par->left){
        if(cur == par->left){             // ll case
          par->color = RBTREE_BLACK;
          g_par->color = RBTREE_RED;
          right_rotate(t, g_par);
        }
        else{                             // lr case
          g_par->color = RBTREE_RED;
          cur->color = RBTREE_BLACK;
          left_rotate(t, par);
          right_rotate(t, g_par);
        }
      }
      else{
        if(cur == par->right){           // rr case
          par->color = RBTREE_BLACK;
          g_par->color = RBTREE_RED;
          left_rotate(t, g_par);
        }
        else{                             // rl case
          g_par->color = RBTREE_RED;
          cur->color = RBTREE_BLACK;
          right_rotate(t, par);
          left_rotate(t, g_par);
        }
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  
  p->nil = malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->parent = p->nil->left = p->nil->right = p->nil;
  p->root = p->nil;
  return p;

}

void free_node(rbtree* t, node_t* del){
  if(del == t->nil){
    return;
  }
  free_node(t, del->left);
  free_node(t, del->right);
  free(del);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free_node(t, t->root);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // printf("insertion key %i\n", key);
  // make new node
  node_t* new_node = malloc(sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->left = new_node->right = t->nil;
  new_node->parent = t->nil;

  // case empty tree
  if(t->root == t->nil){
    t->root = new_node;
    new_node->color = RBTREE_BLACK;      // root's color always be black
    return t->root;
  }
  else{
    node_t* cur = t->root;                 // search position
    node_t* par = NULL;
    while(cur != t->nil){
      if(key >= cur->key){
        par = cur;
        cur = cur->right;
      }
      else{
        par = cur;
        cur = cur->left;
      }
    }

    if(par->key < key){
      par->right = new_node;
      new_node->parent = par;
    }
    else{
      par->left = new_node;
      new_node->parent = par;
    }

    if(par->color == RBTREE_BLACK){
      return new_node;
    }
    else{       // recoloring or restructing
      insert_bottom_up(t, new_node);
      t->root->color = RBTREE_BLACK;
    }
  }

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* cur = t->root;
  
  while(cur != t->nil){
    if(cur->key == key){
      return cur;
    }
    if(cur->key < key){
      cur = cur->right;
    }
    else{
      cur = cur->left;
    }
  }

  if(cur == t->nil) return NULL;

  return cur;
  
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find'
  node_t* cur = t->root;
  if(cur == t->nil){
    return t->root;
  }
  node_t* re = NULL;
  while(cur != t->nil){
    re = cur;
    cur = cur->left;
  }
  return re;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* cur = t->root;
  if(cur == t->nil){
    return t->root;
  }
  node_t* re = NULL;
  while(cur != t->nil){
    re = cur;
    cur = cur->right;
  }
  return re;
}

void delete_bottom_up(rbtree* t, node_t* db){     // doubly black bottom up
  node_t* sibling;
  if(db == t->root){
    return;
  }
  if(db == db->parent->left){
    sibling = db->parent->right;
    if(sibling->color == RBTREE_RED){
      sibling->color = RBTREE_BLACK;
      sibling->parent->color = RBTREE_RED;
      left_rotate(t, sibling->parent);
      delete_bottom_up(t, db);
    }
    else{
      if(sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK){
        sibling->color = RBTREE_RED;
        if(sibling->parent->color == RBTREE_RED){
          sibling->parent->color = RBTREE_BLACK;
          return;
        }
        else{
          delete_bottom_up(t, sibling->parent);
          return;
        }
      }
      else if(sibling->left->color == RBTREE_RED){
        sibling->left->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        right_rotate(t, sibling);
        delete_bottom_up(t, db);
        return;
      }
      else{
        sibling->right->color = RBTREE_BLACK;
        sibling->color = sibling->parent->color;
        sibling->parent->color = RBTREE_BLACK;
        left_rotate(t, sibling->parent);
        return;        
      }
    }



  }
  else{
    sibling = db->parent->left;
    if(sibling->color == RBTREE_RED){
      sibling->color = RBTREE_BLACK;
      sibling->parent->color = RBTREE_RED;
      right_rotate(t, sibling->parent);
      delete_bottom_up(t, db);
    }
    else{
      if(sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK){
        sibling->color = RBTREE_RED;
        if(sibling->parent->color == RBTREE_RED){
          sibling->parent->color = RBTREE_BLACK;
        }
        else{
          delete_bottom_up(t, sibling->parent);
        }
      }
      else if(sibling->right->color == RBTREE_RED){
        sibling->right->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        left_rotate(t, sibling);
        delete_bottom_up(t, db);
        return;
      }
      else{
        sibling->left->color = RBTREE_BLACK;
        sibling->color = sibling->parent->color;
        sibling->parent->color = RBTREE_BLACK;
        right_rotate(t, sibling->parent);
        return;
      }
    }
  }



}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  if(p == t->root){
    // implement root node
    if(p->left == t->nil && p->right == t->nil){
      free(p);
      t->root = t->nil;
      return 0;
    }
  }
  color_t del_color;
  if(p->left == t->nil && p->right == t->nil){        // no child
    del_color = p->color;
    if(p == p->parent->left) p->parent->left = t->nil;
    else p->parent->right = t->nil;


    if(del_color == RBTREE_RED){
      free(p);
      return 0;
    }
    else{
      t->nil->parent = p->parent;
      delete_bottom_up(t, t->nil);
      free(p);
      return 0;
    }
  }
  else if(p->left == t->nil){                          // one child right
    del_color = p->color;
    p->right->parent = p->parent;
    if(p == t->root) t->root = p->right;
    if(p == p->parent->left){
      p->parent->left = p->right;
    }
    else{
      p->parent->right = p->right;
    }

    if(del_color == RBTREE_RED){
      free(p);
      return 0;
    }
    else{
      if(p->right->color == RBTREE_RED){
        p->right->color = RBTREE_BLACK;
        free(p);
        return 0;
      } 
      else{
        delete_bottom_up(t, p->right);
        free(p);
        return 0;
      }
    }
  }
  else if(p->right == t->nil){                          // one child left
    del_color = p->color;
    p->left->parent = p->parent;
    if(p == t->root) t->root = p->left;
    if(p == p->parent->left){
      p->parent->left = p->left;
    }
    else{
      p->parent->right = p->left;
    }

    if(del_color == RBTREE_RED){
      free(p);
      return 0;
    }
    else{
      if(p->left->color == RBTREE_RED){
        p->left->color = RBTREE_BLACK;
        free(p);
        return 0;
      } 
      else{
        delete_bottom_up(t, p->left);
        free(p);
        return 0;
      }
    } 

  }
  else{                                               // two child
    node_t* cur = p->right;
    node_t* del_node = NULL;
    while(cur != t->nil){
      del_node = cur;
      cur = cur->left;
    }

    // cur가 left인지 right인지 검증 로직 추가 해야 함
    
    int temp = p->key;
    p->key = del_node->key;
    del_node->key = temp;
    p = del_node;
    del_color = p->color;

    if(p->right == t->nil){
      if(p->color == RBTREE_RED){
        if(p == p->parent->left){
          p->parent->left = t->nil;
        }
        else{
          p->parent->right = t->nil;
        }
        
        free(p);
        return 0;
      }
      else{
        if(p == p->parent->left){
          p->parent->left = t->nil;
        }
        else{
          p->parent->right = t->nil;
        }
        t->nil->parent = p->parent;
        delete_bottom_up(t, t->nil);
        free(p);
        return 0;
      }
    }
    else{
      p->right->parent = p->parent;
      if(p == p->parent->left){
        p->parent->left = p->right;
      }
      else{
        p->parent->right = p->right;
      }
      
      if(del_color == RBTREE_RED){
        free(p);
        return 0;
      }
      else{
        if(p->right->color == RBTREE_RED){
          p->right->color = RBTREE_BLACK;
          free(p);
          return 0;
        }
        else{
          delete_bottom_up(t, p->right);
          free(p);
          return 0;
        }
      }
      
    }
  }


  return 0;
}


void to_array(const rbtree* t, key_t *arr, const size_t n,  node_t* cur, int* count){
  if(*count >= n) return;
  if(cur == t->nil) return;

  to_array(t, arr, n, cur->left, count);
  if(*count < n){
    arr[*count] = cur->key;
    *count += 1;
  } 
  to_array(t, arr, n, cur->right, count);

}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int count = 0;
  to_array(t, arr, n, t->root, &count);

  return 0;
}
