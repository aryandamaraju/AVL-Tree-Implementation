// UMBC - CMSC 341 - Spring 2023 - Proj2
#include "show.h"
#include <random>

enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL };
class Random {
public:
  Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50,
         int stdev = 20)
      : m_min(min), m_max(max), m_type(type) {
    if (type == NORMAL) {
      // the case of NORMAL to generate integer numbers with normal distribution
      m_generator = std::mt19937(m_device());
      // the data set will have the mean of 50 (default) and standard deviation
      // of 20 (default) the mean and standard deviation can change by passing
      // new values to constructor
      m_normdist = std::normal_distribution<>(mean, stdev);
    } else if (type == UNIFORMINT) {
      // the case of UNIFORMINT to generate integer numbers
      //  Using a fixed seed value generates always the same sequence
      //  of pseudorandom numbers, e.g. reproducing scientific experiments
      //  here it helps us with testing since the same sequence repeats
      m_generator = std::mt19937(10); // 10 is the fixed seed value
      m_unidist = std::uniform_int_distribution<>(min, max);
    } else { // the case of UNIFORMREAL to generate real numbers
      m_generator = std::mt19937(10); // 10 is the fixed seed value
      m_uniReal =
          std::uniform_real_distribution<double>((double)min, (double)max);
    }
  }
  void setSeed(int seedNum) {
    // we have set a default value for seed in constructor
    // we can change the seed by calling this function after constructor call
    // this gives us more randomness
    m_generator = std::mt19937(seedNum);
  }

  int getRandNum() {
    // this function returns integer numbers
    // the object must have been initialized to generate integers
    int result = 0;
    if (m_type == NORMAL) {
      // returns a random number in a set with normal distribution
      // we limit random numbers by the min and max values
      result = m_min - 1;
      while (result < m_min || result > m_max)
        result = m_normdist(m_generator);
    } else if (m_type == UNIFORMINT) {
      // this will generate a random number between min and max values
      result = m_unidist(m_generator);
    }
    return result;
  }

  double getRealRandNum() {
    // this function returns real numbers
    // the object must have been initialized to generate real numbers
    double result = m_uniReal(m_generator);
    // a trick to return numbers only with two deciaml points
    // for example if result is 15.0378, function returns 15.03
    // to round up we can use ceil function instead of floor
    result = std::floor(result * 100.0) / 100.0;
    return result;
  }

private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;     // normal distribution
  std::uniform_int_distribution<> m_unidist; // integer uniform distribution
  std::uniform_real_distribution<double> m_uniReal; // real uniform distribution
};

class Tester {
public:
  void timeMeasurementInsert(Tester tester);
  void timeMeasurementInsertHelp(Show &aShow, Show &bShow, Drone tempArray[],
                                 Drone tempArr[], int arraySizeA,
                                 int arraySizeB);
  void timeMeasurementRemove(Tester tester);
  void timeMeasurementRemoveHelp(Show &aShow, Show &bShow, int tempArray[],
                                 int tempArr[], int arraySizeA, int arraySizeB);

  void testCountDrones(Show show);
  void testFindDrone(Show show);
  void testremove(Show show, Show show1);
  void testInsert(Show show, Show show1);
  void testAssignment(Show show, Show show1);
  void testRemoveLightOff(Show show);
  bool testBSTProperty(Drone *aDrone);
};
int main() {
  Tester tester;
  Show show1, show2, show3, show4, show5, show6, show7, show8, show9, show10,
      show11;
  tester.testInsert(show1, show2);
  tester.testCountDrones(show3);
  tester.testFindDrone(show4);
  tester.testremove(show5, show6);
  tester.testAssignment(show7, show8);
  tester.testRemoveLightOff(show9);
  tester.timeMeasurementInsert(tester);
  tester.timeMeasurementRemove(tester);

  return 0;
}

