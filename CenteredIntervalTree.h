#ifndef Centered_Interval_Tree_Included
#define Centered_Interval_Tree_Included

#include <vector>
#include <unordered_set>
#include <utility>        /* For std::pair */

/* Static Centered Interval Tree with support for query, no insertion or deletion */
class CenteredIntervalTree
{
  public:
    /* Given a list of intervals, constructs a new interval tree holding
       these elements */
    explicit CenteredIntervalTree (const std::vector<std::pair<double, double>>& intervals);

    /* Destructor for the interval tree */
    ~CenteredIntervalTree ();

    /* Return all the intervals in the tree that contain the requested point */
    std::unordered_set<std::pair<double, double>> pointSearch (double point);

    /* Return all the intervals in the tree that overlap the requested interval */
    std::unordered_set<std::pair<double, double>> intervalSearch (std::pair<double, double> interval);

    static std::string name () {
      return "Centered Interval Tree";
    }

  private:
    struct Node {
      double key;
      Node *left;
      Node *right;
      std::vector<std::pair<double, double>> sorted_starts;
      std::vector<std::pair<double, double>> sorted_ends;
    };

    /* Static Helper Functions */
    static Node* buildTree (std::vector<std::pair<double, double>>& starts,
                            std::vector<std::pair<double, double>>& ends);

    Node* root; /* Root of interval tree */

    /* Set of intervals used to construct interval tree */
    std::vector<std::pair<double, double>> contained_intervals;
}

#endif
