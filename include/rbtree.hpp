#include <iostream>
#include <cassert>

namespace Trees {

enum COLOR: char {
	BLACK, RED
};

template<typename T>
struct Node {
	Node *left_ = nullptr, *right_ = nullptr, *parent_ = nullptr;
	COLOR color_;      // color of node
	size_t nodes_ = 1; // holds number of nodes that contains left subtree +
					   // right subtree + this node
	const T key_;      // stored value
	Node(T key, COLOR color = RED) : key_(key), color_(color) { }
	Node(Node<T> const &rhs) { }
};

template<typename T>
struct Iter {
	const Node<T> *node_;
	int is_less_ = 0;  // stores number of elements that are less current
	Iter(Node<T> *node, size_t is_less) : node_(node), is_less_(is_less) {}
};

template<typename KeyT=int, typename Comp=std::less<KeyT>>
class RBTree {
	Node<KeyT> *root_ = nullptr;
	using iterator = const Node<KeyT> *;
	size_t sz_ = 0;
public:
	const Iter<KeyT> lower_bound(KeyT key) const { 
		Node<KeyT> *cur_node = root_;
		Node<KeyT> *res_node;
		int is_less = sz_;
		while (cur_node != nullptr) {
			if (key > cur_node->key_) {
				cur_node = cur_node->right_;
				continue;
			}
			is_less -= ((cur_node->right_ ? cur_node->right_->nodes_ : 0) + 1);
			if (res_node == nullptr)
				res_node = cur_node;
			else if (res_node->key_ > cur_node->key_) {
				res_node = cur_node;
			}
			cur_node = cur_node->left_;
		}
		assert(is_less >= 0);
		return Iter<KeyT>(cur_node, is_less);
	}
	const Iter<KeyT> upper_bound(KeyT key) const {
		Node<KeyT> *cur_node = root_;
		Node<KeyT> *res_node = nullptr;
		int is_less = sz_;
		while (cur_node != nullptr) {
			if (key >= cur_node->key_) {
				cur_node = cur_node->right_;
				continue;
			}
			is_less -= ((cur_node->right_ ? cur_node->right_->nodes_ : 0) + 1);
			if (res_node == nullptr)
				res_node = cur_node;
			else if (res_node->key_ > cur_node->key_) {
				res_node = cur_node;
			}
			cur_node = cur_node->left_;
		}
		assert(is_less >= 0);
		return Iter<KeyT>(res_node, is_less);
	}
	int distance(iterator fst, iterator snd) const { return 0; }

	void insert(KeyT key) {
		if (sz_ == 0) {
			root_ = new Node(key, BLACK);
			++sz_;
			return;
		}
		Node<KeyT> *cur_node = root_;	
		Node<KeyT> *prev_node;	
		while (cur_node != nullptr) {
			prev_node = cur_node;
			if (key == cur_node->key_)
				return;
			if (key < cur_node->key_)
				cur_node = cur_node->left_;
			else 
				cur_node = cur_node->right_;
		}
		cur_node = new Node(key);
		++sz_;
		if (key < prev_node->key_)
			prev_node->left_ = cur_node;
		else 
			prev_node->right_ = cur_node;
		cur_node->parent_ = prev_node;

		recount_nodes(prev_node);

		balance_tree(cur_node);
	}
	void recount_nodes(Node<KeyT> *cur_node) {
		while (cur_node != nullptr) {
			++cur_node->nodes_;
			cur_node = cur_node->parent_;
		}
	}

	void printBT() {
	    printBT("", root_, false);
	}

	int distance(KeyT &l, KeyT &r) {
		if (l > r) return 0;
		int res = upper_bound(r).is_less_ - lower_bound(l).is_less_;
		assert(res >= 0);
		return res;
	}

private:
	void balance_tree(Node<KeyT> *cur_node) {
		while (cur_node->parent_ != nullptr and cur_node->parent_->color_ == RED) {
			if (cur_node->parent_ == cur_node->parent_->parent_->left_) {
				Node<int> *&uncle = cur_node->parent_->parent_->right_;
				if (uncle == nullptr or uncle->color_ == BLACK) {
					if (cur_node == cur_node->parent_->right_) { // i.e. cur_node is right child
						cur_node = cur_node->parent_;
						left_rotate(cur_node);
					}
					cur_node->parent_->color_ = BLACK;
					cur_node->parent_->parent_->color_ = RED;
					right_rotate(cur_node->parent_->parent_);
				}
				else { // i.e. uncle is RED
					cur_node->parent_->color_ = BLACK;
					uncle->color_ = BLACK;
					cur_node->parent_->parent_->color_ = RED;
					cur_node = cur_node->parent_->parent_;
				}
			}
			else {
				Node<int> *&uncle = cur_node->parent_->parent_->left_;
				if (uncle == nullptr or uncle->color_ == BLACK) {
					if (cur_node == cur_node->parent_->left_) { // i.e. cur_node is left child
						cur_node = cur_node->parent_;
						right_rotate(cur_node);
					}
					cur_node->parent_->color_ = BLACK;
					cur_node->parent_->parent_->color_ = RED;
					left_rotate(cur_node->parent_->parent_);
				}
				else { // i.e. uncle is RED
					cur_node->parent_->color_ = BLACK;
					uncle->color_ = BLACK;
					cur_node->parent_->parent_->color_ = RED;
					cur_node = cur_node->parent_->parent_;
				}
			}
		}
		root_->color_ = BLACK;
	}

	/*
	 *          [x]                [y]     
	 *         /  \               /  \     
	 *       [y]  [g]    ->     [a]  [x]   
	 *      /  \                    /  \   
	 *    [a]  [b]                [b]  [g] 
	 *   x = cur_node
	*/
	void right_rotate(Node<KeyT> *cur_node) {
		Node<KeyT> *y = cur_node->left_;
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
	void left_rotate(Node<KeyT> *cur_node) {
		Node<KeyT> *y = cur_node->right_;
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
