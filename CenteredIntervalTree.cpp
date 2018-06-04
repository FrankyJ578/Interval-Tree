#include "CenteredIntervalTree.h"
#include <algorithm>
#include <iostream>

/* Comparator function for sorting containers with
   pair<T, T> elements */
bool
sortBySec (const std::pair<double, double>& a,
           const std::pair<double, double>& b)
{
  return a.second < b.second;
}

CenteredIntervalTree::CenteredIntervalTree (const std::vector<std::pair<double, double> >& intervals)
{
  std::vector<std::pair<double, double> > sorted_ends = intervals;
  /* Get vector of intervals sorted by end points */
  std::sort (sorted_ends.begin (), sorted_ends.end (), sortBySec);

  /* Store the vector of intervals */
  contained_intervals = sorted_ends;

  /* Create vector of start and end points */
  std::vector<std::tuple<double, double, int> > sorted_tuple_ends;
  for (unsigned i=0; i < contained_intervals.size (); i++) {
    double start = contained_intervals[i].first;
    double end = contained_intervals[i].second;
    combined_points.push_back (std::make_pair (start, i));
    combined_points.push_back (std::make_pair (end, i));
    sorted_tuple_ends.push_back (std::make_tuple (start, end, i));
  }
  std::sort (combined_points.begin (), combined_points.end ());

  root = buildTree (sorted_tuple_ends);
}

/* Returns a new node for the tree */
CenteredIntervalTree::Node*
CenteredIntervalTree::newNode(double key, Node *left, Node *right,
                              std::vector<std::tuple<double, double, int> >& sorted_starts,
                              std::vector<std::tuple<double, double, int> >& sorted_ends) {
  Node *temp = new Node;
  temp->key = key;
  temp->left = left;
  temp->right = right;
  temp->sorted_starts = sorted_starts;
  temp->sorted_ends = sorted_ends;
  return temp;
}

/* Builds the root node for the subtree.
   Takes the median of end values and uses that as the key.
   */
