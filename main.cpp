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

    while (true) {
        cout << "Enter item link (or type 'exit' to stop): ";
        getline(cin, link);
        if (link == "exit") break;

        cout << "Enter item price: ";
        cin >> price;
        cin.ignore(); // To ignore the newline character left in the buffer

        wishlist.addItem(link, price);
    }

    cout << "\nWishlist Items:\n";
    wishlist.displayItems();

    return 0;
}
