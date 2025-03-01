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
  /*tree.insert(-3);*/
  /*EXPECT_EQ(tree.size(), 4);*/
  /*tree.insert(-10);*/
  /*EXPECT_EQ(tree.size(), 5);*/
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
  l = 5, r = 5;
  tree.insert(5);
  res = tree.distance(l, r);
  EXPECT_EQ(res, 1);
  l = 5, r = 10;
  tree.insert(10);
  res = tree.distance(l, r);
  EXPECT_EQ(res, 2);
}


TEST(RBTree, LowerBoundOneElement) {
  Trees::RBTree<int> tree;
  tree.insert(5);
  Trees::Iter<int> iter;
  iter = tree.lower_bound(5);
  EXPECT_EQ(iter.is_less_, 0);
  EXPECT_EQ(iter.node_->key_, 5);
  iter = tree.lower_bound(6);
  EXPECT_EQ(iter.is_less_, 1);
  EXPECT_EQ(iter.node_, nullptr);
}

TEST(RBTree, LowerBound) {
  Trees::RBTree<int> tree;
  Trees::Iter<int> iter;
  tree.insert(5);
  tree.insert(10);
  tree.insert(0);
  tree.insert(-5);
  iter = tree.lower_bound(5);
  EXPECT_EQ(iter.is_less_, 2);
  EXPECT_EQ(iter.node_->key_, 5);
  iter = tree.lower_bound(6);
  EXPECT_EQ(iter.is_less_, 3);
  EXPECT_EQ(iter.node_->key_, 10);
  iter = tree.lower_bound(7);
  EXPECT_EQ(iter.is_less_, 3);
  EXPECT_EQ(iter.node_->key_, 10);
  iter = tree.lower_bound(10);
  EXPECT_EQ(iter.is_less_, 3);
  EXPECT_EQ(iter.node_->key_, 10);
  iter = tree.lower_bound(11);
  EXPECT_EQ(iter.is_less_, 4);
  EXPECT_EQ(iter.node_, nullptr);
}

TEST(RBTree, UpperBoundOneElement) {
  Trees::RBTree<int> tree;
  tree.insert(5);
  int res, val, l, r;
  Trees::Iter<int> iter;
  iter = tree.upper_bound(5);
  EXPECT_EQ(iter.is_less_, 1);
  EXPECT_EQ(iter.node_, nullptr);
  iter = tree.upper_bound(4);
  EXPECT_EQ(iter.is_less_, 0);
  EXPECT_EQ(iter.node_->key_, 5);
}

TEST(RBTree, UpperBound) {
  Trees::RBTree<int> tree;
  Trees::Iter<int> iter;
  tree.insert(5);
  tree.insert(10);
  tree.insert(0);
  tree.insert(-5);
  iter = tree.upper_bound(5);
  EXPECT_EQ(iter.node_->key_, 10);
  EXPECT_EQ(iter.is_less_, 3);
  iter = tree.upper_bound(6);
  EXPECT_EQ(iter.node_->key_, 10);
  EXPECT_EQ(iter.is_less_, 3);
  iter = tree.upper_bound(7);
  EXPECT_EQ(iter.node_->key_, 10);
  EXPECT_EQ(iter.is_less_, 3);
  iter = tree.upper_bound(10);
  EXPECT_EQ(iter.node_, nullptr);
  EXPECT_EQ(iter.is_less_, 4);
  iter = tree.upper_bound(11);
  EXPECT_EQ(iter.node_, nullptr);
  EXPECT_EQ(iter.is_less_, 4);
}

TEST(RBTree, Distance) {
  Trees::RBTree<int> tree;
  int res, l, r;
  tree.insert(5);
  tree.insert(10);
  tree.insert(0);
  tree.insert(-5);
  tree.insert(8);
  tree.insert(20);
  l = -100, r = 100;
  res = tree.distance(l, r);
  EXPECT_EQ(res, 6);
  l = 0, r = 100;
  res = tree.distance(l, r);
  EXPECT_EQ(res, 5);
  l = 50, r = 100;
  res = tree.distance(l, r);
  EXPECT_EQ(res, 0);
  l = 5, r = 5;
  res = tree.distance(l, r);
  EXPECT_EQ(res, 1);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