CenteredIntervalTree::Node*
CenteredIntervalTree::buildTree (std::vector<std::tuple<double, double, int> >& sorted_ends)
{
  if (sorted_ends.size () == 0) {
    return nullptr;
  }

  if (sorted_ends.size () == 1) {
    return newNode (std::get<1> (sorted_ends[0]), nullptr, nullptr, sorted_ends, sorted_ends);
  }

  unsigned median = sorted_ends.size ()/2;
  double key = std::get<1> (sorted_ends[median]);  // median

  /* Find the intervals that contain the key and separate from ones that don't */
  std::vector<std::tuple<double, double, int> > left_elems;
  std::vector<std::tuple<double, double, int> > right_elems;
  std::vector<std::tuple<double, double, int> > in_range_intervals;
  for (unsigned i = 0; i < sorted_ends.size (); i++) {
    double start = std::get<0> (sorted_ends[i]);
    double end = std::get<1> (sorted_ends[i]);
    if (start <= key && end >= key) {
      in_range_intervals.push_back (sorted_ends[i]);
      continue;
    }
    else if (start > key) right_elems.push_back (sorted_ends[i]);
    else if (end < key) left_elems.push_back (sorted_ends[i]);
  }

  /* Create the array sorted by start values */
  std::vector<std::tuple<double, double, int> > sorted_starts (in_range_intervals.begin (),
                                                               in_range_intervals.end ());
  std::sort (sorted_starts.begin (), sorted_starts.end ());

  return newNode (key, buildTree (left_elems), buildTree (right_elems),
                  sorted_starts, in_range_intervals);
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

/* Helper function for performing a point query */
void
CenteredIntervalTree::pointSearchHelper (Node* rootNode, double point, std::unordered_set<int>& intervals)
{
  if (rootNode == nullptr) return;
  /* All the intervals at this level include the point,
     by construction */
  if (point == rootNode->key) {
    for (int i = 0; i < (int) rootNode->sorted_ends.size (); i++) {
      intervals.insert (std::get<2> (rootNode->sorted_ends[i]));
    }
    return;
  }

  /* Case when should go through start points until
     reach start point less than point, add all intervals
     in this node with start points less than point and then
     go search left node */
  if (point < rootNode->key) {
    for (int i = 0; i < (int) rootNode->sorted_starts.size (); i++) {
      if (std::get<0> (rootNode->sorted_starts[i]) <= point) {
        intervals.insert (std::get<2> (rootNode->sorted_starts[i]));
      }
      else {
        break;
      }
    }
    pointSearchHelper (rootNode->left, point, intervals);
  }

  /* Case when should go through end points until
     reach end point more than point, add all intervals
     in this node with end points less than point and
     then go search right node */
  if (point > rootNode->key) {
    for (int i = (int) rootNode->sorted_ends.size () - 1; i >= 0; i--) {
      if (std::get<1> (rootNode->sorted_ends[i]) >= point) {
        intervals.insert (std::get<2> (rootNode->sorted_ends[i]));
      }
      else {
        break;
      }
    }
    pointSearchHelper (rootNode->right, point, intervals);
  }
}

std::unordered_set<int>
CenteredIntervalTree::pointSearch (double point)
{
  std::unordered_set<int> intervals;
  pointSearchHelper (root, point, intervals);
  return intervals;
}

/* Looks for the index of the first element in a sorted array that is
   greater than or equal to start point */
int
CenteredIntervalTree::findStartIndex (double start, int left, int right)
{
  if (right >= left) {
    int mid = left + (right - left)/2;
    int next = mid + 1;

    /* Go as far back as possible to reach first occurrence
       of element similar to start point */
    while (start == combined_points[mid].first) {
      mid -= 1;
      next -= 1;
    }

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
CenteredIntervalTree::findEndIndex (double end, int left, int right)
{
  if (right >= left) {
    int mid = left + (right - left)/2;
    int next = mid + 1;

    while (end == combined_points[next].first) {
      next += 1;
      mid += 1;
    }

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

std::unordered_set<int>
CenteredIntervalTree::intervalSearch (std::pair<double, double> interval)
{
  std::unordered_set<int> overlaps;
  int q_start = findStartIndex (interval.first, 0, combined_points.size () - 1);
  if (q_start == combined_points.size ()) return overlaps;  // no intervals can contain this point
  int q_end = findEndIndex (interval.second, 0, combined_points.size () - 1);
  if (q_end == -1) return overlaps; // no intervals can contain this point

  /* Pick a point inside of the requested interval.
     Perform point query on that chosen point. */
  double query_point = (interval.first + interval.second) / 2;
  pointSearchHelper (root, query_point, overlaps);

  for (int i=q_start; i <= q_end; i++) {
    overlaps.insert (combined_points[i].second);
  }

  return overlaps;
}

/* Returns the intervals corresponding to the indices provided by
   the unordered set 'overlaps'. */
std::vector<std::pair<double, double> >
CenteredIntervalTree::returnIntervals (std::unordered_set<int>& overlaps)
{
  std::vector<std::pair<double, double> > intervals;
  for (const int& index: overlaps) {
    intervals.push_back (contained_intervals[index]);
  }

  return intervals;
}

void
CenteredIntervalTree::printTree (void)
{
  traverse (root->left);
  std::cout << root->key << std::endl;
  // std::cout << "---Printing First Node---" << std::endl;
  // printTupleVec (root->sorted_starts);
  printTupleVec (root->sorted_ends);
  traverse (root->right);
}

void
CenteredIntervalTree::traverse (Node* rootNode)
{
  if (rootNode == nullptr) {
    return;
  }

  traverse (rootNode->left);
  std::cout << rootNode->key << std::endl;
  // printTupleVec (rootNode->sorted_starts);
  printTupleVec (rootNode->sorted_ends);
  traverse (rootNode->right);
}

void
CenteredIntervalTree::printTupleVec (std::vector<std::tuple<double, double, int> >& vec)
{
  std::cout << "---Printing Vector---" << std::endl;
  for (int i = 0; i < (int) vec.size (); i++) {
    std::cout << std::get<0> (vec[i]) << " " << std::get<1> (vec[i])
              << " " << std::get<2> (vec[i]) << std::endl;
  }
  std::cout << std::endl;
}

void
CenteredIntervalTree::printPairVec (std::vector<std::pair<double, double> >& vec)
{
  std::cout << "---Printing Vector---" << std::endl;
  for (int i = 0; i < (int) vec.size (); i++) {
    std::cout << vec[i].first << " " << vec[i].second << std::endl;
  }
  std::cout << std::endl;
}

std::vector<std::pair<double, double> >
CenteredIntervalTree::getStoredIntervalsCopy (void)
{
  return contained_intervals;
}
