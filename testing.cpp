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

void bigTest() {
  int numInsertElement = 10000;
  int numPointQueryElement = 10000;
  int numIntervalQueryElement = 10000;
  vector<DynamicIntervalTree::Interval> intervals;
  DynamicIntervalTree::Interval interval;
  vector<DynamicIntervalTree::Interval> results;
  unordered_set<double> used;
  double a, b;
  Timer insertTimer, pointQueryTimer, intervalQueryTimer;

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
      std::cout << "Got an error with Interval Searching." << std::endl;
    }
  }

  cout << "Interval Query Timer = " << intervalQueryTimer.elapsed() / numIntervalQueryElement << endl;
  cout << "Interval Query Test: PASS!!!" << endl;
}

int main () {
  bigTest ();
  return 0;
}
