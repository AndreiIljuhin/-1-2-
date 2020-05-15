#pragma once
#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>
using namespace std;

namespace trs {
	void stream_cleaning() {
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
	}

	struct NodeT {
		int info;
		NodeT* left = nullptr;
		NodeT* right = nullptr;
	};

	NodeT* node_search(NodeT* current, int value) {
		if (current == nullptr)
			return nullptr;
		if (value > current->info) {
			if (current->right != nullptr)
				current = node_search(current->right, value);
		}
		else if (value < current->info) {
			if (current->left != nullptr)
				current = node_search(current->left, value);
		}
		return current;
	}

	NodeT* node_search(NodeT* current, NodeT*& previous, int value) {//поиск вместе с предыдущим
		if (current == nullptr)
			return nullptr;
		if (value > current->info) {
			if (current->right != nullptr) {
				previous = current;
				current = node_search(current->right, previous, value);
			}
		}
		else if (value < current->info) {
			if (current->left != nullptr) {
				previous = current;
				current = node_search(current->left, previous, value);
			}
		}
		return current;
	}

	void tree_direct_walk(NodeT* root) {
		cout << root->info << " ";
		if (root->left != nullptr)
			tree_direct_walk(root->left);
		if (root->right != nullptr)
			tree_direct_walk(root->right);
	}

	void tree_reverse_walk(NodeT* root) {
		if (root->left != nullptr)
			tree_reverse_walk(root->left);
		if (root->right != nullptr)
			tree_reverse_walk(root->right);
		cout << root->info << " ";
	}

	void tree_symmetrical_walk(NodeT* root) {
		if (root->left != nullptr)
			tree_symmetrical_walk(root->left);
		cout << root->info << " ";
		if (root->right != nullptr)
			tree_symmetrical_walk(root->right);
	}

	bool add_to_tree(NodeT* root, int value) {
		NodeT* current = node_search(root, value);//место для вставки
		if (current == nullptr)
			return 0;
		if (value == current->info)
			return 0;
		NodeT* new_g;
		if (value > current->info) {
			new_g = new NodeT;
			current->right = new_g;
			new_g->info = value;
		}
		if (value < current->info) {
			new_g = new NodeT;
			current->left = new_g;
			new_g->info = value;
		}
		return 1;
	}

	bool remove_from_tree(NodeT*& root, int value) {
		if (root == nullptr)
			return 0;
		NodeT* previous = nullptr;
		NodeT* current = node_search(root, previous, value);//ближайший узел
		if (value != current->info)
			return 0;

		if (current->left != nullptr and current->right != nullptr) {//если есть правый и левый, то цепляю правый на правую сторону
			NodeT* place = current->left;							 //последнего элемента в правой цепочке левого и переписываю предыдущий
			while (place->right != nullptr)
				place = place->right;
			if (previous != nullptr) {
				if (previous->info < current->info)
					previous->right = current->left;
				else
					previous->left = current->left;
			}
			else
				root = current->left;
			place->right = current->right;
			delete current;
		}
		else if (current->left != nullptr) {	//иначе просто удаляю узел и ставлю на его место следующий
			if (previous != nullptr) {
				if (previous->info < current->info)
					previous->right = current->left;
				else
					previous->left = current->left;
			}
			else
				root = current->left;
			delete current;
		}
		else if (current->right != nullptr) {
			if (previous != nullptr) {
				if (previous->info < current->info)
					previous->right = current->right;
				else
					previous->left = current->right;
			}
			else
				root = current->right;
			delete current;
		}
		else {
			if (previous != nullptr) {
				if (previous->info < current->info)
					previous->right = nullptr;
				else
					previous->left = nullptr;
			}
			else
				root = nullptr;
			delete current;
		}

		return 1;
	}

	void tree_randomly(int N, NodeT*& root, int range) {
		root = new NodeT;
		root->info = rand() % range - range / 2;
		for (int i = 1; i < N; i++)
			if (!add_to_tree(root, rand() % range - range / 2))
				i--;
	}

	bool tree_with_keyboard(int& N, NodeT*& root) {
		cout << "Вводите целочисленные элементы бинарного дерева, для конца записи введите любой символ или букву\n";
		int value;
		while (1) {
			if (cin >> value) {
				if (root == nullptr) {
					root = new NodeT;
					root->info = value;
					N++;
				}
				else if (add_to_tree(root, value))
					N++;
				stream_cleaning();
			}
			else {
				stream_cleaning();
				if (N == 0)
					return 0;
				return 1;
			}
		}
	}

