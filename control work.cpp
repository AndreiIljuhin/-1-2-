#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include "trees.h"
#include "notations.h"
#include "studentsdb.h"
#include "dlist.h"

void control_test() {
	std::cout << "Создание контрольной..\n";
	std::ofstream fileC("control.txt");
	std::ofstream fileA("control-answers.txt");

	for (int j = 1; j <= 10; j++) {
		fileC << "_________________________Вариант " + std::to_string(j) + "_________________________\n\n";
		fileA << "_________________________Вариант " + std::to_string(j) + "_________________________\n\n";
		//------------------------------------//
		trs::NodeT* treeRoot = nullptr;
		trs::NodeT* current = nullptr;
		int treeN = 10;
		for (int i = 1; i <= 2; i++) {
			tree_randomly(treeN, treeRoot, 100);

			fileC << std::to_string(i) + ") Переместите неправильный узел бинарного дерева в правильную позицию\n";
			fileA << std::to_string(i) + ") Ответ к: Переместите неправильный узел бинарного дерева в правильную позицию\n";
			current = treeRoot;
			while (current->left != nullptr and current->right != nullptr and rand() % 4) {
				if (rand() % 2) {
					if (current->left->left != nullptr or current->left->right != nullptr)
						current = current->left;
					else if (current->right->left != nullptr or current->right->right != nullptr)
						current = current->right;
					else
						break;
				}
				else {
					if (current->right->left != nullptr or current->right->right != nullptr)
						current = current->right;
					else if (current->left->left != nullptr or current->left->right != nullptr)
						current = current->left;
					else
						break;
				}
			}
			//правда
			tree_output(fileA, treeRoot, treeN, 1);
			trs::NodeT* temp = current->left;
			current->left = current->right;
			current->right = temp;
			//ложь
			tree_output(fileC, treeRoot, treeN, 1);
			delete_tree(treeRoot);
			treeRoot = nullptr;

			fileC << std::endl;
			fileA << std::endl;
		}
		//------------------------------------//
		//------------------------------------//
		for (int i = 3; i <= 4; i++) {
			treeN = 10;
			tree_randomly(treeN, treeRoot, 100);
			current = treeRoot;
			while (current->left != nullptr and current->right != nullptr and rand() % 5) {
				if (rand() % 2) {
					if (current->left->left != nullptr or current->left->right != nullptr)
						current = current->left;
					else if (current->right->left != nullptr or current->right->right != nullptr)
						current = current->right;
					else
						break;
				}
				else {
					if (current->right->left != nullptr or current->right->right != nullptr)
						current = current->right;
					else if (current->left->left != nullptr or current->left->right != nullptr)
						current = current->left;
					else
						break;
				}
			}
			int delete_items = current->info;
			fileC << std::to_string(i) + ") Удалите элемент \'" << delete_items << "\' из бинарного дерева\n";
			fileA << std::to_string(i) + ") Ответ к: Удалите элемент: \'" << delete_items << "\' из бинарного дерева\n";
			tree_output(fileC, treeRoot, treeN, 1);
			remove_from_tree(treeRoot, delete_items);
			treeN--;
			//правда
			tree_output(fileA, treeRoot, treeN, 1);
			delete_tree(treeRoot);
			treeRoot = nullptr;

			fileC << std::endl;
			fileA << std::endl;
		}
		//------------------------------------//
		//------------------------------------//
		for (int i = 5; i <= 6; i++) {
			treeN = 10;
			tree_randomly(treeN, treeRoot, 100);

			int new_items;
			do {
				current = treeRoot;
				new_items = rand() % 100 - 50;
				current = node_search(current, new_items);
			} while (current->info == new_items);
			fileC << std::to_string(i) + ") Добавьте элемент \'" << new_items << "\' в бинарное дерево\n";
			fileA << std::to_string(i) + ") Ответ к: Добавьте элемент \'" << new_items << "\' в бинарное дерево\n";
			tree_output(fileC, treeRoot, treeN, 1);
			current = treeRoot;
			add_to_tree(current, new_items);
			treeN++;
			//правда
			tree_output(fileA, treeRoot, treeN, 1);

			delete_tree(treeRoot);
			treeRoot = nullptr;

			fileC << std::endl;
			fileA << std::endl;
		}
		//------------------------------------//
		//------------------------------------//
		for (int i = 7; i <= 10; i += 2) {
			nts::test_generation((char*)"expressions.txt", 1);
			//system("expressions.txt");
			std::ifstream expr("expressions.txt");
			char* line = new char[100];
			expr.getline(line, 100, '\n');
			fileC << std::to_string(i) + ") Решите выражение:\n";
			fileA << std::to_string(i) + "," + std::to_string(i + 1) + ") Ответ к: Решите выражения:\n";
			fileC << line << std::endl << std::endl;
			fileA << line << std::endl;
			expr.getline(line, 100, '\n');
			fileC << std::to_string(i + 1) + ") Решите выражение:\n";
			fileC << line << std::endl << std::endl;
			fileA << line << std::endl << std::endl;
			delete[] line;
			expr.close();

			nts::test_solution(fileA, (char*)"expressions.txt");

			//fileC << std::endl;
		}
		//------------------------------------//
		//------------------------------------//
		for (int i = 11; i <= 12; i++) {
			fileC << std::to_string(i) + ") Определите некорректные выражения\n";
			fileA << std::to_string(i) + ") Ответ к: Определите некорректные выражения\n";
			char* expression = new char[50];
			int q_operands, q_operations, cout_operands;
			char* c;

			for (int i = 0; i < 3; i++) {
				bool fail = rand() % 2, truFail = 0;
				cout_operands = q_operands = 5 + rand() % 4;
				q_operations = q_operands - 1;
				for (int i = 0; i < 50; i++)
					expression[i] = '\0';
				char* expression_ptr = expression;
				for (expression_ptr; q_operands or q_operations; 1) {//обратная польская
					if (rand() % 2) {//вероятность добавить операнд - 50%, операцию - 50%, если выполнятся проверки
						if (q_operands) {//если еще остались свободные операнды
							c = nts::int_to_char(rand() % 15 + 1);//выбор значения операнда
							strcat(expression_ptr, c);
							delete[] c;
							while (*expression_ptr != '\0')
								expression_ptr++;
							*expression_ptr = ' ';
							q_operands--;
							expression_ptr++;
						}
					}
					else {
						if (q_operands < q_operations) {		//сели количество операндов меньше количества операций
							*expression_ptr = rand() % 3 + 1;	//соответственно, первые два символа выражения будут операнды (считая операнды больше 9 в скобках за один),
																//а последний будет операцией
							switch (*expression_ptr) {
							case 1:
								*expression_ptr = '+';
								break;
							case 2:
								*expression_ptr = '*';
								break;
							case 3:
								*expression_ptr = '-';
								break;
							}
							q_operations--;
							expression_ptr++;
						}
					}
					if (!(rand() % 3) and fail and !truFail) {
						if (rand() % 2) {
							c = nts::int_to_char(rand() % 15 + 1);//выбор значения операнда
							strcat(expression_ptr, c);
							delete[] c;
							while (*expression_ptr != '\0')
								expression_ptr++;
							*expression_ptr = ' ';
							expression_ptr++;
						}
						else {
							*expression_ptr = rand() % 3 + 1;

							switch (*expression_ptr) {
							case 1:
								*expression_ptr = '+';
								break;
							case 2:
								*expression_ptr = '*';
								break;
							case 3:
								*expression_ptr = '-';
								break;
							}
							expression_ptr++;
						}
						truFail = 1;
					}
				}

				fileA << expression << std::endl;
				fileC << expression << std::endl;
				if (truFail)
					fileA << "- fail" << std::endl;

				fail = rand() % 2;
				truFail = 0;
				q_operands = 5 + 8 - cout_operands;//количество операндов в соответствии с 1 выражением
				q_operations = q_operands - 1;
				for (int i = 0; i < 50; i++)
					expression[i] = '\0';
				expression_ptr = expression;
				for (expression_ptr; q_operands or q_operations; 1) {//обратная польская
					if (rand() % 2) {
						if (q_operands) {
							for (int i = 49; i > 0; i--)//сдвиг выражения вправо
								expression_ptr[i] = expression_ptr[i - 1];
							*expression_ptr = ' ';
							c = nts::int_to_char(rand() % 15 + 1);
							for (int j = strlen(c); j > 0; j--) {//сдвиг вправо на символьную длину числа и его запись
								for (int i = 49; i > 0; i--)
									expression_ptr[i] = expression_ptr[i - 1];
								*expression = c[j - 1];//запись числа с конца
							}
							q_operands--;
						}
					}
					else {
						if (q_operands < q_operations) {
							for (int i = 49; i > 0; i--)
								expression_ptr[i] = expression_ptr[i - 1];
							*expression_ptr = rand() % 3 + 1;//выбор операции
							switch (*expression_ptr) {
							case 1:
								*expression_ptr = '+';
								break;
							case 2:
								*expression_ptr = '*';
								break;
							case 3:
								*expression_ptr = '-';
								break;
							}
							q_operations--;
						}
					}
					if (!(rand() % 3) and fail and !truFail) {
						if (rand() % 2) {
							for (int i = 49; i > 0; i--)//сдвиг выражения вправо
								expression_ptr[i] = expression_ptr[i - 1];
							*expression_ptr = ' ';
							c = nts::int_to_char(rand() % 15 + 1);
							for (int j = strlen(c); j > 0; j--) {//сдвиг вправо на символьную длину числа и его запись
								for (int i = 49; i > 0; i--)
									expression_ptr[i] = expression_ptr[i - 1];
								*expression = c[j - 1];//запись числа с конца
							}
						}
						else {
							for (int i = 49; i > 0; i--)
								expression_ptr[i] = expression_ptr[i - 1];
							*expression_ptr = rand() % 3 + 1;//выбор операции
							switch (*expression_ptr) {
							case 1:
								*expression_ptr = '+';
								break;
							case 2:
								*expression_ptr = '*';
								break;
							case 3:
								*expression_ptr = '-';
								break;

							}
						}
						truFail = 1;
					}

				}

				fileA << expression << std::endl;
				fileC << expression << std::endl;
				if (truFail)
					fileA << "- fail" << std::endl;
			}

			delete[] expression;
			expression = nullptr;

			fileC << std::endl;
			fileA << std::endl;
		}
		//------------------------------------//
		fileC << std::endl;
		fileA << std::endl;
	}

	fileC.close();
	fileA.close();
	system("control.txt");
	system("control-answers.txt");
}

