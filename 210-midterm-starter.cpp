#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;

class DoublyLinkedList {
private:
    struct Node {
        int data;
        Node* prev;
        Node* next;
        Node(int val, Node* p = nullptr, Node* n = nullptr) {
            data = val; 
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    void insert_after(int value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }

    void delete_val(int value) {
        if (!head) return;

        Node* temp = head;
        
        while (temp && temp->data != value)
            temp = temp->next;

        if (!temp) return; 

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next; 

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev; 

        delete temp;
    }

    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
    
        if (pos == 1) {
            pop_front();
            return;
        }
    
        Node* temp = head;
    
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }
    
        if (!temp->next) {
            pop_back();
            return;
        }
    
        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    void push_back(int v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    
    void push_front(int v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    
    void pop_front() {

        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node * temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node * temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    void print() {
        Node* current = head;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) { 
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->prev;
        }
        cout << endl;
    }
};

/* NOTES: Simulation runs for 20 time periods (20 iterations) 
          First time period, add 5 customers to the line, use push back in for loop 5 times to add 5 nodes to dll
    WHAT TO USE/DO: - SET: Use doubly linked list to keep track on whos in line based off of position 
                    - DONE: Before for loop, initialize string array to hold names of each person. (initialize for 101 strings, 100 possible names)
                    - Use for loop from 0-19 for each time period 
                    - Append push back 5 numbers each 
                    - At beginning of each loop iteration, generate random probability number to decide what happens*/
    
int main() {
    //cout << MIN_NR + MIN_LS + MAX_NR + MAX_LS;  // dummy statement to avoid compiler warning
    ifstream fin;
    fin.open("names.txt");
    DoublyLinkedList list;
    int prob; // random number generator between 1 and 100
    string names_array[101];
    int names_iter = 0; // iterator for names array and dll list

    cout << "Store opens:" << endl;
    for (int i = 0; i < 5; i++){ // WORKS: fills the first 5 indexes of the names_array with names from the names.txt file
        getline(fin, names_array[i]); // appends a new name to names_array 
        list.push_back(i); // adds a new node to represent each customer
        cout << setw(4) << "" << names_array[i] << " joins the line" << endl;
    }
    cout << setw(4) << "Resulting line:" << endl;
    for (int i = 0; i < 5; i++){
        cout << setw(8) << names_array[i] << endl;
    }

    names_iter = 4; // dll list has nodes 0-4 (5 total)
    // at this point: names array has 5 names, list has 5 nodes
    for (int i = 1; i < 20; i++){ // main for loop to begin the driver program
        prob = rand() % 100 + 1; // generates a random number between 1-100
        cout << "Time steop #" << i + 1 << endl;
        if (prob <= 40){
            cout << names_array[0];
        }

        if (prob <= 60){
            // append customer to end of line
            names_iter++;
            list.push_back(names_iter);
            getline(fin, names_array[names_iter]);
            cout << setw(4) << "" << names_array[names_iter] << " joins the line" << endl;
        }

        if (prob <= 20){ 
            // end customer leaves the line
            list.pop_back()
        }

        if (prob <= 10){
            // random customer leaves line
            int random_number = rand() % names_iter;
            list.delete_pos(random_number);
        }

        if (prob >= 90){
            // vip skips line and gets helped
            names_iter++;
            getline(fin, names_array[names_iter]);
            cout << setw(4) << "" << names_array[names_iter] << " (VIP) joins the front of the line";
            list.delete_pos();
        }
    }

    fin.close();
    list.~DoublyLinkedList(); // deallocates linked list
    return 0;
}