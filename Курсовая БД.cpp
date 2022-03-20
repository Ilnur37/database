#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

struct Customer {
	string surname;			//Фамилия	
	string name;			//Имя
	string patronymic;		//Отчество
							
	string city;			//Город
	string street;			//Улица
	int houseNumb;			//Номер дома
	int apartmentNumb;		//Номер квартиры
	int accountNumber;		//Номер счета

	Customer* pNext = nullptr;		   //Указатель на следующий элемент
	Customer* pPred = nullptr;		   //Указатель на предыдущий элемент
	static int size;				//Количество элементов в базе данных
};

Customer* firstUser = nullptr;		//Голова списка
Customer* lastUser = nullptr;		//Конец списка
Customer* currentUser = nullptr;	//Временный элемент

int Customer::size = 0;

ifstream File("Data.txt");

//Меню
void Menu();

//Вывод данных в консоль
void PrintList();

//Вывод данных одного пользователя в консоль
void PrintData(int i);

//Считывание данных из файла
void FileInput();

//Добавление данных о покупателе
void PushBack();

//Вывод данных о покупателях в файл
void SaveToFile();

//Удаление данных пользователя из конца списка
void PopBack();

//Удаление данных пользователя из начала списка
void PopFront();

//Удаление данных пользователя под введенным индексом
void Remove();

//Выбор поля
void SelectionOptions(int& buffer);

//Выборка значений по заданному условию
void Search(const int buffer);

//Пузырьковая сортировка
void Sort(const int buffer);

//Поменять местами данные о 2-х покупателях
void Swap(Customer* temp1, Customer* temp2);

//Сравнение значей одного поля у двух пользователей 
bool Comparison(const int buffer);

//Функция для возврашения значения поля элемента структуры
template<typename T> T ReturnField(Customer* currentUser, int buffer);
template<> string ReturnField(Customer* currentUser, int buffer);
template<> int ReturnField(Customer* currentUser, int buffer);

//Удаление всего списка
void Delete();

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int field;
	int choiceCase;

	while (true) {
		Menu();
	
		cin >> choiceCase;
		switch (choiceCase) {
		case 1:
			cout << "\n\nСчитывание данных о покупателях\n";
			while (!File.eof()) FileInput();
			cout << "Данные успешно считаны!\n";
			continue;

		case 2:
			cout << "\n\nВведите данные, которые нужно добавить: \n";
			PushBack();
			PrintList();
			continue;

		case 3:
			PrintList();
			continue;

		case 4:
			cout << "\n\nДанные сохранены в файле NeaData.txt!\n";
			SaveToFile();
			continue;

		case 5:
			cout << "\n\n";
			Remove();
			PrintList();
			continue;

		case 6:
			cout << "\n\nПроизводится удаление всех данных\n";
			Delete();
			cout << "База данных пуста!" << endl;
			continue;

		case 7:
			cout << "\n\nВыборка значений по заданному условию\n";
			SelectionOptions(field);
			Search(field);
			continue;

		case 8:
			if (firstUser != nullptr) {
				cout << "\n\nСортировка по определенному значению\n";
				SelectionOptions(field);
				Sort(field);
				cout << "\nОтсортированный список покупателей:\n";
				PrintList();
			}
			else  cout << "База данных пуста!" << endl;
			continue;

		case 0:
			cout << "\n\nПрограмма успешно завершена." << endl;
			Delete();
			return 0;

		default:
			cout << "\nВведенное значение не распознано." << endl;
			continue;
		}
	}
}

void Menu() {
	cout << "\n\nВыберите действие:" << endl;
	cout << "1: Считать данные о покупателях с файла.\n";
	cout << "2: Добавить данные о пользователе.\n";
	cout << "3: Вывести данные о покупателях на экран.\n";
	cout << "4: Вывести данные о покупателях в файл.\n";
	cout << "5: Произвести удаление данных пользователя.\n";
	cout << "6: Произвести удаление всех данных.\n";
	cout << "7: Выборка значений по заданному условию\n";
	cout << "8: Сортировка по определенному значению\n";
	cout << "0: Удаление данных и выход и программы.\n";
}

