// UMBC - CMSC 341 - Spring 2023 - Proj2
#include "show.h"
Show::Show() { m_root = nullptr; }
Show::~Show() { clear(); }

void Show::insert(const Drone &aDrone) {
  if (aDrone.m_id < MINID || aDrone.m_id > MAXID)
    return;

  if (m_root == nullptr) {
    Drone *newDrone = new Drone(aDrone.m_id, aDrone.m_type, aDrone.m_state);
    m_root = newDrone;
    m_root->setHeight(0);
  } else {
    Drone *newDrone = new Drone(aDrone.m_id, aDrone.m_type, aDrone.m_state);
    m_root = insertHelp(m_root, newDrone);
  }
}

Drone *Show::insertHelp(Drone *root, Drone *drone) {
  if (root == nullptr) {
    root = drone;
  }

  if (drone->m_id < root->m_id)
    root->m_left = insertHelp(root->m_left, drone);
  else if (drone->m_id > root->m_id)
    root->m_right = insertHelp(root->m_right, drone);
  else
    return root;

  root = rebalance(root, drone);

  if (heightCount(root->m_left) > heightCount(root->m_right))
    root->setHeight(1 + heightCount(root->m_left));
  else
    root->setHeight(1 + heightCount(root->m_right));

  return root;
}

int Show::heightCount(Drone *drone) {
  if (drone == nullptr)
    return -1;
  else {
    int lheight = heightCount(drone->m_left);
    int rheight = heightCount(drone->m_right);

    if (lheight > rheight)
      return (lheight + 1);
    else
      return (rheight + 1);
  }
}

int Show::imbalanceVal(Drone *drone) {
  if (drone == nullptr)
    return -1;
  return heightCount(drone->m_left) - heightCount(drone->m_right);
}

Drone *Show::leftRotate(Drone *drone) {
  Drone *dr = drone->m_right;
  Drone *temp = dr->m_left;

  dr->m_left = drone;
  drone->m_right = temp;

  int DL = heightCount(drone->m_left);
  int DR = heightCount(drone->m_right);

  int TL = heightCount(dr->m_left);
  int TR = heightCount(dr->m_right);

  if (DL > DR)
    drone->setHeight(DL + 1);
  else
    drone->setHeight(DR + 1);

  if (TL > TR)
    dr->setHeight(TL + 1);
  else
    dr->setHeight(TR + 1);

  return dr;
}

Drone *Show::rightRotate(Drone *drone) {
  Drone *dr = drone->m_left;
  Drone *temp = dr->m_right;

  dr->m_right = drone;
  drone->m_left = temp;

  int DL = heightCount(drone->m_left);
  int DR = heightCount(drone->m_right);

  int TL = heightCount(dr->m_left);
  int TR = heightCount(dr->m_right);

  if (DL > DR)
    drone->setHeight(DL + 1);
  else
    drone->setHeight(DR + 1);

  if (TL > TR)
    dr->setHeight(TL + 1);
  else
    dr->setHeight(TR + 1);

  return dr;
}

Drone *Show::rebalance(Drone *root, Drone *drone) {
  int imb = imbalanceVal(root);
  if (imb > 1 && drone->m_id < root->m_left->m_id)
    return rightRotate(root);
  if (imb < -1 && drone->m_id > root->m_right->m_id)
    return leftRotate(root);
  if (imb > 1 && drone->m_id > root->m_left->m_id) {
    root->m_left = leftRotate(root->m_left);
    return rightRotate(root);
  }
  if (imb < -1 && drone->m_id < root->m_right->m_id) {
    root->m_right = rightRotate(root->m_right);
    return leftRotate(root);
  }
  return root;
}

void Show::clear() {
  clearHelp(m_root);
  m_root = nullptr;
}
void Show::clearHelp(Drone *drone) {
  if (drone == nullptr)
    return;

  if (drone->m_left == nullptr && drone->m_right == nullptr) {
    drone->m_left = nullptr;
    drone->m_right = nullptr;
    delete drone;
    drone = nullptr;
    return;
  }

  else if (drone->m_left == nullptr) {
    clearHelp(drone->m_right);
  }

  else if (drone->m_right == nullptr) {
    clearHelp(drone->m_left);
  }

  else if (drone->m_left && drone->m_right) {
    clearHelp(drone->m_right);
    clearHelp(drone->m_left);
  }
  drone->m_left = nullptr;
  drone->m_right = nullptr;
  delete drone;
  drone = nullptr;
  return;
}

