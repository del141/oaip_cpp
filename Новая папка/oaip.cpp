#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

struct Kniga
{
    string nazvanie;
    string avtor;
    int god;
    int bilet; 
};

struct Chitatel
{
    string fio;
    int bilet;
};

vector<Kniga> knigi;
vector<Chitatel> chitateli;
int sleduyushiyBilet = 4;

int vvodChisla(string tekst, int minimum, int maximum)
{
    string stroka;
    int chislo;
    char simvol;

    while (true)
    {
        cout << tekst;

        if (!getline(cin, stroka))
            return 0;

        stringstream potok(stroka);

        if (potok >> chislo)
        {
            if (!(potok >> simvol) &&
                chislo >= minimum && chislo <= maximum)
            {
                return chislo;
            }
        }

        cout << "Введите целое число от " << minimum
             << " до " << maximum << ".\n";
    }
}

string vvodTeksta(string tekst)
{
    string stroka;

    while (true)
    {
        cout << tekst;

        if (!getline(cin, stroka))
            return "";

        if (stroka.find_first_not_of(" \t\r\v\f") != string::npos)
            return stroka;

        cout << "Строка не должна быть пустой.\n";
    }
}

int kolichestvoKnig(int bilet)
{
    int kolichestvo = 0;

    for (int i = 0; i < (int)knigi.size(); i++)
    {
        if (knigi[i].bilet == bilet)
            kolichestvo++;
    }

    return kolichestvo;
}

void spisokKnig()
{
    cout << "\n--- Книги библиотеки ---\n";

    if (knigi.empty())
    {
        cout << "В библиотеке пока нет книг.\n";
        return;
    }

    for (int i = 0; i < (int)knigi.size(); i++)
    {
        cout << i + 1 << ". " << knigi[i].nazvanie
             << " | " << knigi[i].avtor
             << " | " << knigi[i].god << " | ";

        if (knigi[i].bilet == 0)
            cout << "Свободна\n";
        else
            cout << "Выдана, билет: " << knigi[i].bilet << '\n';
    }
}

void spisokChitateley()
{
    cout << "\n--- Читатели библиотеки ---\n";

    if (chitateli.empty())
    {
        cout << "Читателей пока нет.\n";
        return;
    }

    for (int i = 0; i < (int)chitateli.size(); i++)
    {
        cout << i + 1 << ". " << chitateli[i].fio
             << " | Билет: " << chitateli[i].bilet
             << " | Книг: " << kolichestvoKnig(chitateli[i].bilet)
             << '\n';
    }
}

int vybratKnigu()
{
    spisokKnig();

    if (knigi.empty())
        return -1;

    int nomer = vvodChisla("Номер книги (0 - отмена): ",
                          0, (int)knigi.size());
    return nomer - 1;
}

int vybratChitatelya()
{
    spisokChitateley();

    if (chitateli.empty())
        return -1;

    int nomer = vvodChisla("Номер читателя в списке (0 - отмена): ",
                          0, (int)chitateli.size());
    return nomer - 1;
}

void dobavitKnigu()
{
    Kniga novaya;

    novaya.nazvanie = vvodTeksta("Название: ");
    if (!cin) return;

    novaya.avtor = vvodTeksta("Автор: ");
    if (!cin) return;

    novaya.god = vvodChisla("Год издания (1-2026): ", 1, 2026);
    if (!cin) return;

    novaya.bilet = 0;
    knigi.push_back(novaya);

    cout << "Книга добавлена в библиотеку.\n";
}

void izmenitKnigu()
{
    int indeks = vybratKnigu();
    if (indeks == -1) return;

    string nazvanie = vvodTeksta("Новое название: ");
    if (!cin) return;

    string avtor = vvodTeksta("Новый автор: ");
    if (!cin) return;

    int god = vvodChisla("Новый год издания (1-2026): ", 1, 2026);
    if (!cin) return;

    knigi[indeks].nazvanie = nazvanie;
    knigi[indeks].avtor = avtor;
    knigi[indeks].god = god;

    cout << "Информация о книге изменена.\n";
}

void udalitKnigu()
{
    int indeks = vybratKnigu();
    if (indeks == -1) return;

    if (knigi[indeks].bilet != 0)
    {
        cout << "Нельзя удалить выданную книгу. Сначала верните её.\n";
        return;
    }

    knigi.erase(knigi.begin() + indeks);
    cout << "Книга удалена из библиотеки.\n";
}

void dobavitChitatelya()
{
    Chitatel noviy;

    noviy.fio = vvodTeksta("ФИО читателя: ");
    if (!cin) return;

    noviy.bilet = sleduyushiyBilet;
    chitateli.push_back(noviy);
    sleduyushiyBilet++;

    cout << "Читатель зарегистрирован. Билет: " << noviy.bilet << '\n';
}

void izmenitChitatelya()
{
    int indeks = vybratChitatelya();
    if (indeks == -1) return;

    string fio = vvodTeksta("Новое ФИО: ");
    if (!cin) return;

    chitateli[indeks].fio = fio;
    cout << "ФИО изменено. Номер билета сохранён.\n";
}