void students_database() {
	std::cout << "Загрузка базы данных..\n";
	sdb::student_groups* first_group;
	sdb::student_groups* last_group;
	first_group = last_group = new sdb::student_groups;
	first_group->group = new sdb::Stud * [30];//Первая группа студентов
	for (int i = 0; i < 30; i++)
		first_group->group[i] = nullptr;
	first_group->info = 9893;
	bool exit = 1;
	load_data_from_database(first_group, last_group, (char*)"students_service.txt");
	ofstream file;

	while (exit) {
		system("cls");
		std::cout << "1 -  Создание новой записи о студенте\n";
		std::cout << "2 -  Вывести информацию о всех студентах\n";
		std::cout << "3 -  Вывод информации о всех студентов группы N\n";
		std::cout << "4 -  Вывод количества студентов мужского и женского пола\n";
		std::cout << "5 -  Вывод всех записей, сделанных в день, который введёт пользователь. Вывод всех записей, сделанных после полудня.Вывод всех записей, сделанных до полудня\n";
		std::cout << "6 -  Вывод данных о студентах, имеющий номер в списке – k\n";
		std::cout << "7 -  Вывод данных о всех студентах в файл students и обновление базы данных\n";
		std::cout << "8 -  Добавить новую группу студентов\n";
		std::cout << "9 -  Удалить запись студента\n";
		std::cout << "10 - Вывести информацию о студентах c оценкой \'отлично\'\n";
		std::cout << "11 - Вывести информацию о студентах c оценкой \'хорошо\'\n";
		std::cout << "12 - Вывести информацию о студентах c оценкой \'удовлетворительно\'\n";
		std::cout << "13 - Вывести информацию о студентах c оценкой \'неудовлетворительно\'\n";
		std::cout << "14 - Выход\n";

		int choice;
		std::cin >> choice;
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		int group_info, student_group_number;
		sdb::student_groups* groups;//Найденные группы помещаем сюда

		switch (choice)
		{
		case 1:
			std::cout << "Номер группы? ";
			std::cin >> group_info;
			groups = search_group(first_group, group_info);
			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());

			if (groups != 0) {
				std::cout << "Номер студента в группе: ";
				do {
					std::cin >> student_group_number;
					if (!(student_group_number > 0 and student_group_number < 31))
						std::cout << "Максимальное число студентов в группе - 30, минимальное - 1\n";
					std::cin.clear();
					std::cin.ignore(std::cin.rdbuf()->in_avail());
				} while (!(student_group_number > 0 and student_group_number < 31));
				new_student_record(groups->group, student_group_number - 1, group_info);
			}
			else cout << "Группы нет в списке\n";
			break;

		case 2:
			Display_all_student_data(first_group);
			break;

		case 3:
			cout << "Номер группы? ";
			std::cin >> group_info;
			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			groups = search_group(first_group, group_info);

			if (groups != 0) {
				for (int i = 0; i < 30; i++) {
					if (groups->group[i] != nullptr)
						groups->group[i]->Display_data();
				}
			}
			else cout << "Группы нет в списке\n";
			break;

		case 4:
			students_gender(first_group);
			break;

		case 5:
			students_recording_time(first_group);
			break;

		case 6:
			students_by_number(first_group);
			break;

		case 7:
			data_output_to_file(first_group);
			system("students.txt");
			break;

		case 8:
			new_group(last_group);
			break;

		case 9:
			std::cout << "Номер группы студента: ";
			std::cin >> group_info;
			std::cout << "Номер студента в группе: ";
			do {
				std::cin >> student_group_number;
				if (!(student_group_number > 0 and student_group_number < 31))
					std::cout << "Максимальное число студентов в группе - 30, минимальное - 1\n";
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
			} while (!(student_group_number > 0 and student_group_number < 31));

			if (delete_stud_record(first_group, group_info, student_group_number - 1))
				std::cout << "Запись удалена\n";
			else
				std::cout << "Запись не найдена\n";

			break;

		case 10:
			students_excellent(first_group);
			break;

		case 11:
			students_good(first_group);
			break;

		case 12:
			students_satisfactory(first_group);
			break;

		case 13:
			students_unsatisfactorily(first_group);
			break;

		case 14:
			exit = 0;
			break;
		}

		std::cout << endl;
		if (exit)
			system("Pause");
	}
	release_memory(first_group);
}