void Show::remove(int id) {
  if (findDrone(id)) {
    m_root = removeHelp(id, m_root);
  }
}
Drone *Show::removeHelp(int id, Drone *mroot) {
  if (mroot == nullptr)
    return mroot;

  if (id < mroot->m_id)
    mroot->m_left = removeHelp(id, mroot->m_left);

  else if (id > mroot->m_id)
    mroot->m_right = removeHelp(id, mroot->m_right);

  else if (id == mroot->m_id) { // if id is same as drone's id
    if (mroot->m_left == nullptr && mroot->m_right == nullptr) {
      delete mroot;
      return nullptr;
    }

    else if (mroot->m_left == nullptr && mroot->m_right != nullptr) {
      Drone *temp = mroot->m_right;
      delete mroot;
      return temp;

    } else if (mroot->m_right == nullptr && mroot->m_left != nullptr) {
      Drone *temp = mroot->m_left;
      delete mroot;
      return temp;

    } else { // when both right and left exists
      Drone *temp = mroot->m_right;
      temp->m_state = mroot->m_right->m_state;
      while (temp->m_left)
        temp = temp->m_left;

      mroot->m_id = temp->m_id;
      mroot->m_state = temp->m_state;
      mroot->m_right = removeHelp(temp->m_id, mroot->m_right);
    }
  }
  return rebalance(mroot, mroot);
}

void Show::dumpTree() const { dump(m_root); }
void Show::dump(Drone *aDrone) const {
  if (aDrone != nullptr) {
    cout << "(";
    dump(aDrone->m_left); // first visit the left child
    cout << aDrone->m_id << ":"
         << aDrone->m_height; // second visit the node itself
    dump(aDrone->m_right);    // third visit the right child
    cout << ")";
  }
}

void Show::listDrones() const { listDronesHelp(m_root); }
void Show::listDronesHelp(Drone *drone) const {
  if (drone != nullptr) {
    listDronesHelp(drone->m_left);
    cout << drone->m_id << ":" << drone->m_height << ":" << drone->m_type << ":"
         << drone->m_state << endl;
    listDronesHelp(drone->m_right);
  }
}

bool Show::setState(int id, STATE state) {
  Drone *drone = findDroneHelp(id, m_root);
  if (drone != nullptr) {
    drone->setState(state);
    return true;
  }
  return false;
}

void Show::removeLightOff() {
  if (m_root->m_state == LIGHTOFF)
    m_root = removeHelp(m_root->m_id, m_root);
  else
    m_root = removeLightOffHelp(m_root);
}
Drone *Show::removeLightOffHelp(Drone *drone) {
  if (drone == nullptr) {
    return nullptr;
  }
  if (drone->m_left) {
    drone->m_left = removeLightOffHelp(drone->m_left);
  }
  if (drone->m_right) {
    drone->m_right = removeLightOffHelp(drone->m_right);
  }
  if (drone->m_state == LIGHTOFF) {
    drone = removeHelp(drone->m_id, drone);
  }
  return drone;
}
bool Show::findDrone(int id) const {
  Drone *dr = findDroneHelp(id, m_root);
  if (dr)
    return true;
  return false;
}
Drone *Show::findDroneHelp(int id, Drone *drone) const {
  if (drone == nullptr) {
    return nullptr;
  }
  if (drone->m_id == id) {
    return drone;
  } else if (drone->m_id > id) {
    return findDroneHelp(id, drone->m_left);
  }
  return findDroneHelp(id, drone->m_right);
}

const Show &Show::operator=(const Show &rhs) {
  if (&rhs != this) {
    if (rhs.m_root != nullptr) {
      clear();

      m_root = assignmentHelp(rhs.m_root);
    }
    return *this;
  }
  return *this;
}
Drone *Show::assignmentHelp(Drone *root) {
  if (root == nullptr)
    return root;

  Drone *drone = new Drone(root->m_id, root->m_type, root->m_state);
  drone->m_height = root->m_height;
  drone->m_left = assignmentHelp(drone->m_left);
  drone->m_right = assignmentHelp(drone->m_right);
  return drone;
}

int Show::countDrones(LIGHTCOLOR aColor) const {
  Drone *root = m_root;
  return countDronesHelp(root, aColor);
}
int Show::countDronesHelp(Drone *drone, LIGHTCOLOR aColor, int count) const {
  if (drone == nullptr) {
    return count;
  }

  if (drone->m_type == aColor) {
    count++;
  }

  if (drone->m_left)
    count += countDronesHelp(drone->m_left, aColor);
  if (drone->m_right)
    count += countDronesHelp(drone->m_right, aColor);
  return count;
}
