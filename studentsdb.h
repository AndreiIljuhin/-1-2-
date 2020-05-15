#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

namespace sdb {
	char* int_to_char(int i) {
		char* str = new char[15];
		str[0] = '\0';
		while (i > 0) {
			for (int i = 14; i > 0; i--)
				str[i] = str[i - 1];
			str[0] = (unsigned char)(48 + i % 10);
			i = i / 10;
		}
		return str;
	}

	struct Stud {
		char FIO[50]; //���
		char gender; //���
		int group; //����� ������
		int group_number; //����� � ������
		int points_for_the_test; //����� �� �����������
		int assessment; //������ �� �����������
		struct tm timeinfo; //����� ���������� ���������

		void Display_data() {
			cout << FIO << endl;
			cout << gender << endl;
			cout << "������ " << group << " ����� � ������ " << group_number << endl;
			cout << "����� �� ����������� : " << points_for_the_test << endl;
			cout << "������ �� ����������� : " << assessment << endl;
			cout << "��������� ��������� ������ � ��������: " << asctime(&timeinfo) << endl;
		}

		void put_in_readable_file(char* filename) {
			ofstream file(filename, ios_base::app);
			char* put_str;
			file.write(FIO, strlen(FIO));
			file.put('\n');
			file.put(gender);
			file.put('\n');
			put_str = int_to_char(group);
			file.write(put_str, strlen(put_str));
			file.put('\n');
			put_str = int_to_char(group_number);
			file.write(put_str, strlen(put_str));
			file.put('\n');
			put_str = int_to_char(points_for_the_test);
			file.write(put_str, strlen(put_str));
			file.put('\n');
			file.put((unsigned char)(48 + assessment));
			file.put('\n');
			file.write(asctime(&timeinfo), strlen(asctime(&timeinfo)));
			file.put('\n');
			file.write("-----------------------\n\n", 26);

			file.close();
		}
	};

	// ������ ��������� � ����
	template <typename stru>
	void put_in_service_file(char* filename, stru* s)
	{
		FILE* fp;
		char* c;
		int size = sizeof(stru); // ���������� ������������ ������

		if ((fp = fopen(filename, "ab")) == NULL)
		{
			perror("Error occured while opening file");
		}
		// ������������� ��������� �� ������ ���������
		c = (char*)s;
		// ����������� ���������� � ���� ���������
		for (int i = 0; i < size; i++)
		{
			putc(*c++, fp);
		}
		fclose(fp);
	}

	// -------������ �����---------
	struct student_groups {
		Stud** group = 0;
		int info = 0;
		student_groups* next_group = 0;
	};

	void new_group(student_groups*& last_group) {
		int info;
		cout << "������� ����� ������: ";
		cin >> info;
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		Stud** new_group = new Stud * [30];
		for (int i = 0; i < 30; i++)
			new_group[i] = nullptr;
		student_groups* new_list_group = new student_groups;
		new_list_group->info = info;
		new_list_group->group = new_group;
		last_group->next_group = new_list_group;
		last_group = new_list_group;
	}

	void new_group(student_groups*& last_group, int info) {
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		Stud** new_group = new Stud * [30];
		for (int i = 0; i < 30; i++)
			new_group[i] = nullptr;
		student_groups* new_list_group = new student_groups;
		new_list_group->info = info;
		new_list_group->group = new_group;
		last_group->next_group = new_list_group;
		last_group = new_list_group;
	}

	student_groups* search_group(student_groups* first_group, int group_info) {
		if (first_group == 0)
			return 0;
		while (!(first_group->info == group_info or first_group->next_group == 0))
			first_group = first_group->next_group;

		if (first_group->info == group_info)
			return first_group;
		else return 0;
	}
	// ----------------------------

