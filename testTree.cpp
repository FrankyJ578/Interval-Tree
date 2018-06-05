#include "CenteredIntervalTree.h"
#include <iostream>
#include <assert.h>
#include <algorithm>

void printVec (std::vector<std::pair<double, double> >& vec);
void bigTest ();

void
printVec (std::vector<std::pair<double, double> >& vec)
{
  std::cout << "---Printing Vector---" << std::endl;
  for (int i = 0; i < (int) vec.size (); i++) {
    std::cout << vec[i].first << " " << vec[i].second << std::endl;
  }
  std::cout << std::endl;
}

void
bigTest ()
{
  int numInsertElement = 10000;
  int numPointQueryElement = 10000;
  int numIntervalQueryElement = 10000;
  std::vector<std::pair<double, double> > intervals;
  std::pair<double, double> interval;
  std::vector<std::pair<double, double> > stored_intervals;
  std::unordered_set<int> result;
  double a, b;

  std::cout << "==========================" << std::endl;
  std::cout << "===== Automated Test =====" << std::endl;
  std::cout << "==========================" << std::endl;
  std::cout << "Number of elements inserted = " << numInsertElement << std::endl;
  std::cout << "Number of point queries = " << numPointQueryElement << std::endl;
  std::cout << "Number of interval queries = " << numIntervalQueryElement << std::endl;

  for (int i = 0; i < numInsertElement; i++) {
    a = (double) (rand () % 100001);
    b = (double) (rand () % 100001);
    interval.first = (a >= b) ? b : a;
    interval.second = (a >= b) ? a : b;
    intervals.push_back (interval);
  }

  CenteredIntervalTree cit (intervals);
  stored_intervals = cit.getStoredIntervalsCopy ();

  for (int i = 0; i < numPointQueryElement; i++) {
    a = (double) (rand () % 100001);
    result = cit.pointSearch (a);
    std::unordered_set<int> check;
    for (int j = 0; j < stored_intervals.size (); j++) {
      if (stored_intervals[j].first <= a && stored_intervals[j].second >= a) {
        check.insert (j);
      }
    }
    if (result != check) {
      std::cout << "Got an error with Point Searching Test." << std::endl;
      std::cout << "Result size: " << result.size () << std::endl;
      std::cout << "Check size: " << check.size () << std::endl;
      std::cout << std::endl;
    }
  }

  std::cout << "Point Query Test: PASS!!!" << std::endl;

  for (int i = 0; i < numIntervalQueryElement; i++) {
    a = (double) (rand () % 100001);
    b = (double) (rand () % 100001);
    interval.first = (a >= b) ? b : a;
    interval.second = (a >= b) ? a : b;
    result = cit.intervalSearch (interval);
    std::unordered_set<int> check;

    for (int j = 0; j < stored_intervals.size (); j++) {
      if (stored_intervals[j].first <= interval.first && stored_intervals[j].second >= interval.first) {
        check.insert (j);
      }
      else if (stored_intervals[j].first > interval.first && stored_intervals[j].first <= interval.second) {
        check.insert (j);
      }
    }

    if (result != check) {
      std::cout << "Got an error with Interval Searching Test." << std::endl;
    }
  }

  std::cout << "Interval Query All Test: PASS!!!" << std::endl;
}

int main () {
  bigTest ();
}
