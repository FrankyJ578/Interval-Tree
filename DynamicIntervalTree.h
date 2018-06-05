#ifndef Dynamic_Interval_Tree_Included
#define Dynamic_Interval_Tree_Included

#include "Skiplist.h"
#include "ReverseSkiplist.h"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class DynamicIntervalTree {

  public:
    struct Interval {
      double start, end;
    };

    struct Node {
      double center;
      Skiplist<double, Interval> ascending;
      ReverseSkiplist<double, Interval> descending;
      Node *left, *right;
      int height;
    };

    DynamicIntervalTree();

    ~DynamicIntervalTree();

    vector<Interval> pointQuery(double point);

    vector<Interval> intervalQuery(Interval interval);

    void insertInterval(Interval interval);

    void removeInterval(Interval interval);

    void preOrder();

    vector<std::pair<double, Interval> > getArray(void);

  private:

    Node *root;
    vector<std::pair<double, Interval> > combined_points;
    vector<Interval> startIntervals;
    vector<Interval> endIntervals;

    Node *newNode(Interval interval);

    int height(Node *node);

    Node *rightRotate(Node *node);

    Node *leftRotate(Node *node);

    Node *assimilateOverlappingIntervals(Node *From, Node *To);

    Node *deleteNode (Node *node);

    Node *balanceOut (Node *node);

    Node *insertIntervalRecurse(Node *node, Interval interval);

    Node *removeIntervalRecurse(Node *node, Interval interval);

    void pointQueryRecurse(Node *node, double point, vector<Interval> &result);

    void preOrderRecurse(Node *node);

    int findStartIndex(double start, int left, int right);

    int findEndIndex(double end, int left, int right);

};

#endif