void udalitChitatelya()
{
    int indeks = vybratChitatelya();
    if (indeks == -1) return;

    int bilet = chitateli[indeks].bilet;

    if (kolichestvoKnig(bilet) > 0)
    {
        cout << "У читателя остались книги. Сначала нужно их вернуть.\n";
        return;
    }

    chitateli.erase(chitateli.begin() + indeks);
    cout << "Читатель удалён.\n";
}

void vydatKnigu()
{
    int chitatel = vybratChitatelya();
    if (chitatel == -1) return;

    int kniga = vybratKnigu();
    if (kniga == -1) return;

    if (knigi[kniga].bilet != 0)
    {
        cout << "Эта книга уже выдана другому читателю.\n";
        return;
    }

    knigi[kniga].bilet = chitateli[chitatel].bilet;
    cout << "Книга выдана читателю: " << chitateli[chitatel].fio << '\n';
}

void knigiChitatelya()
{
    int indeks = vybratChitatelya();
    if (indeks == -1) return;

    int bilet = chitateli[indeks].bilet;
    cout << "\nКниги читателя: " << chitateli[indeks].fio << '\n';

    if (kolichestvoKnig(bilet) == 0)
    {
        cout << "У этого читателя нет книг.\n";
        return;
    }

    int nomer = 1;

    for (int i = 0; i < (int)knigi.size(); i++)
    {
        if (knigi[i].bilet == bilet)
        {
            cout << nomer << ". " << knigi[i].nazvanie
                 << " | " << knigi[i].avtor << '\n';
            nomer++;
        }
    }
}

void vernutKnigu()
{
    int indeks = vybratChitatelya();
    if (indeks == -1) return;

    int bilet = chitateli[indeks].bilet;

    if (kolichestvoKnig(bilet) == 0)
    {
        cout << "У читателя нет книг для возврата.\n";
        return;
    }

    vector<int> nomera;
    cout << "\nВыберите книгу для возврата:\n";

    for (int i = 0; i < (int)knigi.size(); i++)
    {
        if (knigi[i].bilet == bilet)
        {
            nomera.push_back(i);
            cout << nomera.size() << ". " << knigi[i].nazvanie << '\n';
        }
    }

    int nomer = vvodChisla("Номер книги (0 - отмена): ",
                          0, (int)nomera.size());
    if (nomer == 0) return;

    int kniga = nomera[nomer - 1];
    knigi[kniga].bilet = 0;
    cout << "Книга возвращена в библиотеку.\n";
}

void statistika()
{
    int vydano = 0;
    int chitateleySKnigami = 0;

    for (int i = 0; i < (int)knigi.size(); i++)
    {
        if (knigi[i].bilet != 0)
            vydano++;
    }

    for (int i = 0; i < (int)chitateli.size(); i++)
    {
        if (kolichestvoKnig(chitateli[i].bilet) > 0)
            chitateleySKnigami++;
    }

    cout << "\n--- Статистика ---\n";
    cout << "Всего книг: " << knigi.size() << '\n';
    cout << "Выдано книг: " << vydano << '\n';
    cout << "Свободно книг: " << (int)knigi.size() - vydano << '\n';
    cout << "Всего читателей: " << chitateli.size() << '\n';
    cout << "Читателей с книгами: " << chitateleySKnigami << '\n';
}

void nachalnieDannie()
{
    knigi.push_back({"Мастер и Маргарита", "М. Булгаков", 1967, 1});
    knigi.push_back({"Два капитана", "В. Каверин", 1940, 2});
    knigi.push_back({"Белый Клык", "Джек Лондон", 1906, 0});
    knigi.push_back({"Пикник на обочине", "А. и Б. Стругацкие", 1972, 0});

    chitateli.push_back({"Иванов Артём Сергеевич", 1});
    chitateli.push_back({"Соколова Анна Олеговна", 2});
    chitateli.push_back({"Орлов Максим Андреевич", 3});
}

void pokazMenu()
{
    cout << "\n========== БИБЛИОТЕКА ==========\n";
    cout << "1. Список книг\n";
    cout << "2. Добавить книгу\n";
    cout << "3. Изменить книгу\n";
    cout << "4. Удалить книгу\n";
    cout << "5. Список читателей\n";
    cout << "6. Добавить читателя\n";
    cout << "7. Изменить читателя\n";
    cout << "8. Удалить читателя\n";
    cout << "9. Выдать книгу\n";
    cout << "10. Книги читателя\n";
    cout << "11. Вернуть книгу\n";
    cout << "12. Статистика\n";
    cout << "0. Выход\n";
}

int main()
{
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    nachalnieDannie();

    while (true)
    {
        pokazMenu();
        int punkt = vvodChisla("Выберите пункт: ", 0, 12);

        if (!cin || punkt == 0)
            break;

        switch (punkt)
        {
        case 1:
            spisokKnig();
            break;
        case 2:
            dobavitKnigu();
            break;
        case 3:
            izmenitKnigu();
            break;
        case 4:
            udalitKnigu();
            break;
        case 5:
            spisokChitateley();
            break;
        case 6:
            dobavitChitatelya();
            break;
        case 7:
            izmenitChitatelya();
            break;
        case 8:
            udalitChitatelya();
            break;
        case 9:
            vydatKnigu();
            break;
        case 10:
            knigiChitatelya();
            break;
        case 11:
            vernutKnigu();
            break;
        case 12:
            statistika();
            break;
        }
    }

    cout << "Работа программы завершена.\n";
    return 0;
}
