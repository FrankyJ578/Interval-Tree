#include "CenteredIntervalTree.h"
#include <algorithm>

bool sortBySec (const pair<double, double>& a,
                const pair<double, double>& b)
{
  return a.second < b.second;
}

CenteredIntervalTree::CenteredIntervalTree (const std::vector<std::pair<double, double>>& intervals)
{
  // starts = intervals;
  sorted_ends = intervals;
  // contained_intervals = intervals;

  /* Create vectors of start and end points */
  // std::vector<std::pair<double, double>> starts;
  // std::vector<std::pair<double, double>> ends;
  // for (int i=0; i < (int) contained_intervals.size (); i++) {
  //   double start = contained_intervals[i].first;
  //   double end = contained_intervals[i].second;
  //   starts.push_back (std::make_pair (start, i));
  //   ends.push_back (std::make_pair (end, i));
  // }
  // std::sort (starts.begin (), starts.end ());
  std::sort (sorted_ends.begin (), sorted_ends.end (), sortBySec);

  root = buildTree (sorted_ends);
}

/* Builds the root node for the subtree.
   Takes the median of end values and uses that as the key.
   */
CenteredIntervalTree::Node*
CenteredIntervalTree::buildTree (std::vector<std::pair<double, double>>& sorted_ends)
{
  if (sorted_ends.size () == 0) {
    return nullptr;
  }

  if (sorted_ends.size () == 1) {
    return new Node {
      sorted_ends[0].second;
      nullptr;
      nullptr;
      sorted_ends;
      sorted_ends
    };
  }

  int median = sorted_ends.size ()/2;
  double key = sorted_ends[median].second;  // median

  std::vector<std::pair<double, double>> left_elems (sorted_ends.begin (),
                                                     sorted_ends.begin () + median - 1);

  /* Find the intervals that contain the key and separate from ones that don't */
  std::vector<std::pair<double, double>> right_elems;
  std::vector<std::pair<double, double>> in_range_intervals;
  for (int i = median; i < (int) sorted_ends.size (); i++) {
    double start = sorted_ends[i].first;
    double end = sorted_ends[i].second;
    if (start <= key) {
      in_range_intervals.push_back (sorted_ends[i]);
      continue;
    }
    right_elems.push_back (sorted_ends[i]);
  }

  /* Create the sorted array by start values */
  std::vector<std::pair<double, double>> sorted_starts (in_range_intervals.begin (),
                                                        in_range_intervals.end ());
  std::sort (sorted_starts.begin (), sorted_starts.end ());

  return new Node {
    key,
    buildTree (left_elems);
    buildTree (right_elems);
    sorted_starts;
    in_range_intervals
  };
}

CenteredIntervalTree::~CenteredIntervalTree () {
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

std::unordered_set<std::pair<double, double>>
CenteredIntervalTree::pointSearch (double point)
{

}

std::unordered_set<std::pair<double, double>>
CenteredIntervalTree::intervalSearch (std::pair<double, double> interval)
{

}
