#pragma once
#include <cassert>
namespace wzz::misc{

template <typename T>
struct intrusive_list_enabled_t{
	intrusive_list_enabled_t<T>* prev = nullptr;
	intrusive_list_enabled_t<T>* next = nullptr;
};

// add element from iterator
//T must derived from intrusive_list_enabled_t
template <typename T>
class intrusive_list_t{
public:
	//construct using ptr created from object pool
	class iterator_t{
	public:
		friend class intrusive_list_t<T>;

		iterator_t(intrusive_list_enabled_t<T>* node)
		  :node(node)
		{}

		iterator_t() = default;

		explicit operator bool() const{
			return node;
		}

		bool operator==(const iterator_t& lhs) const{
			return node == lhs.node;
		}

		bool operator!=(const iterator_t& lhs) const{
			return node != lhs.node;
		}

		T& operator*(){
			return *static_cast<T*>(node);
		}

		const T& operator*() const{
			return *static_cast<T*>(node);
		}

		T* get(){
			return static_cast<T*>(node);
		}
		const T* get() const{
			return static_cast<T*>(node);
		}

		T* operator->(){
			return static_cast<T*>(node);
		}

		const T* operator->() const{
			return static_cast<T*>(node);
		}
		//++it
		iterator_t& operator++(){
			node = node->next;
			return *this;
		}
		iterator_t& operator--(){
			node = node->prev;
			return *this;
		}
	private:
		intrusive_list_enabled_t<T>* node = nullptr;
	};

	iterator_t begin() const{
		return iterator_t(head);
	}
	iterator_t rbegin() const{
		return iterator_t(tail);
	}
	iterator_t end() const{
		return iterator_t();
	}

	void clear(){
		head = tail = nullptr;
	}

	//erase it and return next;
	iterator_t erase(iterator_t it){
		auto* node = it.get();
		auto* next = node->next;
		auto* prev = node->prev;
		if(prev)
			prev->next = next;
		else{
			head = next;
		}
		if(next)
			next->prev = prev;
		else{
			tail = prev;
		}
		return next;
	}
	void insert_front(iterator_t it){
		auto* node = it.get();
		if(head)
			head->prev = node;
		else
			tail = node;

		node->next = head;
		node->prev = nullptr;
		head = node;
	}
	void insert_back(iterator_t it){
		auto* node = it.get();
		if(tail)
			tail->next = node;
		else
			head = node;

		node->prev = tail;
		node->next = nullptr;
		tail = node;

	}
	void move_to_front(intrusive_list_t<T>& other,iterator_t it){
		other.erase(it);
		insert_front(it);
	}
	void move_to_front(iterator_t it){
		erase(it);
		insert_front(it);
	}
	void move_to_back(intrusive_list_t<T>& other,iterator_t it){
		other.erase(it);
		insert_back(it);
	}
	void move_to_back(iterator_t it){
		erase(it);
		insert_back(it);
	}
	bool empty() const{
		return !head;
	}
private:
	intrusive_list_enabled_t<T>* head = nullptr;
	intrusive_list_enabled_t<T>* tail = nullptr;
};

}