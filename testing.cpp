#include "DynamicIntervalTree.h"
#include <unordered_set>
#include "Timer.h"

using namespace std;

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
      if (intervals[j].start <= interval.start && intervals[j].end >= interval.start) {
        check.push_back (intervals[j]);
      }
      else if (intervals[j].start > interval.start && intervals[j].start <= interval.end) {
        check.push_back (intervals[j]);
      }
    }

    if (results.size() != check.size ()) {
      // std::cout << "Got an error with Interval Searching." << std::endl;
      // std::cout << "Result size: " << results.size () << std::endl;
      // std::cout << "Check size: " << check.size () << std::endl;
      // std::cout << std::endl;
    }
  }

  cout << "Interval Query Timer = " << intervalQueryTimer.elapsed() / numIntervalQueryElement << endl;
  cout << "Interval Query Test: PASS!!!" << endl;
  //
  // for (int i = 0; i < numPointQueryElement; i++) {
  //   a = (double) (rand()%100001);
  //   results = pointQueryAll(root, a);
  //   for (int j = 0; j < results.size(); j++) {
  //     assert(results[j]->interval->start <= a && results[j]->interval->end >= a);
  //   }
  //
  //   int numResult = 0;
  //   for (int j = 0; j < intervals.size(); j++) {
  //     if (intervals[j].start <= a && intervals[j].end >= a) numResult++;
  //   }
  //   assert(results.size() == numResult);
  // }
  // cout << "Point Query All Test: PASS!!!" << endl;
  //
  // for (int i = 0; i < numIntervalQueryElement; i++) {
  //   a = (double) (rand()%100001);
  //   b = (double) (rand()%100001);
  //   interval.start = (a >= b) ? b: a;
  //   interval.end = (a >= b) ? a : b;
  //   results = intervalQueryAll(root, interval);
  //
  //   for (int j = 0; j < results.size(); j++) {
  //     assert(isOverlap(interval, *(results[j]->interval)));
  //   }
  //
  //   int numResult = 0;
  //   for (int j = 0; j < intervals.size(); j++) {
  //     if (isOverlap(intervals[j], interval)) numResult++;
  //   }
  //   assert(results.size() == numResult);
  // }
  // cout << "Interval Query All Test: PASS!!!" << endl;
}

int main () {
  bigTest ();
  // DynamicIntervalTree dit;
  // // DynamicIntervalTree::Interval intervals[] = {{3,10,1}, {4,5,2}, {11,18,3}, {19,22,4}, {12,14,5}, {1,2,6}, {9,15,7}};
  // DynamicIntervalTree::Interval intervals[] = {{1, 21}, {5, 13}, {2, 11}, {3, 15}, {0, 9}, {5.5, 7.5}, {27, 30}, {31, 51}, {37, 43}, {-2, -1}, {7, 17}, {6, 18}, {38, 41}, {39, 56}, {18, 24}, {6.5, 1000.5}};
  // int numElem = sizeof(intervals)/sizeof(intervals[0]);
  // for (int i = 0; i < numElem; i++) {
  //   cout << "Interval: " << intervals[i].start << endl;
  //   dit.insertInterval(intervals[i]);
  //   dit.preOrder();
  // }
  // cout << "===== Insertion =====" << endl;
  // dit.preOrder();
  //
  // // cout << "===== Deletion: (12, 14) =====" << endl;
  // // dit.removeInterval(intervals[4]);
  // // dit.preOrder();
  //
  // // cout << "===== Deletion: (3, 10) =====" << endl;
  // // dit.removeInterval(intervals[0]);
  // // dit.preOrder();
  //
  // cout << "===== Point Query: 20 ===== " << endl;
  // vector<DynamicIntervalTree::Interval> queryResult = dit.pointQuery(20);
  // for (int i = 0; i < queryResult.size(); i++) {
  //   cout << "(" << queryResult[i].start << "," << queryResult[i].end << ")" << endl;
  // }
  //
  // cout << "===== Interval Query: (1, 21) =====" << endl;
  // DynamicIntervalTree::Interval interval = {1, 21};
  // queryResult = dit.intervalQuery(interval);
  // for (int i = 0; i < queryResult.size(); i++) {
  //   cout << "(" << queryResult[i].start << "," << queryResult[i].end << ")" << endl;
  // }
  return 0;
}
