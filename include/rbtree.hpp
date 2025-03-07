#include <cassert>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>

namespace Trees {

enum class COLOR {
	BLACK, RED
};

template<typename T>
struct Node {
  std::shared_ptr<Node<T>> left_, right_, parent_;
	COLOR color_;      // color of node
	size_t nodes_ = 1; // holds number of nodes that contains left subtree +
					   // right subtree + this node
	T key_;      // stored value
  Node() = default;
	Node(T key, Trees::COLOR color = Trees::COLOR::RED) : key_(key), color_(color) { }
};

template<typename T=int>
struct Iter {
  std::shared_ptr<const Node<T>> node_;
	int is_less_ = 0;  // stores number of elements that are less current
  Iter() = default;
	Iter(std::shared_ptr<Node<T>> node, size_t is_less) : node_(node), is_less_(is_less) {}
};

template<typename KeyT=int, typename Comp=std::less<KeyT>>
class RBTree final {
  Comp cmp = Comp{};
  std::shared_ptr<Node<KeyT>> root_{};
	size_t sz_ = 0;
public:
  size_t size() const {
    return sz_;
  }
  
  // Returns an Iter<T> pointing to an element greater than or equal to the key
  // If there is no such element Iter<T>.node == nullptr
	Iter<KeyT> lower_bound(KeyT key) const { 
    std::shared_ptr<Node<KeyT>> cur_node = root_;
		std::shared_ptr<Node<KeyT>> res_node = nullptr;
		int is_less = sz_;
		while (cur_node != nullptr) {
			if (cmp(cur_node->key_, key)) {
				cur_node = cur_node->right_;
				continue;
			}
			is_less -= ((cur_node->right_ ? cur_node->right_->nodes_ : 0) + 1);
			if (res_node == nullptr)
				res_node = cur_node;
			else if (cmp(cur_node->key_, res_node->key_)) {
				res_node = cur_node;
			}
			cur_node = cur_node->left_;
		}
		assert(is_less >= 0);
		return Iter<KeyT>(res_node, is_less);
	}


  // Returns an Iter<T> pointing to an element greater than the key
  // If there is no such element Iter<T>.node == nullptr
	Iter<KeyT> upper_bound(KeyT key) const {
    std::shared_ptr<Node<KeyT>> cur_node = root_;
    std::shared_ptr<Node<KeyT>> res_node = nullptr;
		int is_less = sz_;
		while (cur_node != nullptr) {
			if (!cmp(key, cur_node->key_)) {
				cur_node = cur_node->right_;
				continue;
			}
			is_less -= ((cur_node->right_ ? cur_node->right_->nodes_ : 0) + 1);
			if (res_node == nullptr)
				res_node = cur_node;
			else if (cmp(cur_node->key_, res_node->key_)) {
				res_node = cur_node;
			}
			cur_node = cur_node->left_;
		}
		assert(is_less >= 0);
		return Iter<KeyT>(res_node, is_less);
	}

  // Insert key to the Red-Black Tree
	void insert(KeyT key) {
		if (sz_ == 0) {
			root_ = std::make_shared<Node<KeyT>>(key, Trees::COLOR::BLACK);
			++sz_;
			return;
		}
	  std::shared_ptr<Node<KeyT>> cur_node = root_;	
		std::shared_ptr<Node<KeyT>> prev_node;	
		while (cur_node != nullptr) {
			prev_node = cur_node;
			if (!cmp(key, cur_node->key_) and !cmp(cur_node->key_, key))
				return;
			if (cmp(key, cur_node->key_))
				cur_node = cur_node->left_;
			else 
				cur_node = cur_node->right_;
		}
		cur_node = std::make_shared<Node<KeyT>>(key);
		++sz_;
		if (cmp(key, prev_node->key_))
			prev_node->left_ = cur_node;
		else 
			prev_node->right_ = cur_node;
		cur_node->parent_ = prev_node;

		recount_nodes(prev_node);

		balance_tree(cur_node);
	}

	void printBT() {
	    printBT("", root_, false);
	}

  // Counts the number of elements lying between l and r
	int distance(KeyT &l, KeyT &r) {
		if (l > r) return 0;
		int res = upper_bound(r).is_less_ - lower_bound(l).is_less_;
		assert(res >= 0);
		return res;
	}

  ~RBTree() {
    if (root_ == nullptr) 
      return;
    std::stack<std::shared_ptr<Node<KeyT>>> q;
    q.push(root_);
    while (!q.empty()) {
      std::shared_ptr<Node<KeyT>> cur_node = q.top();
      q.pop();
      if (cur_node->left_ != nullptr) {
        q.push(cur_node->left_);
        cur_node->left_ = nullptr;
      }
      if (cur_node->right_ != nullptr) {
        q.push(cur_node->right_);
        cur_node->right_ = nullptr;
      }
    }
  }

private:

	void recount_nodes(std::shared_ptr<Node<KeyT>> cur_node) {
		while (cur_node != nullptr) {
			++cur_node->nodes_;
			cur_node = cur_node->parent_;
		}
	}
  
