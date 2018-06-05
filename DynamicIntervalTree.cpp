#include "DynamicIntervalTree.h"
#include <stack>
#include <unordered_set>
#include <functional>
#include <algorithm>

using namespace std;

struct Hash {
  size_t operator()(const DynamicIntervalTree::Interval &interval) const {
    return interval.start;
  }
};

inline bool operator== (DynamicIntervalTree::Interval const& lhs, DynamicIntervalTree::Interval const& rhs)
{
  return (lhs.start == rhs.start) &&
         (lhs.end == rhs.end);
}

DynamicIntervalTree::DynamicIntervalTree() {
  root = NULL;
}

DynamicIntervalTree::~DynamicIntervalTree() {
  while (root != nullptr) {
    /* If root has no left subtree, just delete root. */
    if (root->left == nullptr) {
      Node* next = root->right;
      delete root;
      root = next;
    }
    /* Otherwise, root has left subtree. Do right rotation to move
       that child to the left. */
    else {
      Node* child = root->left;
      root->left = child->right;
      child->right = root;
      root = child;
    }
  }
}

/*
  Helper Function: newNode
  ========================
  Create a new node given the interval.
*/
DynamicIntervalTree::Node *
DynamicIntervalTree::newNode(Interval interval) {
  Skiplist<double, Interval> ascendingList;
  ReverseSkiplist<double, Interval> descendingList;
  ascendingList.insert (interval.start, interval);
  descendingList.insert (interval.end, interval);
  Node *temp = new Node;
  temp->center = (interval.start + interval.end)/2;
  temp->left = temp->right = NULL;
  temp->ascending = ascendingList;
  temp->descending = descendingList;
  temp->height = 1;
  return temp;
}

int DynamicIntervalTree::height(Node* node) {
  if (node == NULL) return 0;
  return node->height;
}


/*
  Helper Function: rightRotate
  ===========================
  Right Rotate root with y.
          y                         x
         /                           \
        x              =>             y
         \                           /
          T2                        T2
*/
DynamicIntervalTree::Node *
DynamicIntervalTree::rightRotate(Node *y) {
  Node *x = y->left;
  Node *T2 = x->right;

  // Perform Rotation
  x->right = y;
  y->left = T2;

  // Update heights
  y->height = max(height(y->left), height(y->right)) + 1;
  x->right = assimilateOverlappingIntervals(y, x);
  x->height = max(height(x->left), height(x->right)) + 1;

  // Add assimilate HERE!!!
  // assimilate(y, x); // update height of x

  return x;
}

/*
  Helper Function: leftRotate
  ===========================
  Right Rotate root with x.
          x                         y
           \                       /
            y              =>     x
           /                       \
          T2                       T2
*/
DynamicIntervalTree::Node *
DynamicIntervalTree::leftRotate(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;

  // Perform Rotation
  y->left = x;
  x->right = T2;

  // Update heights
  x->height = max(height(x->left), height(x->right)) + 1;
  y->left = assimilateOverlappingIntervals(x, y);
  y->height = max(height(y->left), height(y->right)) + 1;

  // Add assimilate HERE!!!
  // assimilate(x, y); // update height of y

  // Return the root node
  return y;
}

DynamicIntervalTree:: Node*
DynamicIntervalTree::assimilateOverlappingIntervals(Node *from, Node *to) {

  // Get overlapped elements
  vector<Interval> tmp;
  if (to->center < from->center) {
    for (Skiplist<double, Interval>::iterator itr = from->ascending.begin();
         itr != from->ascending.end(); ++itr) {
      if (itr->second.start > to->center) break;
      tmp.push_back (itr->second);
    }
  } else {
    for (ReverseSkiplist<double, Interval>::iterator itr = from->descending.begin();
         itr != from->descending.end(); ++itr) {
      if (itr->second.end < to->center) break;
      tmp.push_back(itr->second);
    }
  }

  // Remove elements from ascending and descending from 'from' and
  // Add them to 'to'
  for (int i = 0; i < tmp.size(); i++) {
    from->ascending.erase(tmp[i].start);
    from->descending.erase(tmp[i].end);
    to->ascending.insert(tmp[i].start, tmp[i]);
    to->descending.insert(tmp[i].end, tmp[i]);
  }

  if ((from->ascending).size() == 0) {
    return deleteNode(from);
  }

  return from;

}

