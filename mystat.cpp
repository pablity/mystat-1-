// username - student
// password - password
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <locale>

using namespace std;

struct User {
    string username;
    string password;
    bool isAuthenticated = false;
};

struct Assignment {
    int id;
    string title;
    string description;
    int grade = -1;
};

class HomeworkManager {
private:
    vector<Assignment> assignments;
    int nextId = 1;
    const string filename = "assignments.txt";

public:
    HomeworkManager() {
        loadAssignments();
    }

    ~HomeworkManager() {
        saveAssignments();
    }
    //добавление задания 
    void addAssignment(const string& title, const string& description) {
        assignments.push_back({ nextId++, title, description });
        cout << "Домашнее задание добавлено.\n";
        saveAssignments();
    }

    void viewAssignments() {
        if (assignments.empty()) {
            cout << "Нет домашних заданий.\n";
            return;
        }
        for (const auto& assignment : assignments) {
            cout << "ID: " << assignment.id << " | "
                << "Название: " << assignment.title << " | "
                << "Описание: " << assignment.description << " | "
                << "Оценка: " << (assignment.grade == -1 ? "Не выставлена" : to_string(assignment.grade))
                << '\n';
        }
    }
    // удаление задания 
    void deleteAssignment(int id) {
        for (auto it = assignments.begin(); it != assignments.end(); ++it) {
            if (it->id == id) {
                assignments.erase(it);
                cout << "Задание удалено.\n";
                saveAssignments();
                return;
            }
        }
        cout << "Задание не найдено.\n";
    }

    //сохранение 
    void saveAssignments() {
        ofstream file(filename, ios::out | ios::trunc);
        if (file.is_open()) {
            for (const auto& assignment : assignments) {
                file << assignment.id << '\n'
                    << assignment.title << '\n'
                    << assignment.description << '\n'
                    << assignment.grade << '\n';
            }
            file.close();
        }
        else {
            cout << "Ошибка при сохранении данных в файл.\n";
        }
    }
    // загружает данные из файла
    void loadAssignments() {
        ifstream file(filename);
        if (file.is_open()) {
            Assignment assignment;
            while (file >> assignment.id) {
                file.ignore();
                getline(file, assignment.title);
                getline(file, assignment.description);
                file >> assignment.grade;
                file.ignore();
                assignments.push_back(assignment);
                nextId = assignment.id + 1;
            }
            file.close();
        }
        else {
            cout << "Файл с заданиями не найден. Создан новый файл.\n";
        }
    }
};
// Вход в прогу 
bool authenticate(User& user, const string& username, const string& password) {
    if (user.username == username && user.password == password) {
        user.isAuthenticated = true;
        cout << "Вход выполнен успешно.\n";
        return true;
    }
    cout << "Неверные данные.\n";
    return false;
}
// выбор действий 
void showMenu() {
    cout << "\nМеню:\n";
    cout << "1. Посмотреть домашние задания\n";
    cout << "2. Добавить домашнее задание\n";
    cout << "3. Удалить домашнее задание\n";
    cout << "4. Выход\n";
    cout << "Выберите опцию: ";
}

int main() {
    setlocale(LC_ALL, "rus");
    User user = { "student", "password" };
    HomeworkManager manager;
    string username, password;

    cout << "Введите логин: ";
    cin >> username;
    cout << "Введите пароль: ";
    cin >> password;

    if (!authenticate(user, username, password)) {
        return 0;
    }
    // подбор  выбора 
    int choice;
    while (user.isAuthenticated) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            manager.viewAssignments();
            break;
        }
        case 2: {
            string title, description;
            cout << "Введите название задания: ";
            cin.ignore();
            getline(cin, title);
            cout << "Введите описание задания: ";
            getline(cin, description);
            manager.addAssignment(title, description);
            break;
        }
        case 3: {
            int id;
            cout << "Введите ID задания для удаления: ";
            cin >> id;
            manager.deleteAssignment(id);
            break;
        }
        case 4: {
            cout << "Выход из системы.\n";
            user.isAuthenticated = false;
            break;
        }
        default:
            cout << "Некорректный выбор.\n";
        }
    }

    return 0;
}