	void balance_tree(std::shared_ptr<Node<KeyT>> cur_node) {
		while (cur_node->parent_ != nullptr and cur_node->parent_->color_ == Trees::COLOR::RED) {
			if (cur_node->parent_ == cur_node->parent_->parent_->left_) {
        std::shared_ptr<Node<KeyT>> uncle = cur_node->parent_->parent_->right_;
				if (uncle == nullptr or uncle->color_ == Trees::COLOR::BLACK) {
					if (cur_node == cur_node->parent_->right_) { // i.e. cur_node is right child
						cur_node = cur_node->parent_;
						left_rotate(cur_node);
					}
					cur_node->parent_->color_ = Trees::COLOR::BLACK;
					cur_node->parent_->parent_->color_ = Trees::COLOR::RED;
					right_rotate(cur_node->parent_->parent_);
				}
				else { // i.e. uncle is RED
					cur_node->parent_->color_ = Trees::COLOR::BLACK;
					uncle->color_ = Trees::COLOR::BLACK;
					cur_node->parent_->parent_->color_ = Trees::COLOR::RED;
					cur_node = cur_node->parent_->parent_;
				}
			}
			else {
        std::shared_ptr<Node<KeyT>> uncle = cur_node->parent_->parent_->left_;
				if (uncle == nullptr or uncle->color_ == Trees::COLOR::BLACK) {
					if (cur_node == cur_node->parent_->left_) { // i.e. cur_node is left child
						cur_node = cur_node->parent_;
						right_rotate(cur_node);
					}
					cur_node->parent_->color_ = Trees::COLOR::BLACK;
					cur_node->parent_->parent_->color_ = Trees::COLOR::RED;
					left_rotate(cur_node->parent_->parent_);
				}
				else { // i.e. uncle is RED
					cur_node->parent_->color_ = Trees::COLOR::BLACK;
					uncle->color_ = Trees::COLOR::BLACK;
					cur_node->parent_->parent_->color_ = Trees::COLOR::RED;
					cur_node = cur_node->parent_->parent_;
				}
			}
		}
		root_->color_ = Trees::COLOR::BLACK;
	}

	/*
	 *          [x]                [y]     
	 *         /  \               /  \     
	 *       [y]  [g]    ->     [a]  [x]   
	 *      /  \                    /  \   
	 *    [a]  [b]                [b]  [g] 
	 *   x = cur_node
	*/
	void right_rotate(std::shared_ptr<Node<KeyT>> cur_node) {
    std::shared_ptr<Node<KeyT>> y = cur_node->left_;
		cur_node->left_ = y->right_;
		if (y->right_ != nullptr)
			y->right_->parent_ = cur_node;
		y->parent_ = cur_node->parent_;

		if (cur_node->parent_ == nullptr)
			root_ = y;
		else if (cur_node == cur_node->parent_->left_)
			cur_node->parent_->left_ = y;
		else
			cur_node->parent_->right_ = y;

		y->right_ = cur_node;
		cur_node->parent_ = y;

		cur_node->nodes_ = (cur_node->left_ ? cur_node->left_->nodes_ : 0)
						 + (cur_node->right_ ? cur_node->right_->nodes_ : 0) + 1;
		y->nodes_ = (y->left_ ? y->left_->nodes_ : 0) + cur_node->nodes_ + 1;
	}

	/*
	 *       [x]                 [y]
	 *      /  \                /  \
	 *    [a]  [y]      ->    [x]  [g]  
	 *        /  \           /  \
	 *      [b]  [g]       [a]  [b]
	 *  x = cur_node
	*/
	void left_rotate(std::shared_ptr<Node<KeyT>> cur_node) {
    std::shared_ptr<Node<KeyT>> y = cur_node->right_;
		cur_node->right_ = y->left_;
		if (y->left_ != nullptr)
			y->left_->parent_ = cur_node;
		y->parent_ = cur_node->parent_;

		if (cur_node->parent_ == nullptr)
			root_ = y;
		else if (cur_node == cur_node->parent_->left_)
			cur_node->parent_->left_ = y;
		else
			cur_node->parent_->right_ = y;

		y->left_ = cur_node;
		cur_node->parent_ = y;


		cur_node->nodes_ = (cur_node->left_ ? cur_node->left_->nodes_ : 0)
						 + ( cur_node->right_ ? cur_node->right_->nodes_ : 0) + 1;
		y->nodes_ = cur_node->nodes_ 
			      + (y->right_ ? y->right_->nodes_ : 0) + 1;
	}

	void printBT(const std::string& prefix, const Node<KeyT>* node, bool isLeft)
	{
	    if (node != nullptr)
	    {
	        std::cout << prefix;
	        std::cout << (isLeft ? "|--" : "L--");
	        // print the value of the node
	        std::cout << "k = " << node->key_ << "; c = " << (node->color_ == 1 ? "R" : "B") << " "
				"n = " << node->nodes_ << std::endl;
	        // enter the next tree level - left and right branch
	        printBT(prefix + (isLeft ? "|   " : "    "), node->right_, true);
	        printBT(prefix + (isLeft ? "|   " : "    "), node->left_, false);
	    }
	}
	
};

}