//Вывод данных в консоль
void PrintList() {
	//Проверка на пустоту базы данных
	if (firstUser == nullptr) {
		cout << "База данных пуста!" << endl;
		return;
	}
	cout << "Актуальный список покупателей:\n";
	int i = 1;
	//Создаётся шапка таблицы
	cout << setfill(' ')
	<< "|" << setw(4) << " "
	<< "|" << setw(16) << "Фамилия"
	<< "|" << setw(16) << "Имя"
	<< "|" << setw(16) << "Отчество"
	<< "|" << setw(16) << "Город"
	<< "|" << setw(16) << "Улица"
	<< "|" << setw(16) << "Номер дома"
	<< "|" << setw(16) << "Номер квартиры"
	<< "|" << setw(16) << "Номер счета"
	<< "|\n";
	currentUser = firstUser;
	//Цикл для вывода каждого экзкмпляра структуры
	while (currentUser != nullptr) {
		PrintData(i);
		currentUser = currentUser->pNext;
		i++;
	}
}

//Вывод данных одного пользователя в консоль
void PrintData(int i) {
	cout << setfill(' ') << "|" << setw(4) << i << "|"
		<< setw(16) << currentUser->surname << "|"
		<< setw(16) << currentUser->name << "|"
		<< setw(16) << currentUser->patronymic << "|"
		<< setw(16) << currentUser->city << "|"
		<< setw(16) << currentUser->street << "|"
		<< setw(16) << currentUser->houseNumb << "|"
		<< setw(16) << currentUser->apartmentNumb << "|"
		<< setw(16) << currentUser->accountNumber << "|\n";
}

//Считывание данных из файла
void FileInput() {
	if (firstUser == nullptr) {						//Если список ещё пуст
		firstUser = new Customer;  			   	   		
		lastUser = firstUser;
	}
	else if (firstUser->pNext == nullptr) {			//Если в списке только 1 покупатель
			lastUser = new Customer;
			firstUser->pNext = lastUser;
			lastUser->pPred = firstUser;
	}
	else {											//Остальные случаи
		currentUser = lastUser;
		lastUser = new Customer;
		currentUser->pNext = lastUser;
		lastUser->pPred = currentUser;
	}

	File >> lastUser->surname;
	File >> lastUser->name;
	File >> lastUser->patronymic;
	File >> lastUser->city;
	File >> lastUser->street;
	File >> lastUser->houseNumb;
	File >> lastUser->apartmentNumb;
	File >> lastUser->accountNumber;
	Customer::size++;
}

//Добавдение данных о покупателе
void PushBack() {
	string surname;
	string name;
	string patronymic;

	string city;
	string street;
	
	if (firstUser == nullptr) {						//Если список ещё пуст
		firstUser = new Customer;  			   	   		
		lastUser = firstUser;
	}
	else if (firstUser->pNext == nullptr) {			//Если в списке только 1 покупатель
		lastUser = new Customer;
		firstUser->pNext = lastUser;
		lastUser->pPred = firstUser;
	}
	else {											//Остальные случаи
		currentUser = lastUser;
		lastUser = new Customer;
		currentUser->pNext = lastUser;
		lastUser->pPred = currentUser;
	}

	// Заполнение элемента
	cout << " Введите фамилию\t\t\t: ";				cin >> surname;			 lastUser->surname = surname;
	cout << " Введите имя\t\t\t\t: ";				cin >> name;			 lastUser->name = name;
	cout << " Введите отчество\t\t\t: ";			cin >> patronymic;		 lastUser->patronymic = patronymic;
	cout << " Введите город\t\t\t\t: ";				cin >> city;			 lastUser->city = city;
	cout << " Введите улицу\t\t\t\t: ";				cin >> street;			 lastUser->street = street;
	cout << " Введите номер дома\t\t\t: ";			cin >> lastUser->houseNumb;
	cout << " Введите номер квартиры\t\t\t: ";		cin >> lastUser->apartmentNumb;
	cout << " Введите номер счета\t\t\t: ";			cin >> lastUser->accountNumber;

	Customer::size++;
}

