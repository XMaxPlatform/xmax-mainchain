/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <vector>

namespace pro
{
	template <class ValueType>
	class Tree {
	public:
		using value_type = ValueType;
		using difference_type = typename std::vector<Tree*>::difference_type;

		Tree(const ValueType& val) :val_(val) { }
		Tree(ValueType&& val) : val_(val) { }		
		~Tree() {
			parent_ = nullptr;
			children_.clear();
		}

		void AddChild(const Tree& child) {
			assert(child.parent_ == nullptr);			
			children_.push_back(child);		
			children_.back().parent_ = this;
		}

		void GetChildCount() const {
			return children_.size();
		}

		Tree& GetChild(difference_type i) {
			return children_[i];
		}

		void ClearChildren() {
			children_.clear();
		}

		Tree* GetParent() {
			return parent_;
		}

		ValueType& value() {
			return val_;
		}

		template <class VisitorType>
		void Traverse(VisitorType vistor) {
			for (auto child : children_)
			{
				child.Traverse(vistor);				
			}
			vistor(this);
		}

	private:
		ValueType val_{};
		Tree * parent_{ nullptr };
		std::vector<Tree> children_;
	};

}