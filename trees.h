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

	NodeT* node_search(NodeT* current, NodeT*& previous, int value) {//����� ������ � ����������
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
		NodeT* current = node_search(root, value);//����� ��� �������
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
		NodeT* current = node_search(root, previous, value);//��������� ����
		if (value != current->info)
			return 0;

		if (current->left != nullptr and current->right != nullptr) {//���� ���� ������ � �����, �� ������ ������ �� ������ �������
			NodeT* place = current->left;							 //���������� �������� � ������ ������� ������ � ����������� ����������
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
		else if (current->left != nullptr) {	//����� ������ ������ ���� � ������ �� ��� ����� ���������
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
		cout << "������� ������������� �������� ��������� ������, ��� ����� ������ ������� ����� ������ ��� �����\n";
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
		cout << "������� �������� ��������� ������ � ���� ����� ������\n";
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
				cout << "������ ������� �� " << 0.000000001 * elapsed_seconds << " �������.\n";
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
			for (int i = 14; i > 0; i--)//����� ������
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

	void right_shift_matrix(int** matrix, int i, int j, int N) {//����� �� 1 ������ // j ���������� ������
		for (int q = i; q >= 0; q--) {
			for (int k = N * 3 + 2 - 1; k > j; k--)
				matrix[q][k] = matrix[q][k - 1];
			if (matrix[q][j] != -999998)//���� �� �������������� �����
				matrix[q][j] = -999999;
		}
	}

	void tree_output(ofstream& file, const NodeT* root, int N, bool in_file) {
		if (root != nullptr) {

			List<const NodeT*>* head, * current, * new_t;
			head = new List<const NodeT*>;
			head->info = root;
			int** matrix;//����� ����� ���������� ������ �����
			int* entry_points = new int[N / 2];//�����, �������� ��������� ����� � ������� ��� ���������� ������ ������
			int i = 0, j = 0, entry_tail = 1;
			matrix = new int* [N * 2];
			for (int i = 0; i < N * 2; i++) {
				matrix[i] = new int[N * 3 + 2];
				for (int j = 0; j < N * 3 + 2; j++)
					matrix[i][j] = -999999;//-999999 �������� ������ ' '
			}
			entry_points[0] = 1;//����������� ��������� �� x ����� ������

			while (head != nullptr) {
				j = 0;
				current = head;
				while (current != nullptr) {//������ ���������� � �������
					matrix[i][entry_points[j]] = current->info->info;
					if (current->info->left != nullptr) {//���������� ����� ����� ��� ����
						for (int q = 0; q < 3; q++) {//����� ���� ����������� ���������
							right_shift_matrix(matrix, i + 1, entry_points[j], N);
							for (int q = j + 1; q < entry_tail; q++)//���������� ���� ����� ������
								entry_points[q]++;
							matrix[i + 1][entry_points[j]] = -999998;//-999998 �������� ������ '-'
						}
						entry_tail++;
						for (int i = entry_tail; i > j; i--)	  //entry_points[j] ������ ����� ���� ���������� ������, ������� ����� ������
							entry_points[i] = entry_points[i - 1];//��� �� ������������,
						j++;									  //��������� j ���������� � ������� �����,
						entry_points[j] += 3;					  //���������� ������� ����� ���� �� 3(��� ��� ������� ���������� �� 3 ������� ������)
					}

					if (current->info->right != nullptr) {//���������� ������ ����� ����
						for (int q = 0; q < 3; q++) {
							right_shift_matrix(matrix, i + 1, entry_points[j] + 1, N);
							for (int q = j + 1; q < entry_tail; q++)
								entry_points[q]++;
							matrix[i + 1][entry_points[j] + 1] = -999998;
						}
						entry_points[j] += 3;//����� ������ ���� �� ����� � �������� � ����������
						j++;
					}
					else {//���� ��� ������� ����, ������ �����
						for (int i = j; i < entry_tail; i++)//����� ����� ��������� � ��������� �����
							entry_points[i] = entry_points[i + 1];
						entry_tail--;
					}
					current = current->next;
				}
				current = head;
				while (current != nullptr) {//������� � ��������� ����� ��������� �� ���������� ���������� �� �������
					bool b = 0;
					if (current->info->right != nullptr) {//���������� ������� �������� ������ �� ����� ����� �������
						new_t = new List<const NodeT*>;
						new_t->next = current->next;
						current->next = new_t;
						new_t->info = current->info->right;
						b = 1;
					}
					if (current->info->left != nullptr) {//������ �������� ������
						current->info = current->info->left;
						current = current->next;
					}
					else {//�������� �������� ������
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
					if (b)//������� ������������ ������� ��������
						current = current->next;
				}
				i += 2;
			}
			for (int q = 0; q < i - 1; q++) {//����� ������ � ����
				if (q % 2 == 0) {//������������ ������� � �����
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
								file << (unsigned char)149;//� ������� ��� ����� �������
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
				bool b = 0;//b �������� ������ ������ ������ ��� ����� �����, ��� ��� ������ ����� ������� ������� ������� '-' ������� ����� �����
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
					else if (matrix[q][j] == -999998) {//������ ����� ����� ����� ����� � ������ ������
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
						j += strlen(str) - 1;//��������� n ��������, n ����� ����� ������ ����� - 1
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