DynamicIntervalTree::Node*
DynamicIntervalTree::deleteNode(Node *node) {

  if (node->left == NULL && node->right == NULL) {
    // free(node);
    return NULL;
  }

  if (node->left == NULL) {
    Node *returnNode = node->right;
    // free(node);
    return returnNode;
  }
  else {
    Node *n = node->left;
    stack<Node *> s;

    while (n->right != NULL) {
      s.push (n);
      n = n->right;
    }

    if (!s.empty()) {
      s.top()->right = n->left;
      n->left = node->left;
    }
    n->right = node->right;

    Node *newRoot = n;
    while (!s.empty()) {
      n = s.top();
      s.pop();
      if (!s.empty()) {
        s.top()->right = assimilateOverlappingIntervals(n, newRoot);
        s.top()->height = max(height(s.top()->left), height(s.top()->right)) + 1;
      }
      else {
        newRoot->left = assimilateOverlappingIntervals(n, newRoot);
        newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;
      }
    }
    newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;

    // Need TO FREE NODE
    // free(node);
    return balanceOut(newRoot);
  }
}

DynamicIntervalTree::Node*
DynamicIntervalTree::balanceOut(Node* node) {
  int balance = height(node->left) - height(node->right);
  Node *result;
  if (balance < -1) {
    if (height(node->right->left) > height(node->right->right)) {
      node->right = rightRotate(node->right);
    }
    result = leftRotate(node);
    if (root == node) root = result;
    return result;
  } else if (balance > 1) {
    if (height(node->left->right) > height(node->left->left)) {
      node->left = leftRotate(node->left);
    }
    result = rightRotate(node);
    if (root == node) root = result;
    return result;
  } else {
    return node;
  }
}

DynamicIntervalTree::Node*
DynamicIntervalTree::insertIntervalRecurse(Node *node, Interval interval) {
  if (node == NULL) {
    return newNode(interval);
  }

  if (interval.start <= node->center && node->center <= interval.end) {
    (node->ascending).insert(interval.start, interval);
    (node->descending).insert(interval.end, interval);
    return node;
  } else if (node->center > interval.end) {
    node->left = insertIntervalRecurse(node->left, interval);
    node->height = max(height(node->left), height(node->right)) + 1;
  } else {
    node->right = insertIntervalRecurse(node->right, interval);
    node->height = max(height(node->left), height(node->right)) + 1;
  }

  return balanceOut(node);
}

void DynamicIntervalTree::insertInterval(Interval interval) {
  int index = findStartIndex (interval.start, 0, combined_points.size() - 1);
  combined_points.insert (combined_points.begin () + index, make_pair(interval.start, interval));
  index = findEndIndex (interval.end, 0, combined_points.size() - 1);
  combined_points.insert (combined_points.begin() + index + 1, make_pair(interval.end, interval));

  if (root == NULL) {
    root = newNode(interval);
    return;
  }

  insertIntervalRecurse(root, interval);
}

DynamicIntervalTree::Node*
DynamicIntervalTree::removeIntervalRecurse(Node *node, Interval interval) {
  if (node == NULL) return NULL;
  if (interval.start <= node->center && node->center <= interval.end) {
    (node->descending).erase(interval.end);
    (node->ascending).erase(interval.start);
    if (node->ascending.size() == 0) {
      Node *result = deleteNode(node);
      if (node == root) root = result;
      return result;
    }
  } else if (interval.end < node->center) {
    node->left = removeIntervalRecurse(node->left, interval);
  } else {
    node->right = removeIntervalRecurse(node->right, interval);
  }
  return balanceOut(node);
}

void DynamicIntervalTree::removeInterval(Interval interval) {
  if (root == NULL) return;

  // Remove from the array of starts and ends
  int index = findStartIndex (interval.start, 0, combined_points.size () - 1);
  combined_points.erase(combined_points.begin() + index);
  index = findEndIndex (interval.end, 0, combined_points.size () - 1);
  combined_points.erase(combined_points.begin() + index);

  if (interval.start <= root->center && root->center <= interval.end) {
    (root->descending).erase(interval.end);
    (root->ascending).erase(interval.start);
    if (root->ascending.size() == 0) {
      root = deleteNode(root);
    }
  } else {
    removeIntervalRecurse(root, interval);
  }
}

