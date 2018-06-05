#include <algorithm>
#include <vector>
#include <iostream>
#include <random>
#include <assert.h>
#include "Timer.h"
using namespace std;

struct Interval {
  double start, end;
};

struct Node {
  Interval *interval;
  double max;
  int height;
  Node *left, *right;
};

/* Helper Function */
int maxInt(int a, int b) {
  if (a > b) return a;
  return b;
}

/* Helper Function */
double maxDouble(double a, double b) {
  if (a > b) return a;
  return b;
}

/*
  Helper Function: newNode
  ========================
  Create a new node given the interval.
*/
Node *newNode(Interval interval) {
  Node *temp = new Node;
  temp->interval = new Interval(interval);
  temp->max = interval.end;
  temp->left = temp->right = NULL;
  temp->height = 1;
  return temp;
}

/* Helper Function: getHeight */
int getHeight(Node *node) {
  if (node == NULL) return 0;
  return node->height;
}

/* Helper Function */
double getMax(Node *node) {
  if (node == NULL) return -1;
  return node->max;
}

/* Helper Function */
int getBalance(Node *node){
  if (node == NULL) return 0;
  return getHeight(node->left) - getHeight(node->right);
}

/* Helper Function */
bool isOverlap(Interval i1, Interval i2) {
  return i1.start <= i2.end && i2.start <= i1.end;
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
Node* rightRotate(Node *y) {
  Node *x = y->left;
  Node *T2 = x->right;

  // Perform Rotation
  x->right = y;
  y->left = T2;

  // Update heights
  y->height = maxInt(getHeight(y->left), getHeight(y->right)) + 1;
  x->height = maxInt(getHeight(x->left), getHeight(x->right)) + 1;

  // Update max
  y->max = maxDouble(maxDouble(getMax(y->left), getMax(y->right)), y->interval->end);
  x->max = maxDouble(maxDouble(getMax(x->left), getMax(x->right)), x->interval->end);

  // Return the root node
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
Node *leftRotate(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;

  // Perform Rotation
  y->left = x;
  x->right = T2;

  // Update heights
  x->height = maxInt(getHeight(x->left), getHeight(x->right)) + 1;
  y->height = maxInt(getHeight(y->left), getHeight(y->right)) + 1;

  // Update max
  x->max = maxDouble(maxDouble(getMax(x->left), getMax(x->right)), x->interval->end);
  y->max = maxDouble(maxDouble(getMax(y->left), getMax(y->right)), y->interval->end);

  // Return the root node
  return y;
}

/*
  Main Function: insert
  =====================
  Given an in interval and root node,
  insert a node to the augmented interval tree
*/
Node *insert(Node* node, Interval newInterval) {

  if (newInterval.start > newInterval.end) return NULL;

  // Perform normal BST insertion indexed by start value
  if (node == NULL) return newNode(newInterval);

  if (newInterval.start < node->interval->start) {
    node->left = insert(node->left, newInterval);
  } else {
    node->right = insert(node->right, newInterval);
  }

  // Update height and max
  node->height = 1 + maxInt(getHeight(node->left), getHeight(node->right));
  node->max = maxDouble(maxDouble(getMax(node->left), getMax(node->right)), node->interval->end);

  // Get the balance factor
  int balance = getBalance(node);

  // Balance the tree (4 possible cases)
  if (balance > 1 && newInterval.start < node->left->interval->start) {
    return rightRotate(node);
  }

  if (balance < -1 && newInterval.start >= node->right->interval->start) {
    return leftRotate(node);
  }

  if (balance > 1 && newInterval.start >= node->left->interval->start) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && newInterval.start < node->right->interval->start) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  // Return the unchange node
  return node;
}

/* Helper Function */
Node *minValueNode(Node* node) {
  Node *current = node;
  while (current->left != NULL)
    current = current->left;
  return current;
}

/*
  Main Function: deleteNode
  =========================
  Delete a node from the augmented interval tree
*/
Node *deleteNode(Node* node, Interval interval) {

  if (interval.start > interval.end) return NULL;

  // Perform normal BST deletion indexed by start value
  if (node == NULL) return node;

  // Recursion Case
  if (node->interval->start != interval.start || node->interval->end != interval.end) {
    if (interval.start < node->interval->start) {
      node->left = deleteNode(node->left, interval);
    } else {
      node->right = deleteNode(node->right, interval);
    }

  // Delete current node
  } else {

    // node with one child or no child
    if (node->left == NULL || node->right == NULL) {
      Node *temp = node->left ? node->left: node->right;
      if (temp == NULL) {
        temp = node;
        node = NULL;
      } else {
        *node = *temp;
      }
      free(temp);
    } else {
      Node* temp = minValueNode(node->right);
      node->interval = temp->interval;
      node->right = deleteNode(node->right, *(temp->interval));
    }
  }

  if (node == NULL) return node;

  // Update height and max
  node->height = 1 + maxInt(getHeight(node->left), getHeight(node->right));
  node->max = maxDouble(maxDouble(getMax(node->left), getMax(node->right)), node->interval->end);

  // Get the balance factor
  int balance = getBalance(node);

  if (balance > 1 && getBalance(node->left) >= 0) {
    return rightRotate(node);
  }

  if (balance > 1 && getBalance(node->left) < 0) {
    node->left =  leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && getBalance(node->right) <= 0) {
    return leftRotate(node);
  }

  if (balance < -1 && getBalance(node->right) > 0) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

/*
  Main Function: pointQuery
  =========================
  Return a node whose interval contain a query point
  Runtime: O(log n)
*/
Node* pointQuery(Node *node, double query) {
  if (node == NULL || node->max < query) return NULL;
  if (node->interval->start <= query && node->interval->end >= query) return node;
  if (node->left != NULL && node->left->max >= query) return pointQuery(node->left, query);
  return pointQuery(node->right, query);
}

/* Helper Function */
void pointQueryAllRecurse(Node *node, double query, vector<Node *> *queryResult) {
  if (node == NULL || node->max < query) return;
  if (node->interval->start <= query && node->interval->end >= query) {
    (*queryResult).push_back(node);
  }

  if (node->left != NULL && node->left->max >= query) {
    pointQueryAllRecurse(node->left, query, queryResult);
  }

  if (node->right != NULL && node->right->max >= query) {
    pointQueryAllRecurse(node->right, query, queryResult);
  }
};

/*
  Main Function: pointQueryAll
  ============================
  Return all nodes whose interval contain a query point
  Runtime: O(min{zlogn, n})
*/
vector<Node *> pointQueryAll(Node *node, double query) {
  vector<Node *> queryResult;
  pointQueryAllRecurse(node, query, &queryResult);
  return queryResult;
}

/*
  Main Function: intervalQuery
  ============================
  Return a node whow interval overlaps with a query interval
  Runtime: O(log n)
*/
Node* intervalQuery(Node *node, Interval query) {
  if (node == NULL || query.start > query.end) return NULL;
  if (isOverlap(*(node->interval), query)) return node;
  if (node->left != NULL && node->left->max >= query.start)
    return intervalQuery(node->left, query);
  return intervalQuery(node->right, query);
}

/* Helper Function */
void intervalQueryAllRecurse(Node *node, Interval query, vector<Node *> *queryResult) {
  if (node == NULL) return;
  if (isOverlap(*(node->interval), query)){
    (*queryResult).push_back(node);
  }

  if (node->left != NULL && node->left->max >= query.start) {
    intervalQueryAllRecurse(node->left, query, queryResult);
  }

  if (node->right != NULL && node->right->max >= query.start) {
    intervalQueryAllRecurse(node->right, query, queryResult);
  }
}

/*
  Main Function: intervalQueryAll
  ===============================
  Return all node whoses interval contain a query point
  Runtime: O(min{zlogn, n})
*/
vector<Node *> intervalQueryAll(Node *node, Interval query) {
  vector<Node *> queryResult;
  if (node == NULL || query.start > query.end) return queryResult;
  intervalQueryAllRecurse(node, query, &queryResult);
  return queryResult;
}

/* Helper Function (for testing) */
void preOrder (Node *root) {
  if (root != NULL) {
    cout << "Interval = (" << root->interval->start << "," << root->interval->end << "), ";
    cout << "Max = " << root->max << " ,";
    cout << "Height = " << root->height << endl;
    preOrder(root->left);
    preOrder(root->right);
  }
}


// ================================================
// ================================================
// ==================== TEST ======================
// ================================================
// ================================================


void smallTest() {

  cout << "=======================" << endl;
  cout << "===== MANUAL TEST =====" << endl;
  cout << "=======================" << endl;

  Interval intervals[] = {{9, 20}, {5, 30}, {10, 19}, {0, 20}, {6, 15}, {11, 40}, {0, 12}, {1, 7}, {2, 34}};
  int numElem = sizeof(intervals)/sizeof(intervals[0]);
  Node *root = NULL;
  for (int i = 0; i < numElem; i++) {
    root = insert(root, intervals[i]);
  }

  Interval deletedInterval = {10, 19};
  root = deleteNode(root, deletedInterval);

  Node *query1 = pointQuery(root, 100);
  assert(query1 == NULL);

  Node *query2 = pointQuery(root, 15);
  assert(query2->interval->start == 0 && query2->interval->end == 20);

  Node *query3 = pointQuery(root, 34);
  assert(query3->interval->start == 2 && query3->interval->end == 34);

  Node *query4 = pointQuery(root, 39);
  assert(query4->interval->start == 11 && query4->interval->end == 40);

  Node *query5 = pointQuery(root, -2);
  assert(query5 == NULL);

  cout << "Point Query Test: PASS!!!" << endl;

  Interval interval6 = {45, 100};
  Node *query6 = intervalQuery(root, interval6);
  assert(query6 == NULL);

  Interval interval7 = {10, 12};
  Node *query7 = intervalQuery(root, interval7);
  assert(isOverlap(interval7, *(query7->interval)));

  Interval interval8 = {32, 33};
  Node *query8 = intervalQuery(root, interval8);
  assert(isOverlap(interval8, *(query8->interval)));

  Interval interval9 = {-100, -3};
  Node *query9 = intervalQuery(root, interval9);
  assert(query9 == NULL);

  Interval interval10 = {5, 3};
  Node *query10 = intervalQuery(root, interval10);
  assert(query10 == NULL);

  cout << "Interval Query Test: PASS!!!" << endl;

  vector<Node *>query11 = pointQueryAll(root, 5);
  assert(query11.size() == 5);
  assert(query11[0]->interval->start == 1 && query11[0]->interval->end == 7);
  assert(query11[1]->interval->start == 0 && query11[1]->interval->end == 12);
  assert(query11[2]->interval->start == 0 && query11[2]->interval->end == 20);
  assert(query11[3]->interval->start == 5 && query11[3]->interval->end == 30);
  assert(query11[4]->interval->start == 2 && query11[4]->interval->end == 34);


  vector<Node *>query12 = pointQueryAll(root, 100);
  assert(query12.size() == 0);

  vector<Node *>query13 = pointQueryAll(root, -3);
  assert(query13.size() == 0);

  vector<Node *>query14 = pointQueryAll(root, 7);
  assert(query14.size() == 6);
  assert(query14[0]->interval->start == 1 && query14[0]->interval->end == 7);
  assert(query14[1]->interval->start == 0 && query14[1]->interval->end == 12);
  assert(query14[2]->interval->start == 0 && query14[2]->interval->end == 20);
  assert(query14[3]->interval->start == 5 && query14[3]->interval->end == 30);
  assert(query14[4]->interval->start == 2 && query14[4]->interval->end == 34);
  assert(query14[5]->interval->start == 6 && query14[5]->interval->end == 15);

  vector<Node *>query15 = pointQueryAll(root, 33);
  assert(query15.size() == 2);
  assert(query15[0]->interval->start == 2 && query15[0]->interval->end == 34);
  assert(query15[1]->interval->start == 11 && query15[1]->interval->end == 40);

  cout << "Point Query All Test: PASS!!!" << endl;

  Interval interval16 = {1,7};
  vector<Node *>query16 = intervalQueryAll(root, interval16);
  assert(query16.size() == 6);
  for (int i = 0; i < query16.size(); i++) {
    assert(isOverlap(*(query16[i]->interval), interval16));
  }

  Interval interval17 = {41, 42};
  vector<Node *>query17 = intervalQueryAll(root, interval17);
  assert(query17.size() == 0);

  Interval interval18 = {0, 0};
  vector<Node *>query18 = intervalQueryAll(root, interval18);
  assert(query18.size() == 2);
  for (int i = 0; i < query18.size(); i++) {
    assert(isOverlap(*(query18[i]->interval), interval18));
  }

  Interval interval19 = {0, 100};
  vector<Node *>query19 = intervalQueryAll(root, interval19);
  assert(query19.size() == 8);
  for (int i = 0; i < query19.size(); i++) {
    assert(isOverlap(*(query19[i]->interval), interval19));
  }

  Interval interval20 = {5, 3};
  vector<Node *>query20 = intervalQueryAll(root, interval20);
  assert(query20.size() == 0);

  cout << "Interval Query All Test: PASS!!!" << endl;
}


void test(int numIntervals) {
  Node *root = NULL;
  int numInsertElement = numIntervals;
  int numPointQueryElement = numIntervals;
  int numIntervalQueryElement = numIntervals;
  vector<Interval> intervals;
  Interval interval;
  Node *result;
  vector<Node *> results;
  double a, b;
  Timer insertTimer, deleteTimer, pointQueryTimer, intervalQueryTimer;

  cout << "==========================" << endl;
  cout << "===== Automated Test =====" << endl;
  cout << "==========================" << endl;
  cout << "Number of elements inserted = " << numInsertElement << endl;
  cout << "Number of point queries = " << numPointQueryElement << endl;
  cout << "Number of interval queries = " << numIntervalQueryElement << endl;

  for (int i = 0; i < numInsertElement; i++) {
    a = (double) (rand()%100001);
    b = (double) (rand()%100001);
    interval.start = (a >= b) ? b: a;
    interval.end = (a >= b) ? a : b;
    intervals.push_back(interval);

    insertTimer.start();
    root = insert(root, interval);
    insertTimer.stop();
  }

  cout << "Insert Timer = " << insertTimer.elapsed() / numInsertElement << endl;

  for (int i = 0; i < numPointQueryElement; i++) {
    a = (double) (rand()%100001);
    result = pointQuery(root, a);

    assert(!(result != NULL && ( result->interval->end < a || result->interval->start > a )));
    if (result == NULL) {
      for (int j = 0; j < intervals.size(); j++) {
        assert(intervals[j].start > a || intervals[j].end < a);
      }
    }
  }

  cout << "Point Query Test: PASS!!!" << endl;

  for (int i = 0; i < numIntervalQueryElement; i++) {
    a = (double) (rand()%100001);
    b = (double) (rand()%100001);
    interval.start = (a >= b) ? b: a;
    interval.end = (a >= b) ? a : b;
    result = intervalQuery(root, interval);

    assert(!(result != NULL && !isOverlap(*(result->interval), interval)));

    if (result == NULL) {
      for (int j = 0; i < intervals.size(); j++) {
        assert(!isOverlap(intervals[j], interval));
      }
    }
  }

  cout << "Interval Query Test: PASS!!!" << endl;

  for (int i = 0; i < numPointQueryElement; i++) {
    a = (double) (rand()%100001);
    pointQueryTimer.start ();
    results = pointQueryAll(root, a);
    pointQueryTimer.stop ();
    for (int j = 0; j < results.size(); j++) {
      assert(results[j]->interval->start <= a && results[j]->interval->end >= a);
    }

    int numResult = 0;
    for (int j = 0; j < intervals.size(); j++) {
      if (intervals[j].start <= a && intervals[j].end >= a) numResult++;
    }
    assert(results.size() == numResult);
  }

  cout << "Point Query Timer = " << pointQueryTimer.elapsed() / numPointQueryElement << endl;
  cout << "Point Query All Test: PASS!!!" << endl;

  for (int i = 0; i < numIntervalQueryElement; i++) {
    a = (double) (rand()%100001);
    b = (double) (rand()%100001);
    interval.start = (a >= b) ? b: a;
    interval.end = (a >= b) ? a : b;
    intervalQueryTimer.start();
    results = intervalQueryAll(root, interval);
    intervalQueryTimer.stop();

    for (int j = 0; j < results.size(); j++) {
      assert(isOverlap(interval, *(results[j]->interval)));
    }

    int numResult = 0;
    for (int j = 0; j < intervals.size(); j++) {
      if (isOverlap(intervals[j], interval)) numResult++;
    }
    assert(results.size() == numResult);
  }

  cout << "Interval Query Timer = " << intervalQueryTimer.elapsed() / numIntervalQueryElement << endl;
  cout << "Interval Query All Test: PASS!!!" << endl;

  for (int i = 0; i < numIntervals/10; i++) {
    int index = (int) (rand()%(intervals.size()-1));
    // std::cout << "Element to remove: " << intervals[index].start << " "
    //           << intervals[index].end << std::endl;
    interval = intervals[index];
    deleteTimer.start ();
    root = deleteNode (root, interval);
    deleteTimer.stop ();
    intervals.erase (intervals.begin() + index);
  }

  std::cout << "Delete Timer = " << deleteTimer.elapsed () / (numIntervals/10) << std::endl;
}

int main() {
  // smallTest();
  test(1000);
  test(10000);
  test(25000);
  return 0;
}