//Вывод данных о покупателях в файл
void SaveToFile() {
	ofstream outf("NewData.txt");

	// Если мы не можем открыть этот файл для записи данных,
	if (!outf) {
		cout << "Ошибка открытия файла для записи!" << endl;
		return;
	}

	// Записываем в файл
	int i = 1;
	currentUser = firstUser;
	while (currentUser != nullptr) {
		outf << setfill(' ') << "|" << setw(4) << i++ << "|"
			<< setw(16) << currentUser->surname << "|"
			<< setw(16) << currentUser->name << "|"
			<< setw(16) << currentUser->patronymic << "|"
			<< setw(16) << currentUser->city << "|"
			<< setw(16) << currentUser->street << "|"
			<< setw(16) << currentUser->houseNumb << "|"
			<< setw(16) << currentUser->apartmentNumb << "|"
			<< setw(16) << currentUser->accountNumber << "|\n";
		currentUser = currentUser->pNext;
	}
}

//Удаления данных пользователя из конца списка
void PopBack() {
	if (firstUser == lastUser) {				//Если в списке только 1 покупатель
		delete firstUser;
		lastUser = nullptr;
		firstUser = nullptr;
		Customer::size--;
	}
	else {
		currentUser = lastUser->pPred;
		delete lastUser;
		lastUser = currentUser;
		lastUser->pNext = nullptr;
		Customer::size--;
	}
}

//Удаления данных пользователя из начала списка
void PopFront() {
	if (firstUser == lastUser) {				//Если в списке только 1 покупатель
		delete firstUser;
		lastUser = nullptr;
		firstUser = nullptr;
		Customer::size--;
	}
	else {
		currentUser = firstUser->pNext;
		delete firstUser;
		firstUser = currentUser;
		firstUser->pPred = nullptr;
		Customer::size--;
	}
}

//Удалить данные пользователя под введенным индексом
void Remove() {
	//Проверка на пустоту базы данных
	if (Customer::size <= 0) {
		cout << "\nБаза данных пуста!" << endl;
		return;
	}

	int index;

	cout << "Введите номер пользователя, данные которого хотите удалить: ";
	cin >> index;
	//Если номер пользователя задан некорректно, повторный вызов этой функции
	if ((index < 1) || (index > Customer::size)) {
		cout << "\nНомер пользователя задан некорректно! Попробуйте ещё раз." << endl;
		Remove();
		return;
	}

	cout << "\n\nПроизводится удаление данных покупетеля...\n";

	//Если введённый пользователем индекс является первым элементом 
	if (index == 1) PopFront();

	//Если введённый пользователем индекс является последним элементом 
	else if (index == Customer::size) PopBack();

	else {
		currentUser = firstUser;
		int i = 1;
		while (i++ != index) currentUser = currentUser->pNext;
		//Связывание соседних элементов друг с другом
		currentUser->pPred->pNext = currentUser->pNext;
		currentUser->pNext->pPred = currentUser->pPred;

		currentUser->pNext = nullptr;
		currentUser->pPred = nullptr;
		delete currentUser;

		Customer::size--;
	}
}

//Выбор поля
void SelectionOptions(int& buffer) {
	cout << "\nВыберете поле:\n";
	cout << "1: Поле ФАМИЛИЯ.\n"
		<< "2: Поле ИМЯ.\n"
		<< "3: Поле ОТЧЕСТВО.\n"
		<< "4: Поле ГОРОД.\n"
		<< "5: Поле УЛИЦА.\n"
		<< "6: Поле НОМЕР ДОМА.\n"
		<< "7: Поле НОМЕР КВАРТИРЫ.\n"
		<< "8: Поле НОМЕР СЧЕТА.\n";
	cin >> buffer;

	//Введенное число некорректно
	if ((buffer < 1) || (buffer > 8)) {
		cout << "Введенное число некорректно, попробуйте ещё раз.\n" << endl;
		return;
	}
}