	bool tree_from_file(int& N, NodeT*& root) {
		cout << "Введите элементы бинарного дерева в файл через пробел\n";
		fstream file("file.txt", ios_base::out);
		file.close();
		system("file.txt");
		chrono::time_point<chrono::high_resolution_clock> start, end;
		double elapsed_seconds;
		start = chrono::high_resolution_clock::now();
		file.open("file.txt", ios_base::in);
		char ch;
		int value;
		while (1) {
			if (file >> value) {
				if (root == nullptr) {
					root = new NodeT;
					root->info = value;
					N++;
				}
				else if (add_to_tree(root, value))
					N++;
			}
			else if (file >> ch);
			else {
				if (N == 0)
					return 0;
				end = chrono::high_resolution_clock::now();
				elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				cout << "Дерево создано за " << 0.000000001 * elapsed_seconds << " секунды.\n";
				return 1;
			}
		}
		file.close();
	}

	void delete_tree(NodeT* root) {
		if (root->left != nullptr)
			delete_tree(root->left);
		if (root->right != nullptr)
			delete_tree(root->right);
		delete root;
	}

	char* int_to_char(int i) {
		char* str = new char[15];
		bool minus = 0;
		if (i < 0) {
			i = abs(i);
			minus = 1;
		}
		else if (i == 0) {
			str[0] = (unsigned char)(48);
			str[1] = '\0';
			return str;
		}
		str[0] = '\0';
		while (i > 0) {
			for (int i = 14; i > 0; i--)//сдвиг вправо
				str[i] = str[i - 1];
			str[0] = (unsigned char)(48 + i % 10);
			i = i / 10;
		}
		if (minus) {
			for (int i = 14; i > 0; i--)
				str[i] = str[i - 1];
			str[0] = '-';
		}
		return str;
	}

	template<class T>
	struct List {
		T info;
		List* next = nullptr;
	};

	void right_shift_matrix(int** matrix, int i, int j, int N) {//сдвиг на 1 вправо // j становится пустой
		for (int q = i; q >= 0; q--) {
			for (int k = N * 3 + 2 - 1; k > j; k--)
				matrix[q][k] = matrix[q][k - 1];
			if (matrix[q][j] != -999998)//если не горизонтальная линия
				matrix[q][j] = -999999;
		}
	}

