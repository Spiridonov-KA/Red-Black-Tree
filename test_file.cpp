#include <gtest/gtest.h>

#include "rbtree.hpp"

TEST(RBTree, Size) {
  Trees::RBTree<int> tree;
  EXPECT_EQ(tree.size(), 0);
  tree.insert(10);
  EXPECT_EQ(tree.size(), 1);
  tree.insert(10);
  EXPECT_EQ(tree.size(), 1);
  tree.insert(10);
  EXPECT_EQ(tree.size(), 1);
  tree.insert(11);
  EXPECT_EQ(tree.size(), 2);
  tree.insert(0);
  EXPECT_EQ(tree.size(), 3);
  tree.insert(-3);
  EXPECT_EQ(tree.size(), 4);
  tree.insert(-10);
  EXPECT_EQ(tree.size(), 5);
}

TEST(RBTree, EmptyTreeDistance) {
  Trees::RBTree<int> tree;
  int res, l, r;
  l = 0, r = 0;
  res = tree.distance(l, r);
  EXPECT_EQ(res, 0);
  l = -1000, r = 1000;
  res = tree.distance(l, r);
  EXPECT_EQ(res, 0);
}

TEST(RBTree, AddingSameElementsDistance) {
  Trees::RBTree<int> tree;
  int res, l, r;
  tree.insert(5);
  l = 0, r = 0;
  res = tree.distance(l, r);
  EXPECT_EQ(res, 0);
  l = -1000, r = 1000;
  res = tree.distance(l, r);
  EXPECT_EQ(res, 1);
  tree.insert(5);
  res = tree.distance(l, r);
  EXPECT_EQ(res, 1);
  l = -10, r = 10;
  tree.insert(6);
  res = tree.distance(l, r);
  EXPECT_EQ(res, 2);
}


TEST(RBTree, ElementsEqualsEdgesDistance) {
  Trees::RBTree<int> tree;
  int res, l, r;
  tree.insert(5);
  l = 5, r = 5;
  res = tree.distance(l, r);
  EXPECT_EQ(res, 1);
  l = 5, r = 10;
  tree.insert(10);
  res = tree.distance(l, r);
  EXPECT_EQ(res, 2);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