//Функция для возврашения значения поля (string) элемента структуры
template<> string ReturnField(Customer* currentUser, int buffer) {
	switch (buffer) {
	case 1:
		return currentUser->surname;
	case 2:
		return currentUser->name;
	case 3:
		return currentUser->patronymic;
	case 4:
		return currentUser->city;
	case 5:
		return currentUser->street;
	default:
		cout << "\nВведенное значение не распознано." << endl;
	}
}

//Функция для возврашения значения поля (int) элемента структуры
template<> int ReturnField(Customer* currentUser, int buffer) {
	switch (buffer) {
	case 6:
		return currentUser->houseNumb;
	case 7:
		return currentUser->apartmentNumb;
	case 8:
		return currentUser->accountNumber;
	default:
		cout << "\nВведенное значение не распознано." << endl;
	}
}

//Выборка значений по заданному условию
void Search(const int buffer) {
	//Проверка на пустоту базы данных
	if (Customer::size <= 0) {
		cout << "\nБаза данных пуста!" << endl;
		return;
	}
	int counter = 0;
	if (buffer < 6) {
		string bufferWord;
		cout << "Введите слово, по которому будет производтьиться поиск: ";
		cin >> bufferWord;
		cout << "Идет поиск покупателей...\n";
		currentUser = firstUser;
		//Цикл поиска элементов, значения полей которых совпадают с искомыми
		while (currentUser != nullptr) {
			if (ReturnField<string>(currentUser, buffer) == bufferWord) {
				counter++;
				PrintData(counter);
			}
			currentUser = currentUser->pNext;
		}
	}
	else {
		int bufferNumb;
		cout << "Введите число, по которому будет производтьиться поиск: ";
		cin >> bufferNumb;
		cout << "Идет поиск покупателей...\n";
		currentUser = firstUser;
		//Цикл поиска элементов, значения полей которых совпадают с искомыми
		while (currentUser != nullptr) {
			if (ReturnField<int>(currentUser, buffer) == bufferNumb) {
				counter++;
				PrintData(counter);
			}
			currentUser = currentUser->pNext;
		}
	}
	if (counter == 0) cout << "Таких покупателей не выявленно." << endl;
}

//Пузырьковая сортировка
void Sort(const int buffer) {
	if (Customer::size <= 1) {
		cout << "\nСортировка невозможна!" << endl;
		return;
	}

	for (int i = 0; i < Customer::size; i++) {
		for (currentUser = firstUser; currentUser != lastUser; currentUser = currentUser->pNext) {
			if (currentUser->pNext == nullptr) break;
			if (Comparison(buffer)) {
				Swap(currentUser, currentUser->pNext);
				currentUser = currentUser->pPred;
			}
		}
	}
}

//Сравнение значей одного поля у двух пользователей 
bool Comparison(const int buffer) {
	if (buffer < 6) {
		return ReturnField<string>(currentUser, buffer) > ReturnField<string>(currentUser->pNext, buffer);
	}
	else {
		return ReturnField<int>(currentUser, buffer) > ReturnField<int>(currentUser->pNext, buffer);
	}
}

//Поменять местами данные о 2-х покупателях
void Swap(Customer* temp1, Customer* temp2) {

	//Если первый элемент является головой списка
	if (temp1->pPred == nullptr) {
		firstUser = temp2;
		temp2->pPred = nullptr;
	}
	else {
		temp1->pPred->pNext = temp2;
		temp2->pPred = temp1->pPred;
	}

	//Если второй элемент является последним элементом списка
	if (temp2->pNext == nullptr) {
		lastUser = temp1;
		temp1->pNext = nullptr;
	}
	else {
		temp2->pNext->pPred = temp1;
		temp1->pNext = temp2->pNext;
	}

	temp1->pPred = temp2;
	temp2->pNext = temp1;
}

//Удаление всего списка
void Delete() {
	if (Customer::size <= 0) {
		cout << "\nБаза данных уже пуста!" << endl;
		return;
	}
	while (Customer::size != 0) PopFront();
}