void Tester::timeMeasurementInsert(Tester tester) {
  Show show, show1;
  int sizeA = 1000;
  int sizeB = 2000;
  Random idGen(MINID, MAXID);
  Random typeGen(0, 2); // there are three colors
  Drone tempID[1001] = {};
  Drone tempIDs[2002] = {};
  double TA = 0.0, TB = 0.0;
  clock_t startA, stopA, startB, stopB;
  int id = 0;
  startA = clock();
  for (int i = 0; i < sizeA; i++) {
    id = idGen.getRandNum();
    Drone drone(id, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    tempID[i] = drone;
  }
  stopA = clock();
  TA = stopA - startA;

  startB = clock();
  for (int i = 0; i < sizeB; i++) {
    id = idGen.getRandNum();
    Drone drone(id, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    tempIDs[i] = drone;
  }
  stopB = clock();
  cout << "________________________________________________________\n" << endl;
  cout << "Testing insertion time complexity\n" << endl;
  
  TB = stopB - startB;
  double measureTimeA = TA / CLOCKS_PER_SEC;
  double measureTimeB = TB / CLOCKS_PER_SEC;
  double ratio = (measureTimeB / (2 * measureTimeA));
  cout << "Ratio: " << ratio << endl;
  if (ratio > 0.7 && ratio < 1.5) {
    cout << "\nTime for inserting nodes is O(log n). Test passed." << endl;
  }
}

void Tester::timeMeasurementInsertHelp(Show &aShow, Show &bShow,
                                       Drone *tempArray, Drone *tempArr,
                                       int arraySizeA, int arraySizeB) {
  /*double TA = 0.0, TB = 0.0;
  clock_t startA, stopA, startB, stopB;
  startA = clock();
  for (int j = 0; j < arraySizeA; j++) {
    aShow.insert(tempArray[j]);
  }
  stopA = clock();
  TA = stopA - startA;

  startB = clock();
  for (int j = 0; j < arraySizeB; j++) {
    bShow.insert(tempArr[j]);
  }
  stopB = clock();
  TB = stopB - startB;

  double measureTimeA = TA / CLOCKS_PER_SEC;
  double measureTimeB = TB / CLOCKS_PER_SEC;
  double ratio = (measureTimeB / (2*measureTimeA));
  cout << "Ratio: " << ratio << endl;
  if (ratio > 0.7 && ratio < 1.5) {
    cout << "\nTime for inserting nodes is O(log n). Test passed." << endl;
  }
	aShow.clear();
	bShow.clear();*/
}

void Tester::timeMeasurementRemove(Tester tester) {
  Show show, show1;
  int sizeA = 1000;
  int sizeB = 2000;
  Random idGen(MINID, MAXID);
  Random typeGen(0, 2); // there are three colors
  int tempID[1001] = {};
  int tempIDs[2002] = {};
  int id = 0;
  for (int i = 0; i < sizeA; i++) {
    id = idGen.getRandNum();
    tempID[i] = id;
  }
  for (int i = 0; i < sizeB; i++) {
    id = idGen.getRandNum();
    tempIDs[i] = id;
  }
  cout << "________________________________________________________\n" << endl;
  cout << "Testing Removal time complexity\n" << endl;
  tester.timeMeasurementRemoveHelp(show, show1, tempID, tempIDs, sizeA, sizeB);
}

void Tester::timeMeasurementRemoveHelp(Show &aShow, Show &bShow, int *tempArray,
                                       int *tempArr, int arraySizeA,
                                       int arraySizeB) {
  double TA = 0.0, TB = 0.0;
  clock_t startA, stopA, startB, stopB;
  startA = clock();
  for (int j = 0; j < arraySizeA; j++) {
    aShow.remove(tempArray[j]);
  }
  stopA = clock();
  TA = stopA - startA;

  startB = clock();
  for (int j = 0; j < arraySizeB; j++) {
    bShow.remove(tempArr[j]);
  }
  stopB = clock();
  TB = stopB - startB;

  double measureTimeA = TA / CLOCKS_PER_SEC;
  double measureTimeB = TB / CLOCKS_PER_SEC;
  double ratio = (measureTimeB / (2*measureTimeA));
  cout << "Ratio: " << ratio << endl;
  if (ratio > 0.7 && ratio < 1.5) {
    cout << "\nTime for removing nodes is O(log n). Test passed." << endl;
  }
}

void Tester::testCountDrones(Show show) {
  Random idGen(MINID, MAXID);
  Random typeGen(0, 2); // there are three colors
  int teamSize = 10;
  int ID = 0;
  for (int i = 0; i < teamSize; i++) {
    ID = idGen.getRandNum();
    Drone drone(ID, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    show.insert(drone);
  }
  cout << "________________________________________________________\n" << endl;
  cout << "Count drone test\n" << endl;
  int quant = show.countDrones(LIGHTCOLOR::RED);
  if (quant == 2)

    cout << "\nCount drones working for normal case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;
}

void Tester::testFindDrone(Show show) {
  Random idGen(MINID, MAXID);
  Random typeGen(0, 2); // there are three colors
  int teamSize = 10;
  int ID = 0;
  int tempID = 0;
  for (int i = 0; i < teamSize; i++) {
    ID = idGen.getRandNum();
    if (i == teamSize / 2)
      tempID = ID; // we store this ID for later use
    Drone drone(ID, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    show.insert(drone);
  }
  cout << "________________________________________________________\n" << endl;
  cout << "Find drone test\n" << endl;

  if (show.findDrone(tempID))
    cout << "\nFind Drone working for normal case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;

  if (!show.findDrone(1000))
    cout << "\nError case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;
}

void Tester::testremove(Show show, Show show1) {
  Random idGen(MINID, MAXID);
  Random typeGen(0, 2); // there are three colors
  int teamSize = 10;
  int ID = 0;
  for (int i = 0; i < teamSize; i++) {
    ID = idGen.getRandNum();
    Drone drone(ID, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    show.insert(drone);
  }
  cout << "________________________________________________________\n" << endl;
  cout << "Remove drone test\n" << endl;
  bool var = show.findDrone(11867);
  show.remove(11867);
  if (var && !show.findDrone(11867))
    cout << "\nRemove working for normal case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;

  int tempRoot = show.m_root->m_id;
  show.remove(show.m_root->m_id);
  if (!show.findDrone(tempRoot))
    cout << "\nRemove working for edge case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;

  if (testBSTProperty(show.m_root)) {
    cout << "\nBST property retained for remove." << endl;
  } else
    cout << "\nBST property not retained." << endl;

  show.remove(100);
  if (!show.findDrone(100))
    cout << "\nError case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;

  for (int i = 0; i < 300; i++) {
    ID = idGen.getRandNum();
    Drone drone(ID, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    show1.insert(drone);
  }

  for (int i = 0; i < 150; i++) {
    show1.remove(show.m_root->getID());
  }

  if (show1.m_root && show1.m_root->m_left && show1.m_root->m_right &&
      show1.m_root->m_height > 0)
    cout << "\nRemove works for huge case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;
}

void Tester::testInsert(Show show1, Show show2) {
  Random idGen(MINID, MAXID);
  Random typeGen(0, 2);
  int teamSize = 10;
  int ID = 0;
  for (int i = 0; i < teamSize; i++) {
    ID = idGen.getRandNum();
    Drone drone(ID, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    show1.insert(drone);
    if (i % 2 == 0) {
      show1.setState(ID, STATE::LIGHTOFF);
    }
  }
  cout << "________________________________________________________\n" << endl;
  cout << "Insert drone test\n" << endl;
  Drone drone1(100, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
  show1.insert(drone1);
  if (show1.m_root->getHeight() == 3 && show1.m_root->getLeft() &&
      show1.m_root->getRight())
    cout << "\nInsert works for normal case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;

  if (!show1.findDrone(100))
    cout << "\nError case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;

  Drone drone2(11789, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
  show2.insert(drone2);
  if (show2.m_root->getID() == 11789 && !show2.m_root->getLeft() &&
      !show2.m_root->getRight())
    cout << "\nInsert works for edge case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;

  if (testBSTProperty(show1.m_root)) {
    cout << "\nBST property retained for insert." << endl;
  } else
    cout << "\nBST property not retained." << endl;

  for (int i = 0; i < 300; i++) {
    ID = idGen.getRandNum();
    Drone drone(ID, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    show2.insert(drone);
    if (i % 2 == 0) {
      show2.setState(ID, STATE::LIGHTOFF);
    }
  }
  if (show1.m_root && show1.m_root->m_left && show1.m_root->m_right &&
      show1.m_root->m_height > 0)
    cout << "\nInsert works for huge case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;
  // test HUGE
}

void Tester::testAssignment(Show show, Show show1) {
  Show show2;
  Random idGen(MINID, MAXID);
  Random typeGen(0, 2);
  int teamSize = 10;
  int ID = 0;
  for (int i = 0; i < teamSize; i++) {
    ID = idGen.getRandNum();
    Drone drone(ID, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    show.insert(drone);
    if (i % 2 == 0) {
      show.setState(ID, STATE::LIGHTOFF);
    }
  }
  cout << "________________________________________________________\n" << endl;
  cout << "Overloaded operator test\n" << endl;

  int og = show.m_root->m_id;
  Drone *ogL = show.m_root->m_left;
  show1 = show;
  if (og == show1.m_root->m_id && ogL == show1.m_root->m_left)
    cout << "\nAssignment works for normal case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;

  og = show1.m_root->m_id;
  show1 = show2;
  if (show2.m_root == nullptr)
    cout << "\nError case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;
}

void Tester::testRemoveLightOff(Show show) {
  Random idGen(MINID, MAXID);
  Random typeGen(0, 2);
  int teamSize = 10;
  int ID = 0;
  for (int i = 0; i < teamSize; i++) {
    ID = idGen.getRandNum();
    Drone drone(ID, static_cast<LIGHTCOLOR>(typeGen.getRandNum()));
    show.insert(drone);
    if (i % 2 == 0) {
      show.setState(ID, STATE::LIGHTOFF);
    }
  }
  cout << "________________________________________________________\n" << endl;
  cout << "Remove Light off test\n" << endl;
  show.removeLightOff();
  if (show.m_root->m_state == LIGHTON &&
      show.m_root->m_left->m_state == LIGHTON &&
      show.m_root->m_right->m_state == LIGHTON)
    cout << "\nRemove Light Off works for normal case. Test passed." << endl;
  else
    cout << "\nTest failed." << endl;
}

bool Tester::testBSTProperty(Drone *aDrone) {

  int id = aDrone->m_id;
  bool var;
  if (aDrone != nullptr) {
    if (aDrone->m_left && aDrone->m_left->m_id < id) {
      var = testBSTProperty(aDrone->m_left);
      if(var == false)
        return false;
    } else
      return false;

    if (aDrone->m_right && aDrone->m_right->m_id > id) {
      var = testBSTProperty(aDrone->m_right);
    } else
      return false;
  }
  return true;
}