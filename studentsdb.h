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
		char FIO[50]; //фио
		char gender; //пол
		int group; //номер группы
		int group_number; //номер в группе
		int points_for_the_test; //баллы за контрольную
		int assessment; //оценка за контрольную
		struct tm timeinfo; //время последнего изменения

		void Display_data() {
			cout << FIO << endl;
			cout << gender << endl;
			cout << "Группа " << group << " номер в группе " << group_number << endl;
			cout << "Баллы за контрольную : " << points_for_the_test << endl;
			cout << "Оценка за контрольную : " << assessment << endl;
			cout << "Последнее изменение данных о студенте: " << asctime(&timeinfo) << endl;
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

	// запись структуры в файл
	template <typename stru>
	void put_in_service_file(char* filename, stru* s)
	{
		FILE* fp;
		char* c;
		int size = sizeof(stru); // количество записываемых байтов

		if ((fp = fopen(filename, "ab")) == NULL)
		{
			perror("Error occured while opening file");
		}
		// устанавливаем указатель на начало структуры
		c = (char*)s;
		// посимвольно записываем в файл структуру
		for (int i = 0; i < size; i++)
		{
			putc(*c++, fp);
		}
		fclose(fp);
	}

	// -------Список групп---------
	struct student_groups {
		Stud** group = 0;
		int info = 0;
		student_groups* next_group = 0;
	};

	void new_group(student_groups*& last_group) {
		int info;
		cout << "Введите номер группы: ";
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

	// загрузка из файла массива структур
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
			cout << "Введите фио: ";
			cin.getline(S[student_group_number]->FIO, 50);
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail());
			do {
				cout << "Пол: ";
				cin >> S[student_group_number]->gender;
				if (!(S[student_group_number]->gender == 'm' or S[student_group_number]->gender == 'f'))
					cout << "Допускаются варианты 'm' или 'f'\n";
				cin.clear();
				cin.ignore(cin.rdbuf()->in_avail());
			} while (!(S[student_group_number]->gender == 'm' or S[student_group_number]->gender == 'f'));

			S[student_group_number]->group = group_info;
			S[student_group_number]->group_number = student_group_number + 1;

			cout << "Баллы за контрольную: ";
			do {
				cin >> S[student_group_number]->points_for_the_test;
				if (!(S[student_group_number]->points_for_the_test >= 0 and S[student_group_number]->points_for_the_test <= 200))
					cout << "От 0 до 200\n";
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
			/*cout << "Оценка за контрольную: ";
			do {
				cin >> S[student_group_number]->assessment;
				if (!(S[student_group_number]->assessment >= 0 and S[student_group_number]->assessment <= 5))
					cout << "Допускаются только оценки по 5-ти бальной шкале, 0 означает отсутствие оценки\n";
				cin.clear();
				cin.ignore(cin.rdbuf()->in_avail());
			} while (!(S[student_group_number]->assessment >= 0 and S[student_group_number]->assessment <= 5));*/

			time_t rawtime;
			time(&rawtime); // текущая дата, выраженная в секундах
			S[student_group_number]->timeinfo = *localtime(&rawtime); // текущая дата, представленная в нормальной форме
			cout << "Текущая дата/время : " << asctime(&S[student_group_number]->timeinfo);

			return 1;
		}
		else {
			cout << "Запись о суденте уже существует\n";

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
		cout << "Количество студентов мужского пола: " << cout_m << "\nженского: " << cout_f << endl;
	}

	void students_recording_time(student_groups* first_group) {
		student_groups* current_group = first_group;
		int day;
		cout << "Вывести все записи, сделанные в день: ";
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
		cout << "Все записи, сделанные после полудня: ";
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
		cout << "Все записи, сделанные до полудня: ";
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

	void students_by_number(student_groups* first_group) {//вывод всех студентов с номером в группе n
		int number;
		cout << "Номер студентов в группе: ";
		do {
			cin >> number;
			if (!(number > 0 and number < 31))
				cout << "Максимальное число студентов в группе - 30, минимальное - 1\n";
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