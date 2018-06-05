#include "DynamicIntervalTree.h"
#include <unordered_set>
#include <algorithm>
#include "Timer.h"

using namespace std;

bool isOverlap(DynamicIntervalTree::Interval i1, DynamicIntervalTree::Interval i2) {
  return i1.start <= i2.end && i2.start <= i1.end;
}

inline bool operator== (DynamicIntervalTree::Interval const& lhs, DynamicIntervalTree::Interval const& rhs)
{
  return (lhs.start == rhs.start) &&
         (lhs.end == rhs.end);
}

void test(int numIntervals) {
  int numInsertElement = numIntervals;
  int numPointQueryElement = numIntervals;
  int numIntervalQueryElement = numIntervals;
  vector<DynamicIntervalTree::Interval> intervals;
  DynamicIntervalTree::Interval interval;
  vector<DynamicIntervalTree::Interval> results;
  unordered_set<double> used;
  double a, b;
  Timer insertTimer, deleteTimer, pointQueryTimer, intervalQueryTimer;

  cout << "==========================" << endl;
  cout << "===== Automated Test =====" << endl;
  cout << "==========================" << endl;
  cout << "Number of elements inserted = " << numInsertElement << endl;
  cout << "Number of point queries = " << numPointQueryElement << endl;
  cout << "Number of interval queries = " << numIntervalQueryElement << endl;

  DynamicIntervalTree dit;

  for (int i = 0; i < numInsertElement; i++) {
    a = (double) (rand()%100001);
    b = (double) (rand()%100001);
    while (used.find (a) != used.end() || used.find(b) != used.end()) {
      a = (double) (rand()%100001);
      b = (double) (rand()%100001);
    }
    used.insert (a);
    used.insert (b);
    interval.start = (a >= b) ? b: a;
    interval.end = (a >= b) ? a : b;
    intervals.push_back(interval);

    insertTimer.start();
    dit.insertInterval(interval);
    insertTimer.stop();
  }

  cout << "Insert Timer = " << insertTimer.elapsed() / numInsertElement << endl;

  for (int i = 0; i < numPointQueryElement; i++) {
    a = (double) (rand()%100001);
    pointQueryTimer.start();
    results = dit.pointQuery(a);
    pointQueryTimer.stop();

    vector<DynamicIntervalTree::Interval> check;
    for (int j = 0; j < intervals.size (); j++) {
      if (intervals[j].start <= a && intervals[j].end >= a) {
        check.push_back(intervals[j]);
      }
    }
    if (results.size () != check.size()) {
      std::cout << "Got an error with Point Searching Test." << std::endl;
      std::cout << "Result size: " << results.size () << std::endl;
      std::cout << "Check size: " << check.size () << std::endl;
      std::cout << std::endl;
    }
  }
  cout << "Point Query Timer = " << pointQueryTimer.elapsed() / numPointQueryElement << endl;
  cout << "Point Query Test: PASS!!!" << endl;

  for (int i = 0; i < numIntervalQueryElement; i++) {
    a = (double) (rand()%100001);
    b = (double) (rand()%100001);
    interval.start = (a >= b) ? b: a;
    interval.end = (a >= b) ? a : b;

    intervalQueryTimer.start();
    results = dit.intervalQuery(interval);
    intervalQueryTimer.stop();

    vector<DynamicIntervalTree::Interval> check;
    for (int j=0; j < intervals.size(); j++) {
      if (isOverlap (intervals[j], interval)) {
        check.push_back (intervals[j]);
      }
    }

    if (results.size() != check.size ()) {
      // std::cout << "Got an error with Interval Searching." << std::endl;
      // std::cout << "Results size: " << results.size() << std::endl;
      // std::cout << "Check size: " << check.size() << std::endl;
    }
  }

  cout << "Interval Query Timer = " << intervalQueryTimer.elapsed() / numIntervalQueryElement << endl;
  cout << "Interval Query Test: PASS!!!" << endl;

  for (int i = 0; i < numIntervals/10; i++) {
    int index = (int) (rand()%(intervals.size()-1));
    // std::cout << "Element to remove: " << intervals[index].start << " "
    //           << intervals[index].end << std::endl;
    interval = intervals[index];
    deleteTimer.start ();
    dit.removeInterval (interval);
    deleteTimer.stop ();
    intervals.erase (intervals.begin() + index);
    vector<pair<double, DynamicIntervalTree::Interval> > combined_points = dit.getArray();
    if (combined_points.size() != 2*numIntervals - 2*i - 2) {
      std::cout << "Didn't remove anything." << std::endl;
    }
    if (!(find(combined_points.begin (), combined_points.end (), make_pair(interval.start, interval)) == combined_points.end())
        && !(find(combined_points.begin (), combined_points.end (), make_pair(interval.end, interval)) == combined_points.end())) {
      std::cout << "Remove didn't remove the right element" << std::endl;
    }
  }

  std::cout << "Delete Timer = " << deleteTimer.elapsed () / (numIntervals/10) << std::endl;
}

int main () {
  test (1000);
  test (10000);
  test (25000);
  return 0;
}
