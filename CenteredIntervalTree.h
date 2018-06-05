#ifndef Centered_Interval_Tree_Included
#define Centered_Interval_Tree_Included

#include <vector>
#include <unordered_set>
#include <utility>        /* For std::pair */
#include <tuple>
#include <string>

/* Static Centered Interval Tree with support for query, no insertion or deletion */
class CenteredIntervalTree
{
  public:
    /* Given a list of intervals, constructs a new interval tree holding
       these elements */
    CenteredIntervalTree (const std::vector<std::pair<double, double> >& intervals);

    /* Destructor for the interval tree */
    ~CenteredIntervalTree ();

    /* Return all the intervals in the tree that contain the requested point */
    std::unordered_set<int> pointSearch (double point);

    /* Return all the intervals in the tree that overlap the requested interval */
    std::unordered_set<int> intervalSearch (std::pair<double, double> interval);

    std::vector<std::pair<double, double> > returnIntervals (std::unordered_set<int>& overlaps);

    std::vector<std::pair<double, double> > getStoredIntervalsCopy (void);

    void printTree (void);

    static std::string name () {
      return "Centered Interval Tree";
    }

  private:
    struct Node {
      double key;
      Node *left;
      Node *right;
      std::vector<std::tuple<double, double, int> > sorted_starts;
      std::vector<std::tuple<double, double, int> > sorted_ends;
    };

    Node* root; /* Root of interval tree */

    /* Helper Functions */
    Node* buildTree (std::vector<std::tuple<double, double, int> >& sorted_ends);
    Node* newNode (double key, Node* left, Node* right,
                   std::vector<std::tuple<double, double, int> >& sorted_starts,
                   std::vector<std::tuple<double, double, int> >& sorted_ends);
    void pointSearchHelper (Node* rootNode, double point, std::unordered_set<int>& intervals);
    int findStartIndex (double start, int left, int right);
    int findEndIndex (double end, int left, int right);
    void traverse (Node* rootNode);
    void printTupleVec (std::vector<std::tuple<double, double, int> >& vec);
    void printPairVec (std::vector<std::pair<double, double> >& vec);

    /* Set of intervals used to construct interval tree */
    std::vector<std::pair<double, double> > contained_intervals;
    std::vector<std::pair<double, double> > combined_points;
};

#endif
