#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

const int MAX_ITEMS = 20;
const double TAX_RATE = 0.21;

struct menu_item_type {
    std::string menu_item;
    double menu_price;
};

void get_data(menu_item_type menu_list[], int& menu_count);
void show_menu(const menu_item_type menu_list[], int menu_count);
void print_check(const menu_item_type menu_list[], const int order_quantity[], int menu_count);

int main() {
    menu_item_type menu_list[MAX_ITEMS];
    int order_quantity[MAX_ITEMS] = { 0 };
    int menu_count = 0;

    get_data(menu_list, menu_count);
    show_menu(menu_list, menu_count);

    int choice;
    while (true) {
        std::cout << "Ivesk norimo maisto numeri (0 - exit): ";
        std::cin >> choice;

        if (choice == 0)
            break;

        if (choice < 1 || choice > menu_count) {
            std::cout << "Pasirinkimas neegzistuoja.";
            continue;
        }

        std::cout << "Ivesk porciju kieki: ";
        std::cin >> order_quantity[choice - 1];
    }

    print_check(menu_list, order_quantity, menu_count);

    return 0;
}

void get_data(menu_item_type menu_list[], int& menu_count) {
    std::ifstream file("menu.txt");

    if (!file) {
        std::cout << "menu.txt failas neegzistuoja arba nepavyko atidaryti.\n";
        exit(1);
    }

    while (!file.eof() && menu_count < MAX_ITEMS) {
        std::getline(file, menu_list[menu_count].menu_item, '|');
        file >> menu_list[menu_count].menu_price;
        file.ignore(1000, '\n');
        menu_count++;
    }

    file.close();
}

void show_menu(const menu_item_type menu_list[], int menu_count) {
    std::cout << "\nRestorano mini apskaitos sistema.\n\n";

    std::cout << std::fixed << std::setprecision(2);

    for (int i = 0; i < menu_count; i++) {
        std::cout << std::setw(2) << i + 1 << ". "
            << std::left << std::setw(40) << menu_list[i].menu_item
            << std::right << menu_list[i].menu_price << " Eur.\n";
    }

    std::cout << "Irasykite porciju kieki ir maisto numeri.\n";
}

void print_check(const menu_item_type menu_list[], const int order_quantity[], int menu_count) {
    std::ofstream receipt("saskaita.txt");

    double total_price_before_tax = 0.0;

    std::cout << "\n--- SASKAITA ---\n\n";
    receipt << "Saskaita:\n\n";

    std::cout << std::fixed << std::setprecision(2);
    receipt << std::fixed << std::setprecision(2);

    for (int i = 0; i < menu_count; i++) {
        if (order_quantity[i] > 0) {
            double item_total = order_quantity[i] * menu_list[i].menu_price;
            total_price_before_tax += item_total;

            std::cout << order_quantity[i] << "  "
                << std::left << std::setw(40) << menu_list[i].menu_item
                << std::right << item_total << " Eur\n";

            receipt << order_quantity[i] << "  "
                << std::left << std::setw(40) << menu_list[i].menu_item
                << std::right << item_total << " Eur\n";
        }
    }

    double tax = total_price_before_tax * TAX_RATE;
    double total = total_price_before_tax + tax;

    std::cout << "\nMokesciai 21%" << std::setw(28) << tax << " Eur\n";
    std::cout << "Galutine suma" << std::setw(30) << total << " Eur\n";

    receipt << "\nMokesciai 21%" << std::setw(28) << tax << " Eur\n";
    receipt << "Galutine kaina: " << std::setw(30) << total << " Eur\n";

    receipt.close();
}
