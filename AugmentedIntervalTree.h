#ifndef Augmented_Interval_Tree_Included
#define Augmented_Interval_Tree_Included

#include <vector>
#include <iostream>
using namespace std;

/* Augmented Interval Tree */
class AugmentedIntervalTree
{
  public:

    struct Interval {
      double start, end;
    };

    /* Given a list of intervals, constructs a new interval tree holding
       these elements */
    explicit AugmentedIntervalTree (vector<Interval> &intervals);

    /* Destructor for the interval tree */
    ~AugmentedIntervalTree ();

    /* Return all the intervals in the tree that contain the requested point */
    vector<Interval> pointSearch (double point);

    /* Return all the intervals in the tree that overlap the requested interval */
    vector<Interval> intervalSearch (Interval interval);

    ~AugmentedIntervalTree();

  private:

    struct Node {
      Interval *interval;
      double max;
      Node *left, *right;
    };
}

#endif
