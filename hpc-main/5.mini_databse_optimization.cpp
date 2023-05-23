#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// define a struct for databasebase entry
struct databasebaseEntry {
    int id;
    string name;
    int age;
};

// define a vector to hold databasebase entries
vector<databasebaseEntry> databasebase;

// function to add an entry to the databasebase
void addEntry(databasebaseEntry entry) {
    #pragma omp critical
    {
        databasebase.push_back(entry);
    }
}

// function to delete an entry from the databasebase
void deleteEntry(int id) {
    #pragma omp parallel for
    for(int i=0; i<databasebase.size(); i++) {
        if(databasebase[i].id == id) {
            #pragma omp critical
            {
                databasebase.erase(databasebase.begin() + i);
            }
        }
    }
}

// function to update an entry in the databasebase
void updateEntry(int id, string name, int age) {
    #pragma omp parallel for
    for(int i=0; i<databasebase.size(); i++) {
        if(databasebase[i].id == id) {
            #pragma omp critical
            {
                databasebase[i].name = name;
                databasebase[i].age = age;
            }
        }
    }
}

// function to retrieve an entry from the databasebase
databasebaseEntry getEntry(int id) {
    databasebaseEntry result;
    #pragma omp parallel for
    for(int i=0; i<databasebase.size(); i++) {
        if(databasebase[i].id == id) {
            #pragma omp critical
            {
                result = databasebase[i];
            }
        }
    }
    return result;
}

int main() {
    // get number of entries from user
    int numEntries;
    cout << "Enter number of databasebase entries: ";
    cin >> numEntries;

    // get databasebase entries from user
    for(int i=0; i<numEntries; i++) {
        int id, age;
        string name;
        cout << "Enter databasebase entry #" << i+1 << ":" << endl;
        cout << "ID: ";
        cin >> id;
        cout << "Name: ";
        cin >> name;
        cout << "Age: ";
        cin >> age;
        addEntry({id, name, age});
    }

    // delete an entry from the databasebase
    int deleteId;
    cout << "Enter ID of entry to delete: ";
    cin >> deleteId;
    deleteEntry(deleteId);

    // update an entry in the databasebase
    int updateId, updateAge;
    string updateName;
    cout << "Enter ID of entry to update: ";
    cin >> updateId;
    cout << "Enter updated name: ";
    cin >> updateName;
    cout << "Enter updated age: ";
    cin >> updateAge;
    updateEntry(updateId, updateName, updateAge);

    // retrieve an entry from the databasebase
    int getId;
    cout << "Enter ID of entry to retrieve: ";
    cin >> getId;
    databasebaseEntry entry = getEntry(getId);
    cout << "Name: " << entry.name << ", Age: " << entry.age << endl;

    return 0;
}