	// �������� �� ����� ������� ��������
	bool load_data_from_database(student_groups* first_group, student_groups*& last_group, char* filename)
	{
		ifstream file(filename, ios_base::binary);
		if (!file.is_open())
			return 0;
		file.seekg(0, ios::end);
		int size = file.tellg();
		file.clear();
		file.seekg(0);
		for (int i = 0; i < size / sizeof(Stud); i++) {
			Stud* student = new Stud;
			char* c = (char*)student;
			for (int j = 0; j < sizeof(Stud); j++) {
				file.get(*c);
				c++;
			}
			//		student->Display_data();
			student_groups* group = search_group(first_group, student->group);
			if (group == 0) {
				new_group(last_group, student->group);
				group = last_group;
			}
			group->group[student->group_number - 1] = student;
		}

		file.close();
		return 1;
	}

	bool new_student_record(sdb::Stud**& S, int student_group_number, int group_info) {
		if (S[student_group_number] == nullptr) {
			S[student_group_number] = new sdb::Stud;
			cout << "������� ���: ";
			cin.getline(S[student_group_number]->FIO, 50);
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			do {
				cout << "���: ";
				cin >> S[student_group_number]->gender;
				if (!(S[student_group_number]->gender == 'm' or S[student_group_number]->gender == 'f'))
					cout << "����������� �������� 'm' ��� 'f'\n";
				cin.clear();
				cin.ignore(cin.rdbuf()->in_avail());
			} while (!(S[student_group_number]->gender == 'm' or S[student_group_number]->gender == 'f'));

			S[student_group_number]->group = group_info;
			S[student_group_number]->group_number = student_group_number + 1;

			cout << "����� �� �����������: ";
			do {
				cin >> S[student_group_number]->points_for_the_test;
				if (!(S[student_group_number]->points_for_the_test >= 0 and S[student_group_number]->points_for_the_test <= 200))
					cout << "�� 0 �� 200\n";
				cin.clear();
				cin.ignore(cin.rdbuf()->in_avail());
			} while (!(S[student_group_number]->points_for_the_test >= 0 and S[student_group_number]->points_for_the_test <= 200));

			if (S[student_group_number]->points_for_the_test >= 170)
				S[student_group_number]->assessment = 5;
			else if (S[student_group_number]->points_for_the_test >= 140)
				S[student_group_number]->assessment = 4;
			else if (S[student_group_number]->points_for_the_test >= 100)
				S[student_group_number]->assessment = 3;
			else
				S[student_group_number]->assessment = 2;
			/*cout << "������ �� �����������: ";
			do {
				cin >> S[student_group_number]->assessment;
				if (!(S[student_group_number]->assessment >= 0 and S[student_group_number]->assessment <= 5))
					cout << "����������� ������ ������ �� 5-�� ������� �����, 0 �������� ���������� ������\n";
				cin.clear();
				cin.ignore(cin.rdbuf()->in_avail());
			} while (!(S[student_group_number]->assessment >= 0 and S[student_group_number]->assessment <= 5));*/

			time_t rawtime;
			time(&rawtime); // ������� ����, ���������� � ��������
			S[student_group_number]->timeinfo = *localtime(&rawtime); // ������� ����, �������������� � ���������� �����
			cout << "������� ����/����� : " << asctime(&S[student_group_number]->timeinfo);

			return 1;
		}
		else {
			cout << "������ � ������� ��� ����������\n";

			return 0;
		}
	}

	void Display_all_student_data(student_groups* first_group) {
		do {
			for (int i = 0; i < 30; i++) {
				if (first_group->group[i] != nullptr)
					first_group->group[i]->Display_data();
			}
			first_group = first_group->next_group;
		} while (first_group != 0);
	}

	void students_excellent(student_groups* first_group) {
		do {
			for (int i = 0; i < 30; i++) {
				if (first_group->group[i] != nullptr) {
					if (first_group->group[i]->assessment == 5)
						first_group->group[i]->Display_data();
				}

			}
			first_group = first_group->next_group;
		} while (first_group != 0);
	}

	void students_good(student_groups* first_group) {
		do {
			for (int i = 0; i < 30; i++) {
				if (first_group->group[i] != nullptr) {
					if (first_group->group[i]->assessment == 4)
						first_group->group[i]->Display_data();
				}

			}
			first_group = first_group->next_group;
		} while (first_group != 0);
	}