	void tree_output(ofstream& file, const NodeT* root, int N, bool in_file) {
		if (root != nullptr) {

			List<const NodeT*>* head, * current, * new_t;
			head = new List<const NodeT*>;
			head->info = root;
			int** matrix;//здесь будет находиться дерево графа
			int* entry_points = new int[N / 2];//точки, хранящие положение узлов в матрице для следующего уровня дерева
			int i = 0, j = 0, entry_tail = 1;
			matrix = new int* [N * 2];
			for (int i = 0; i < N * 2; i++) {
				matrix[i] = new int[N * 3 + 2];
				for (int j = 0; j < N * 3 + 2; j++)
					matrix[i][j] = -999999;//-999999 означает символ ' '
			}
			entry_points[0] = 1;//изначальное положение по x корня дерева

			while (head != nullptr) {
				j = 0;
				current = head;
				while (current != nullptr) {//запись информации в матрицу
					matrix[i][entry_points[j]] = current->info->info;
					if (current->info->left != nullptr) {//добавление левой ветки для узла
						for (int q = 0; q < 3; q++) {//сдвиг всех вышестоящих элементов
							right_shift_matrix(matrix, i + 1, entry_points[j], N);
							for (int q = j + 1; q < entry_tail; q++)//увеличение всех точек справа
								entry_points[q]++;
							matrix[i + 1][entry_points[j]] = -999998;//-999998 означает символ '-'
						}
						entry_tail++;
						for (int i = entry_tail; i > j; i--)	  //entry_points[j] хранит точку узла следующего уровня, поэтому сдвиг вправо
							entry_points[i] = entry_points[i - 1];//для ее дублирования,
						j++;									  //инкремент j возвращает к текущей точке,
						entry_points[j] += 3;					  //увеличение текущей точки узла на 3(так как матрица сдвинулась на 3 позиции вправо)
					}

					if (current->info->right != nullptr) {//добавление правой ветки узла
						for (int q = 0; q < 3; q++) {
							right_shift_matrix(matrix, i + 1, entry_points[j] + 1, N);
							for (int q = j + 1; q < entry_tail; q++)
								entry_points[q]++;
							matrix[i + 1][entry_points[j] + 1] = -999998;
						}
						entry_points[j] += 3;//меняю старый узел на новый и перехожу к следующему
						j++;
					}
					else {//если нет правого узла, удаляю точку
						for (int i = j; i < entry_tail; i++)//сдвиг влево переходит к следующей точке
							entry_points[i] = entry_points[i + 1];
						entry_tail--;
					}
					current = current->next;
				}
				current = head;
				while (current != nullptr) {//переход к следующей линии элементов на одинаковом расстоянии от вершины
					bool b = 0;
					if (current->info->right != nullptr) {//добавление правого элемента списка на место перед текущим
						new_t = new List<const NodeT*>;
						new_t->next = current->next;
						current->next = new_t;
						new_t->info = current->info->right;
						b = 1;
					}
					if (current->info->left != nullptr) {//замена элемента списка
						current->info = current->info->left;
						current = current->next;
					}
					else {//удаление элемента списка
						if (current == head) {
							head = current->next;
							delete current;
							current = head;
						}
						else {
							new_t = current;
							current = head;
							while (current->next != new_t)
								current = current->next;
							current->next = new_t->next;
							delete new_t;
							current = current->next;
						}
					}
					if (b)//пропуск добавленного правого элемента
						current = current->next;
				}
				i += 2;
			}
			for (int q = 0; q < i - 1; q++) {//вывод дерева в файл
				if (q % 2 == 0) {//формирование стрелок к узлам
					for (j = 0; j < N * 3 + 2; j++) {
						if (matrix[q][j] > -999998) {
							if (in_file)
								file << "|";
							else
								cout << "|";
							j++;
							while (matrix[q][j] > -999998 and j < N * 3 + 2) {
								j++;
								if (in_file)
									file << " ";
								else
									cout << " ";
							}
							j--;
						}
						else if (in_file)
							file << " ";
						else
							cout << " ";
					}
					if (in_file)
						file << endl;
					else
						cout << endl;
					if (in_file) {
						for (j = 0; j < N * 3 + 2; j++) {
							if (matrix[q][j] > -999998) {
								file << (unsigned char)149;//в консоле нет этого символа
								j++;
								while (matrix[q][j] > -999998 and j < N * 3 + 2) {
									j++;
									file << " ";
								}
								j--;
							}
							else
								file << " ";
						}
						file << endl;
					}
				}
				bool b = 0;//b означает первый символ правой или левой ветки, так как иногда после сдвигов матрицы символы '-' выходят левее узлов
				for (j = 0; j < N * 3 + 2; j++) {
					if (!b and matrix[q][j] == -999998 and (matrix[q + 1][j] != -999999 or matrix[q - 1][j - 1] != -999999))
						b = 1;
					if (b and matrix[q][j] == -999999)
						b = 0;
					if (matrix[q][j] == -999999 or matrix[q][j] == -999998 and !b) {
						matrix[q][j] = -999999;
						if (in_file)
							file << " ";
						else
							cout << " ";
					}
					else if (matrix[q][j] == -999998) {//нахожу левую точку левой ветки и правую правой
						if (matrix[q + 1][j] != -999999 and matrix[q][j - 1] == -999999)
							if (in_file)
								file << ",";
							else
								cout << ",";
						else if (matrix[q + 1][j] != -999999)
							if (in_file)
								file << ".";
							else
								cout << ".";
						else if (in_file)
							file << "-";
						else
							cout << "-";
					}
					else {
						if (in_file)
							file << matrix[q][j];
						else
							cout << matrix[q][j];
						char* str = int_to_char(matrix[q][j]);
						j += strlen(str) - 1;//пропускаю n пробелов, n равно длине строки числа - 1
						delete[] str;
					}
				}
				if (in_file)
					file << endl;
				else
					cout << endl;
			}

			for (i = 0; i < N * 2; i++)
				delete[] matrix[i];
			delete[] matrix;
		}
	}
}