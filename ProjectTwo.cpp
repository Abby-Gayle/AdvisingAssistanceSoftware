//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Abigail Gutierrez
// Description : Advising Assistance Software
//============================================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <climits>
using namespace std;

const unsigned int DEFAULT_SIZE = 179;

//structure of courses
struct CourseData {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

class HashTable {

private:
    // Define structures to hold courses
    struct Node {
        CourseData coursedata;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a course
        Node(CourseData aCourse) : Node() {
            coursedata = aCourse;
        }

        // initialize with a course and a key
        Node(CourseData aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(string key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void hashInsert(CourseData coursedata);
    void printAllCourses();
    CourseData searchCourse(string courseNumber);
    size_t Size();
};

//functions definitions
void displayMenu();
void menu(HashTable& courses);
void storeData(HashTable& courses);
void quicksort(vector<CourseData>& courses, int lowindex, int highIndex);
int partition(vector<CourseData>& courses, int lowIndex, int highIndex);

/*
* Size of the table unknown will create a default
*/
HashTable::HashTable() {   
    nodes.resize(tableSize);
}

/*
* If the size of table is known this will create it
*/
HashTable::HashTable(unsigned int size) { 
    this->tableSize = size;
    nodes.resize(size);
}

/*
* Deletes all nodes chained in the hash table 
*/
HashTable::~HashTable() {  
    // goes through all the nodes
    for (int i = 0; i < nodes.size(); i++) { 
        //next node would be the chain grabs that one
        Node* curNode = nodes[i].next;  
        //deletes the chained node
        while (curNode != nullptr) {   
            Node* temp = curNode;  
            curNode = curNode->next;    
            delete temp;    
        }
    }
}

/*
* Calculates the hash value using the course number
*/
unsigned int HashTable::hash(string courseNumber) {
    //holds the total
    int total = 0;

    //go through each character
    for (int i = 0; i < courseNumber.length(); i++) {

        //add each character "ASCII" together into total
        total += courseNumber[i];
    }

    //creates the hash
    return total % tableSize;
}

/*
* Creates a key "bucket area" if nothing in there inserts data there
* Goes to the next node in the bucket to create a chain
* If the bucket already has data and a chain goes to the end of the chain and inserts the data there
*/
void HashTable::hashInsert(CourseData coursedata) {
    //Creating a bucket for the table
    unsigned int key = hash(coursedata.courseNumber);
    Node* node = &nodes[key];

    //empty bucket add data there
    if (node->key == UINT_MAX) {
        node->key = key;
        node->coursedata = coursedata;
        node->next = nullptr;
    }
    //not an empty bucket add data at the next node
    else if (node->next == nullptr) {
        node->next = new Node(coursedata, key);
    }
    //not an empty bucket and a clash add data at next-next node
    else {
        Node* currNode = node;

        while (currNode->next != nullptr) {
            currNode = currNode->next;
        }
        currNode->next = new Node(coursedata, key);
    }
}

/*
* Searchs for a course by using a hash with course number and seeing
* where the key matches in the bucket and checks the chain
*/
CourseData HashTable::searchCourse(string courseNumber) {

    //creates an empty course data
    CourseData coursedata;

    //creating a bucket for the table
    unsigned int key = hash(courseNumber);
    Node* node = &nodes[key];

    //empty bucket return nothing
    if (node->key == UINT_MAX) {
        return coursedata;
    }

    //if bucket not empty will go through to see about the chains
    while (node != nullptr) {
        if (node->coursedata.courseNumber == courseNumber) {
            return node->coursedata;
        }
        node = node->next;
    }

    //found nothing returns nothing
    return coursedata;
}

/*
* This will print all the courses and sort them
* by using a quicksort
*/
void HashTable::printAllCourses() {
    //Holds the courses
    vector<CourseData> tempCourses;
    //goes through each node
    for (int i = 0; i < nodes.size(); i++) {
        Node* currNode = &nodes[i];
        //empty continue
        if (currNode->key == UINT_MAX) {
            continue;
        }
        //if not empty put data into the temp from the back and go to the next one
        while (currNode != nullptr) {
            tempCourses.push_back(currNode->coursedata);
            currNode = currNode->next;
        }
    }

    //sorts the data in the tempcourses
    quicksort(tempCourses, 0, tempCourses.size() - 1);

    //prints all the data from the temp courses
    for (int i = 0; i < tempCourses.size(); i++) {
        cout << tempCourses[i].courseNumber << ", "
            << tempCourses[i].courseName << endl;
    }
}

/*
* Recursivley sorts data dividing into partitions around pivots
*/
void quicksort(vector<CourseData>& courses, int lowIndex, int highIndex) {
    if (lowIndex >= highIndex) {
        return;
    }

    int pivot = partition(courses, lowIndex, highIndex);

    quicksort(courses, lowIndex, pivot);
    quicksort(courses, pivot + 1, highIndex);
}

/*
* Create a pivot to sort the data
* Left for the smaller data
* right for the bigger data
*/
int partition(vector<CourseData>& courses, int lowIndex, int highIndex) {
    int midpoint = lowIndex + (highIndex - lowIndex) / 2;
    string pivot = courses[midpoint].courseNumber;
    bool done = false;

    while (!done) {
        while (courses[lowIndex].courseNumber < pivot) {
            lowIndex++;
        }

        while (pivot < courses[highIndex].courseNumber) {
            highIndex--;
        }

        if (lowIndex >= highIndex) {
            done = true;
        }
        else {
            CourseData temp = courses[lowIndex];
            courses[lowIndex] = courses[highIndex];
            courses[highIndex] = temp;
            lowIndex++;
            highIndex--;
        }
    }

    return highIndex;
}

int main()
{
    //Intialize the hashtable for the courses
    HashTable courses;
    //begins the menu
    menu(courses);
}

/*
* Shows the user what they can press in the menu
*/
void displayMenu() {
    cout << "*********************************" << endl;
    cout << "*" << setw(10) << "\t  Course Menu" << setw(12) << "*" << endl;
    cout << "*********************************" << endl;
    cout << "*" << setw(10) << "\t1. Load File" << setw(13) << "*" << endl;
    cout << "*" << setw(10) << "\t2. Print All Courses"  << setw(5) << "*" << endl;
    cout << "*" << setw(10) << "\t3. Search for Course" << setw(5) << "*" << endl;
    cout << "*" << setw(10) << "\t9. Quit" << setw(18) << "*" << endl;
    cout << "*********************************" << endl;
}

/*
* User must pick and option
* Option 1: Load the file data into the data structure. Note that before you can print the 
* course information or the sorted list of courses, you must load the data into the data structure.
* Option 2: Print an alphanumeric list of all the courses in the Computer Science department.
* Option 3: Print the course title and the prerequisites for any individual course.
* Option 9: Closes program.
*/
void menu(HashTable& courses) {
    //will hold users input
    int userInput;

    //do-while to continue the program till option 9 is picked
    do {

        //menu for user to see options
        displayMenu();

        //user prompted to input choice
        cout << "Enter choice: ";
        cin >> userInput;
        switch (userInput) {
        case 1:
            //Loads the data file into the data structure
            storeData(courses);
            break;
        case 2:
            //prints the data in alphanumeric order
            courses.printAllCourses();
            break;
        case 3: {
            //user will search a specific course
            string courseNumber;
            cout << "Enter course Number: ";
            cin.ignore();
            getline(cin, courseNumber);
            // Convert course to uppercase
            for (int i = 0; i < courseNumber.length(); i++) {
                courseNumber[i] = toupper(courseNumber[i]);
            }

            //course will be searched then found
            CourseData found = courses.searchCourse(courseNumber);

            //checks if a course was brought back
            if (found.courseNumber == "") {
                cout << "Error: Course Not Found!" << endl;
                break;
            }
            else {
                cout << found.courseNumber << ", " << found.courseName << "\n" << "Prerequisites: ";
            }

            //checks if there were any prerequisites and prints them
            if (found.prerequisites.empty()) {
                cout << "None Required" << endl;
            }
            else {
                for (int i = 0; i < found.prerequisites.size(); i++) {
                    cout << found.prerequisites[i];
                    if (i < found.prerequisites.size() - 1) {
                        cout << ", ";
                    }
                }
            }
            cout << endl;
            break;
        }

        case 9:                               
            //exits program
            cout << "Goodbye!" << endl;
            break;
        default:
            //an option not on the menu was pressed
            cout << "Invalid Option" << endl;
            break;
        }
    } while (userInput != 9);
}

/*
* Loads course data from a the file user inputs into the hash table
* Prompts the user for a file name, goes through each line,
* and stores course objects
*/

void storeData(HashTable& courses) {
    //File Name: CS 300 ABCU_Advising_Program_Input.csv

    //variables to hold courses, a file, and a file line
    vector<CourseData> loadCourses;
    string file;    
    string line;    

    //user enters a file
    cout << "Enter file Name: ";
    cin.ignore();
    getline(cin, file);

    //open file user asked for
    ifstream inputFile;
    inputFile.open(file);

    //check if the file opened or not
    if (inputFile.is_open()) {
        cout << "File opened successfully.\n";
    }
    else {
        cout << "Failed to open file.\n";
        return;
    }

    //while there are lines in the file
    while (getline(inputFile, line)) {
        string token;                                           //this will help grab words in file
        vector<string> tokens;                                  //this will store all the token "grabbed words" into tokens
        stringstream parser(line);                              //create a stream to read split the string

        //this will parser the file by commas, checks if two commas "undo" multi "" tokens
        while (getline(parser, token, ',')) {   
            if (token != "") {              
                    tokens.push_back(token);
            }
        }

        //tokens have to be 2 or more | token 1: number | token 2: name |
        if (tokens.size() < 2) {
            cout << "Error: Incomplete Course Data!" << endl;
            continue;
        }
        //putting tokens into the data areas: number, name, and pre
        CourseData course;
        course.courseNumber = tokens[0];
        course.courseName = tokens[1];
        for (int i = 2; i < tokens.size(); i++) {   
            course.prerequisites.push_back(tokens[i]);
        }

        //insert the data into the hash table
        courses.hashInsert(course);

        //insert the data into the vector
        loadCourses.push_back(course);
    }

    //closed the file
    inputFile.close();

    //checks for prerequistes with the vector to make sure everything was fine
    for (int i = 0; i < loadCourses.size(); i++) {
        for (int j = 0; j < loadCourses[i].prerequisites.size(); j++) {
            string pre = loadCourses[i].prerequisites[j];

            CourseData foundCourse = courses.searchCourse(pre);

            if (foundCourse.courseNumber == "") {
                cout << "Error: Prerequisite Not Found!";
            }
        }
    }
}