	void students_satisfactory(student_groups* first_group) {
		do {
			for (int i = 0; i < 30; i++) {
				if (first_group->group[i] != nullptr) {
					if (first_group->group[i]->assessment == 3)
						first_group->group[i]->Display_data();
				}

			}
			first_group = first_group->next_group;
		} while (first_group != 0);
	}

	void students_unsatisfactorily(student_groups* first_group) {
		do {
			for (int i = 0; i < 30; i++) {
				if (first_group->group[i] != nullptr) {
					if (first_group->group[i]->assessment < 3)
						first_group->group[i]->Display_data();
				}

			}
			first_group = first_group->next_group;
		} while (first_group != 0);
	}

	void students_gender(student_groups* first_group) {
		int cout_m = 0, cout_f = 0;
		do {
			for (int i = 0; i < 30; i++) {
				if (first_group->group[i] != nullptr) {
					first_group->group[i]->gender == 'm' ? (cout_m++) : (cout_f++);
				}
			}
			first_group = first_group->next_group;
		} while (first_group != 0);
		cout << "���������� ��������� �������� ����: " << cout_m << "\n��������: " << cout_f << endl;
	}

	void students_recording_time(student_groups* first_group) {
		student_groups* current_group = first_group;
		int day;
		cout << "������� ��� ������, ��������� � ����: ";
		cin >> day;
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		do {
			for (int i = 0; i < 30; i++) {
				if (current_group->group[i] != nullptr) {
					if (current_group->group[i]->timeinfo.tm_mday == day)
						current_group->group[i]->Display_data();
				}
			}
			current_group = current_group->next_group;
		} while (current_group != 0);
		current_group = first_group;
		cout << "��� ������, ��������� ����� �������: ";
		do {
			for (int i = 0; i < 30; i++) {
				if (current_group->group[i] != nullptr) {
					if (current_group->group[i]->timeinfo.tm_hour >= 12)
						current_group->group[i]->Display_data();
				}
			}
			current_group = current_group->next_group;
		} while (current_group != 0);
		current_group = first_group;
		cout << "��� ������, ��������� �� �������: ";
		do {
			for (int i = 0; i < 30; i++) {
				if (current_group->group[i] != nullptr) {
					if (current_group->group[i]->timeinfo.tm_hour < 12)
						current_group->group[i]->Display_data();
				}
			}
			current_group = current_group->next_group;
		} while (current_group != 0);
	}

	void students_by_number(student_groups* first_group) {//����� ���� ��������� � ������� � ������ n
		int number;
		cout << "����� ��������� � ������: ";
		do {
			cin >> number;
			if (!(number > 0 and number < 31))
				cout << "������������ ����� ��������� � ������ - 30, ����������� - 1\n";
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
		} while (!(number > 0 and number < 31));
		do {
			if (first_group->group[number - 1] != nullptr) {
				first_group->group[number - 1]->Display_data();
			}
			first_group = first_group->next_group;
		} while (first_group != 0);
	}

	void data_output_to_file(student_groups* first_group) {
		ofstream file((char*)"students.txt");
		file.close();
		file.open((char*)"students_service.txt");
		file.close();
		do {
			for (int i = 0; i < 30; i++) {
				if (first_group->group[i] != nullptr) {
					first_group->group[i]->put_in_readable_file((char*)"students.txt");
					put_in_service_file((char*)"students_service.txt", first_group->group[i]);
				}
			}
			first_group = first_group->next_group;
		} while (first_group != 0);
	}

	bool delete_stud_record(student_groups* first_group, int group_number, int number) {
		student_groups* group;
		group = search_group(first_group, group_number);
		if (group == 0)
			return 0;
		if (group->group[number] == nullptr)
			return 0;
		else {
			delete group->group[number];
			group->group[number] = nullptr;
		}
	}

	void release_memory(student_groups* first_group) {
		while (first_group != 0) {
			if (first_group->group != 0) {
				for (int i = 0; i < 30; i++)
					if (first_group->group[i] != nullptr)
						delete first_group->group[i];
				delete[] first_group->group;
			}

			first_group = first_group->next_group;
		}
	}
}