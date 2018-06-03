#include "CenteredIntervalTree.h"
#include <algorithm>

CenteredIntervalTree::CenteredIntervalTree (const std::vector<std::pair<double, double>>& intervals)
{
  contained_intervals = intervals;

  /* Create vectors of start and end points */
  std::vector<std::pair<double, double>> starts;
  std::vector<std::pair<double, double>> ends;
  for (int i=0; i < (int) contained_intervals.size (); i++) {
    double start = contained_intervals[i].first;
    double end = contained_intervals[i].second;
    std::pair<double, double> start_pair = std::make_pair (start, i);
    std::pair<double, double> end_pair = std::make_pair (end, i);
    starts.push_back (start_pair);
    ends.push_back (end_pair);
  }
  std::sort (starts.begin (), starts.end ());
  std::sort (ends.begin (), ends.end ());
  root = buildTree (intervals);
}
