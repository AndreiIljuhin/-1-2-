#pragma once
#include <iostream>
#include <ctime>
#include <fstream>

namespace dl {
	template<typename T>
	struct doubly_list {//Двусвязный список
		T info;
		doubly_list* previous = nullptr;//Предыдущий
		doubly_list* following = nullptr;//Следующий
	};

	template<typename T>
	void list_output(doubly_list<T>* first_object) {
		int counter = 1;
		while (first_object != nullptr) {
			cout << "[" << counter << "]" << first_object->info << " ";
			first_object = first_object->following;
			counter++;
		}
		cout << endl;
	}

	template<typename T>
	bool delete_list(doubly_list<T>*& first_object) {
		if (first_object == nullptr)
			return 0;
		doubly_list<T>* current_object = first_object;
		doubly_list<T>* next_object = first_object->following;
		while (current_object != nullptr) {
			delete current_object;
			current_object = next_object;
			if (current_object != nullptr)
				next_object = next_object->following;
		}
		first_object = nullptr;
		return 1;
	}

	template<typename T>
	void create_list(doubly_list<T>*& first_object, int N) {
		first_object = new doubly_list<T>;
		doubly_list<T>* current_object = first_object;
		doubly_list<T>* next_object;
		for (int i = 0; i < N - 1; i++) {
			next_object = new doubly_list<T>;
			next_object->previous = current_object;
			current_object->following = next_object;
			current_object = next_object;
		}
	}

	template<typename T>
	bool list_insert_item(doubly_list<T>*& first_object, int N, T info) {
		if (first_object == nullptr) {
			doubly_list<T>* new_object = new doubly_list<T>;
			new_object->info = info;
			first_object = new_object;
			return 0;
		}
		if (N > 0) {
			doubly_list<T>* current_object = first_object;
			for (int i = 1; i < N; i++) {
				if (current_object->following != nullptr)
					current_object = current_object->following;
				else {//Последний в списке
					doubly_list<T>* new_object = new doubly_list<T>;
					new_object->info = info;
					new_object->previous = current_object;
					current_object->following = new_object;
					return 0;
				}
			}
			doubly_list<T>* new_object = new doubly_list<T>;
			new_object->info = info;
			if (N != 1)
				current_object->previous->following = new_object;
			else
				first_object = new_object;
			new_object->previous = current_object->previous;
			new_object->following = current_object;
			current_object->previous = new_object;
		}
		return 1;
	}

	template<typename T>
	bool list_delete_item_by_index(doubly_list<T>*& first_object, int index) {
		if (index > 0) {
			doubly_list<T>* current_object = first_object;
			for (int i = 1; i < index; i++) {
				if (current_object->following != nullptr)
					current_object = current_object->following;
				else
					return 0;//Слишком большой index
			}
			if (current_object->following != nullptr)
				current_object->following->previous = current_object->previous;
			if (current_object->previous != nullptr)
				current_object->previous->following = current_object->following;
			else
				first_object = current_object->following;
			delete current_object;
			return 1;
		}
		else
			return 0;
	}

	template<typename T>
	bool list_delete_item_by_value(doubly_list<T>*& first_object, T value) {
		doubly_list<T>* current_object = first_object;
		while (!(current_object == nullptr or current_object->info == value))
			current_object = current_object->following;
		if (current_object != nullptr) {
			if (current_object->following != nullptr)
				current_object->following->previous = current_object->previous;
			if (current_object->previous != nullptr)
				current_object->previous->following = current_object->following;
			else
				first_object = current_object->following;
			delete current_object;
			return 1;
		}
		else
			return 0;

	}

	template<typename T>
	T list_search_by_index(doubly_list<T>* first_object, int N) {
		doubly_list<T>* current_object = first_object;
		for (int i = 1; i < N; i++)
			if (current_object->following != nullptr)
				current_object = current_object->following;
			else
				return -1;//Слишком большой N
		return current_object->info;
	}

	template<typename T>
	int list_search_by_value(doubly_list<T>* first_object, T value) {
		doubly_list<T>* current_object = first_object;
		int counter = 1;
		while (!(current_object == nullptr or current_object->info == value)) {
			current_object = current_object->following;
			counter++;
		}
		if (current_object == nullptr)
			return -1;
		else
			return counter;
	}

	template<typename T>
	int list_get_length(doubly_list<T>* current_object) {
		int counter = 0;
		while (current_object != nullptr) {
			current_object = current_object->following;
			counter++;
		}
		return counter;
	}

	template<typename T>
	void list_bubbleSort(doubly_list<T>* first_object, int N)
	{
		T temp;
		doubly_list<T>* current_object;
		for (int i = 0; i < N - 1; i++) {
			current_object = first_object;
			for (int j = 0; j < N - i - 1; j++) {
				if (current_object->info > current_object->following->info) {
					temp = current_object->info;
					current_object->info = current_object->following->info;
					current_object->following->info = temp;
				}
				current_object = current_object->following;
			}
		}
	}
}