bool add_to_turn(dl::doubly_list<sdb::Stud>*& students_turn, sdb::Stud student) {
	//наибольший приоритет имеют люди с низкими оценками по контрольной
	//люди с одинаковыми оценками встают в очередь по алфавиту
	//люди с одинаковыми оценками и первыми буквами Фамилии встают по баллам за контрольную, меньше балл - больше приоритет

	if (students_turn == nullptr) {
		list_insert_item(students_turn, 0, student);
		return 0;
	}

	int N = 1;
	dl::doubly_list<sdb::Stud>* current = students_turn;
	while (current != nullptr) {
		if (current->info.assessment > student.assessment) {
			list_insert_item(students_turn, N, student);
			return 1;
		}
		if (current->info.assessment == student.assessment) {
			if (*current->info.FIO > * student.FIO) {
				list_insert_item(students_turn, N, student);
				return 1;
			}
			if (*current->info.FIO == * student.FIO) {
				if (current->info.points_for_the_test > student.points_for_the_test) {
					list_insert_item(students_turn, N, student);
					return 1;
				}
			}
		}
		N++;
		current = current->following;
	}
	list_insert_item(students_turn, N, student);
	return 0;
}

void exam_day() {
	std::cout << "Загрузка базы данных 2 задания..\n";
	sdb::student_groups* first_group;
	sdb::student_groups* last_group;
	first_group = last_group = new sdb::student_groups;
	first_group->group = new sdb::Stud * [30];
	for (int i = 0; i < 30; i++)
		first_group->group[i] = nullptr;
	first_group->info = 9893;
	bool exit = 1;
	load_data_from_database(first_group, last_group, (char*)"students_service.txt");
	ofstream infile;
	ifstream file;

	dl::doubly_list<sdb::Stud>* studentsHdb1 = nullptr;
	dl::doubly_list<sdb::Stud>* current = nullptr;
	int listNdb1 = 0;

	sdb::student_groups* current_group = first_group; //перенос студентов из базы данных
	while (current_group) {
		for (int i = 0; i < 30; i++) {
			if (current_group->group[i] != nullptr) {
				list_insert_item(studentsHdb1, listNdb1, *current_group->group[i]);
				listNdb1++;
			}
		}
		current_group = current_group->next_group;
	}
	release_memory(first_group);
	first_group = last_group = 0;

	sdb::Stud* currentSt = nullptr;
	std::cout << "Загрузка базы данных 3 задания..\n";
	dl::doubly_list<sdb::Stud>* studentsHdb2 = nullptr;
	int listNdb2 = 0;
	file.open("students2.txt", std::ios_base::binary);
	if (file.is_open()) {
		currentSt = new sdb::Stud;
		while (file.read((char*)currentSt, sizeof(*currentSt))) {
			list_insert_item(studentsHdb2, listNdb2, *currentSt);
			listNdb2++;
		}
		file.close();
	}

	dl::doubly_list<sdb::Stud>* students_turn = nullptr;//очередь студентов
	//внесение в очередь по правилам*
	if (studentsHdb1 != nullptr) {
		current = studentsHdb1;
		for (int i = 0; i < listNdb1; i++) {
			add_to_turn(students_turn, current->info);
			current = current->following;
		}
	}
	if (studentsHdb2 != nullptr) {
		current = studentsHdb2;
		for (int i = 0; i < listNdb2; i++) {
			add_to_turn(students_turn, current->info);
			current = current->following;
		}
	}

	//------------------------------//
	while (1) {
		system("Pause");
		int choice = 0;
		system("cls");
		std::cout << "Правила внесения в очередь:\n";
		std::cout << "наибольший приоритет имеют люди с низкими оценками по контрольной\n";
		std::cout << "люди с одинаковыми оценками встают в очередь по алфавиту\n";
		std::cout << "люди с одинаковыми оценками и первыми буквами Фамилии встают по баллам\n за контрольную, меньше балл - больше приоритет\n";
		std::cout << "1 - Вывести текущую очередь\n";
		std::cout << "2 - Добавить новго студента\n";
		std::cout << "3 - Выход в главное меню\n";

		std::cin >> choice;
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		if (choice == 1) {
			if (students_turn != nullptr) {
				current = students_turn;
				int counter = 1;
				while (current != nullptr) {
					std::cout << "№ " << counter << std::endl;
					current->info.Display_data();
					current = current->following;
					counter++;
				}
			}
			else
				std::cout << "В очереди пока никого нет\n";
		}

		if (choice == 2) {
			currentSt = new sdb::Stud;
			cout << "Введите фио: ";
			std::cin.getline(currentSt->FIO, 50);
			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			do {
				cout << "Пол: ";
				std::cin >> currentSt->gender;
				if (!(currentSt->gender == 'm' or currentSt->gender == 'f'))
					std::cout << "Допускаются варианты 'm' или 'f'\n";
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
			} while (!(currentSt->gender == 'm' or currentSt->gender == 'f'));

			cout << "Номер группы: ";
			std::cin >> currentSt->group;
			std::cin.clear();
			std::cin.ignore(std::cin.rdbuf()->in_avail());

			cout << "Номер в группе: ";
			do {
				std::cin >> currentSt->group_number;
				if (!(currentSt->group_number > 0 and currentSt->group_number < 31))
					std::cout << "Максимальное число студентов в группе - 30, минимальное - 1\n";
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
			} while (!(currentSt->group_number > 0 and currentSt->group_number < 31));

			cout << "Баллы за контрольную: ";
			do {
				std::cin >> currentSt->points_for_the_test;
				if (!(currentSt->points_for_the_test >= 0 and currentSt->points_for_the_test <= 200))
					std::cout << "От 0 до 200\n";
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
			} while (!(currentSt->points_for_the_test >= 0 and currentSt->points_for_the_test <= 200));

			if (currentSt->points_for_the_test >= 170)
				currentSt->assessment = 5;
			else if (currentSt->points_for_the_test >= 140)
				currentSt->assessment = 4;
			else if (currentSt->points_for_the_test >= 100)
				currentSt->assessment = 3;
			else
				currentSt->assessment = 2;

			time_t rawtime;
			time(&rawtime); // текущая дата, выраженная в секундах
			currentSt->timeinfo = *localtime(&rawtime); // текущая дата, представленная в нормальной форме
			cout << "Текущая дата/время : " << asctime(&currentSt->timeinfo);

			list_insert_item(studentsHdb2, listNdb2, *currentSt);
			listNdb2++;
			add_to_turn(students_turn, *currentSt);
		}

		if (choice == 3)
			break;

	}
	//------------------------------//

	//сохранение дб 2
	if (studentsHdb2 != nullptr) {
		infile.open("students2.txt");
		current = studentsHdb2;
		for (int i = 0; i < listNdb2; i++) {
			infile.write((char*)& current->info, sizeof(current->info));
			current = current->following;
		}
		infile.close();
	}
	delete_list(studentsHdb1);
	delete_list(studentsHdb2);
	delete_list(students_turn);
	system("Pause");
}

int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(0));
	std::ifstream file;
	std::ofstream infile;
	int choice;
	bool exit = 1;

	while (exit) {
		system("cls");
		std::cout << "1 - Первое задание\n";
		std::cout << "2 - Второе задание\n";
		std::cout << "3 - Третье задание\n";
		std::cout << "4 - Выход\n";
		std::cin >> choice;
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());

		switch (choice)
		{
		case 1:
			control_test();
			break;
		case 2:
			students_database();
			break;
		case 3:
			exam_day();
			break;
		case 4:
			exit = 0;
			break;
		}
	}

}