void DynamicIntervalTree::pointQueryRecurse(Node *node, double point, vector<Interval> &result) {
  if (node == NULL) return;
  if (node->center >= point) {
    for (Skiplist<double, Interval>::iterator itr = (node->ascending).begin(); itr != (node->ascending).end(); ++itr) {
      if (itr->first > point) break;
      result.push_back(itr->second);
    }
    pointQueryRecurse(node->left, point, result);
  } else {
    for (ReverseSkiplist<double, Interval>::iterator itr = (node->descending).begin(); itr != (node->descending).end(); ++itr) {
      if (itr->first < point) break;
      result.push_back(itr->second);
    }
    pointQueryRecurse(node->right, point, result);
  }
}

vector<DynamicIntervalTree::Interval> DynamicIntervalTree::pointQuery(double point) {
  vector<Interval> result;
  if (root == NULL) return result;
  pointQueryRecurse(root, point, result);
  return result;
}

/* Looks for the index of the first element in a sorted array that is
   greater than or equal to start point */
int
DynamicIntervalTree::findStartIndex (double start, int left, int right)
{
  if (right > left) {
    int mid = left + (right - left)/2;
    int next = mid + 1;

    /* Go as far back as possible to reach first occurrence
       of element similar to start point */
    while (start == combined_points[mid].first) {
      mid -= 1;
      next -= 1;
    }

    // if (next >= combined_points.size()) return left;

    if (start >= combined_points[mid].first && start <= combined_points[next].first) {
      return next;
    }
    if (start < combined_points[mid].first) {
      return findStartIndex (start, left, mid);
    }
    if (start > combined_points[next].first) {
      return findStartIndex (start, next, right);
    }
  }

  return left;
}

/* Looks for the index of the last element of a sorted array
   that is less than or equal to end point */
int
DynamicIntervalTree::findEndIndex (double end, int left, int right)
{
  if (right > left) {
    int mid = left + (right - left)/2;
    int next = mid + 1;

    while (end == combined_points[next].first) {
      next += 1;
      mid += 1;
    }

    // if (next >= combined_points.size()) return left;

    if (end >= combined_points[mid].first && end <= combined_points[next].first) {
      return mid;
    }
    if (end < combined_points[mid].first) {
      return findEndIndex (end, left, mid);
    }
    if (end > combined_points[next].first) {
      return findEndIndex (end, next, right);
    }
  }

  return right;
}

vector<DynamicIntervalTree::Interval> DynamicIntervalTree::intervalQuery(Interval interval) {
  unordered_set<Interval, Hash> no_duplicates;
  vector<Interval> vec, result;
  int q_start = findStartIndex (interval.start, 0, combined_points.size () - 1);
  if (q_start == combined_points.size ()) return result;  // no intervals can contain this point
  int q_end = findEndIndex (interval.end, 0, combined_points.size () - 1);
  if (q_end == -1) return result;

  double query_point = (interval.start + interval.end) / 2;
  pointQueryRecurse (root, query_point, vec);

  for (int i=q_start; i <= q_end; i++) {
    no_duplicates.insert (combined_points[i].second);
  }

  for (int i = 0; i < vec.size(); i++) {
    no_duplicates.insert (vec[i]);
  }

  for (const Interval& i: no_duplicates) {
    result.push_back(i);
  }

  return result;
}

void DynamicIntervalTree::preOrderRecurse(Node *node) {
  if (node == NULL) {
    return;
  }

  cout << "=== Node ===" << endl;
  cout << "node height = " << node->height << endl;
  cout << "node center = " << node->center << endl;
  cout << "node ascending skiplist = ";

  for (Skiplist<double, Interval>::iterator itr = (node->ascending).begin(); itr != (node->ascending).end(); ++itr) {
    cout << "( " << itr->second.start << "," << itr->second.end << ") ->";
  }
  cout << endl;

  cout << "node descending skiplist = ";
  for (ReverseSkiplist<double, Interval>::iterator itr = (node->descending).begin(); itr != (node->descending).end(); ++itr) {
    cout << "( " << itr->second.start << "," << itr->second.end << ") ->";
  }
  cout << "END" << endl;

  if (node->left != NULL) cout << "left child node center = " << node->left->center << endl;
  if (node->right != NULL) cout << "right child node center = " << node->right->center << endl;

  if (node->left != NULL) preOrderRecurse(node->left);
  if (node->right != NULL) preOrderRecurse(node->right);
}

void DynamicIntervalTree::preOrder() {
  preOrderRecurse(root);
}

vector<pair<double, DynamicIntervalTree::Interval> > DynamicIntervalTree::getArray() {
  return combined_points;
}
