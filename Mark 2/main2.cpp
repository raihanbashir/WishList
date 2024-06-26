#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node {
    string link;
    double price;
    Node* next;

    Node(string l, double p) : link(l), price(p), next(nullptr) {}
};

class Wishlist {
private:
    Node* head;

public:
    Wishlist() : head(nullptr) {
        loadFromFile("wishlist.txt");  // Load wishlist from file on startup
    }

    void addItem(const string& link, double price) {
        Node* newNode = new Node(link, price);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void removeItem(const string& link) {
        if (!head) {
            cout << "Wishlist is empty. Cannot remove item." << endl;
            return;
        }

        if (head->link == link) {
            Node* temp = head;
            head = head->next;
            delete temp;
            cout << "Item removed: " << link << endl;
            return;
        }

        Node* temp = head;
        while (temp->next && temp->next->link != link) {
            temp = temp->next;
        }

        if (temp->next) {
            Node* nodeToDelete = temp->next;
            temp->next = temp->next->next;
            delete nodeToDelete;
            cout << "Item removed: " << link << endl;
        } else {
            cout << "Item not found: " << link << endl;
        }
    }

    void displayItems() const {
        Node* temp = head;
        while (temp) {
            cout << "Link: " << temp->link << " | Price: $" << temp->price << endl;
            temp = temp->next;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            Node* temp = head;
            while (temp) {
                file << temp->link << "," << temp->price << endl;
                temp = temp->next;
            }
            file.close();
        } else {
            cerr << "Unable to open file for writing." << endl;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t commaPos = line.find(',');
                if (commaPos != string::npos) {
                    string link = line.substr(0, commaPos);
                    double price = stod(line.substr(commaPos + 1));
                    addItem(link, price);
                }
            }
            file.close();
        } else {
            cerr << "Unable to open file for reading." << endl;
        }
    }

    ~Wishlist() {
        saveToFile("wishlist.txt");  // Save wishlist to file on exit

        Node* temp = head;
        while (temp) {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
    }
};

int main() {
    Wishlist wishlist;

    string link;
    double price;
    char choice;

    while (true) {
        cout << "\nMenu Options:\n";
        cout << "1. Add item\n";
        cout << "2. Remove item\n";
        cout << "3. Display wishlist\n";
        cout << "4. Exit\n";
        cout << "Choose an option (1-4): ";
        cin >> choice;
        cin.ignore(); // To ignore the newline character left in the buffer

        switch (choice) {
            case '1':
                cout << "Enter item link: ";
                getline(cin, link);
                cout << "Enter item price: ";
                cin >> price;
                cin.ignore(); // To ignore the newline character left in the buffer
                wishlist.addItem(link, price);
                break;

            case '2':
                cout << "Enter the link of the item to remove: ";
                getline(cin, link);
                wishlist.removeItem(link);
                break;

            case '3':
                cout << "\nWishlist Items:\n";
                wishlist.displayItems();
                break;

            case '4':
                cout << "Exiting the program. Goodbye!" << endl;
                return 0;

            default:
                cout << "Invalid option. Please choose a valid option (1-4)." << endl;
                break;
        }
    }

    return 0;
}
