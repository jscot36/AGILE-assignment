#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Availability {
    string day;
    int startHour;  // 24-hour format because it's useful in code logic
    int endHour;
};

//profile struct contains list of class names and then times when available
struct Profile {
    string name;
    vector<string> classes;
    vector<Availability> availability; 
};

//universal list of profiles to edit
vector<Profile> profiles;

//overload needed for comparison in editing profile
bool operator==(const Availability& main, const Availability& other){
    return (main.day == other.day) && (main.startHour == other.startHour) && (main.endHour == other.endHour);
}

//used to standardize inputs
string normalizeName(string name) {
    if (name.empty()) return name;

    // Lowercase everything first
    for (auto &ch : name) {
        ch = tolower(ch);
    }

    // Capitalize first letter
    name[0] = toupper(name[0]);
    return name;
}


//checks if availabilities match via having same day and overlapping times
bool overlaps(const Availability& a, const Availability& b) {
    return (a.day == b.day) && !(a.endHour <= b.startHour || b.endHour <= a.startHour);
}

//function name explains the point, user is meant to add each class first and then whenever they are available
void createProfile() {
    Profile prof;
    string userInput;
    Availability avail;
    char day[10];
    int start, end;

    cout << "Enter your name: ";
    getline(cin, userInput);
    prof.name = normalizeName(userInput);

    cout << "Enter classes (type 'done' when finished):\n";
    while (true) {
        cout << "Class: ";
        getline(cin, userInput);
        if (userInput == "done") break;
        prof.classes.push_back(normalizeName(userInput));
    }

    cout << "Enter availability slots in format (Day StartHour EndHour). Type 'done' when finished.\n";
    cout << "Example: Mon 14 16 (for Monday 2PM-4PM)\n";

    //Availability loop
    while (true) {
        cout << "Availability: ";
        getline(cin, userInput);
        if (userInput == "done") break;

        sscanf(userInput.c_str(), "%s %d %d", day, &start, &end);
        avail.day = normalizeName(day);
        avail.startHour = start;
        avail.endHour = end;
        prof.availability.push_back(avail);
    }

    profiles.push_back(prof);
    cout << "Profile created successfully!\n";
}

void viewProfiles() {

    if (profiles.empty()) {
        cout << "No profiles available.\n";
        return;
    }

    for (int i = 0; i < profiles.size(); i++) {
        cout << "--- Profile " << i+1 << " ---\n";
        cout << "Name: " << profiles[i].name << "\nClasses: ";
        for (string currClass : profiles[i].classes) cout << currClass << " ";

        cout << "\nAvailability: \n";
        for (Availability currAvail : profiles[i].availability) {
            cout << "  " << currAvail.day << " " << currAvail.startHour << "-" << currAvail.endHour << "\n";
        }
        cout << "\n";
    }
}

void findMatches() {

    if (profiles.size() < 2) {
        cout << "Not enough profiles to match.\n";
        return;
    }

    string className = "";
    bool sharedClass;
    int meetingOptions = 0;
    int choice;
    int i,j;

    for (i = 0; i < profiles.size(); i++) {

        for (j = i+1; j < profiles.size(); j++) {
            //Check for a shared class
            sharedClass = false;
            for (string currClass : profiles[i].classes) {
                if (find(profiles[j].classes.begin(), profiles[j].classes.end(), currClass) != profiles[j].classes.end()) {
                    sharedClass = true;
                    className = currClass;
                    break;
                }
            }

            if (!sharedClass) continue;
            //Checks overlapping availabilities
            for (Availability available1 : profiles[i].availability) {
                for (Availability available2 : profiles[j].availability) {
                    
                    if (overlaps(available1, available2)) {
                        cout << "Match found!\n";
                        cout << "Option: " << ++meetingOptions << endl; 
                        cout << profiles[i].name << " and " << profiles[j].name << " can study together for " << className << ".\n";
                        cout << "Shared availability: " << available1.day << " " << max(available1.startHour, available2.startHour)
                             << "-" << min(available1.endHour, available2.endHour) << endl << endl; 
                             //min and max are for correcting the availability shown to when both students can be there
                    }

                }
            }
            // "Confirm" a meeting time
            cout << "Which meeting would you like to request?\n"; 
            cin >> choice;
            cin.ignore();

            //Doesn't really go anywhere but would be sent to respective users.
            cout << "Request Sent!\n\n";
        }
    }

}

void editProfile() {
    
    string userInput;
    int choice;
    int addOrDelete;
    int start, end;
    char day[10];
    Profile currProfile;
    Availability avail;
    
    
    viewProfiles();
    cout << "What profile would you like to edit? (type the profile name) \n";
    getline(cin,userInput);

    for (int i = 0; i < profiles.size(); i++) {

        if (profiles[i].name == userInput){
            currProfile = profiles[i];

            cout << "What would you like to edit?\n";
            cout << "1. Add/Remove Classes\n";
            cout << "2. Add/Remove Availability\n";
            cout << "3. Delete Profile\n";
            cout << "4. Exit\n";
            cin >> choice;
            cin.ignore();

            if (choice == 1){
                cout << "Would you like to add or remove?\n";
                cout << "1. Add\n";
                cout << "2. Delete\n";
                cin >> addOrDelete;
                cin.ignore();

                if (addOrDelete == 1) {
                    cout << "Enter classes (type 'done' when finished):\n";
                    
                    while (true) {
                        cout << "Class: ";
                        getline(cin, userInput);
                        if (userInput == "done") break;
                        currProfile.classes.push_back(normalizeName(userInput));
                    }
                }

                else if (addOrDelete == 2){
                    cout << "What class would you like to remove?\n";
                    getline(cin,userInput);

                    for(int j = 0;  j < currProfile.classes.size(); j++){
                        
                        if (currProfile.classes[j] == userInput){
                            currProfile.classes.erase(currProfile.classes.begin()+j);
                        }

                    }
                }
                profiles[i] = currProfile;
            }
            else if (choice == 2){

                cout << "Would you like to add or remove?\n";
                cout << "1. Add\n";
                cout << "2. Delete\n";
                cin >> addOrDelete;
                cin.ignore();

                if (addOrDelete == 1){

                    cout << "Enter availability slots in format (Day StartHour EndHour). Type 'done' when finished.\n";
                    cout << "Example: Mon 14 16 (for Monday 2PM-4PM)\n";

                    //Availability loop
                    while (true) {
                        cout << "Availability: ";
                        getline(cin, userInput);
                        if (userInput == "done") break;
                        
                        sscanf(userInput.c_str(), "%s %d %d", day, &start, &end);
                        avail.day = normalizeName(day);
                        avail.startHour = start;
                        avail.endHour = end;
                        currProfile.availability.push_back(avail);
                    }
                
                }

                else if (addOrDelete == 2){
                    cout << "What time would you like to remove?\n";
                    cout << "Input it as: Mon 14 16\n";
                    getline(cin,userInput);
                    sscanf(userInput.c_str(), "%s %d %d", day, &start, &end);
                    avail.day = day;
                    avail.startHour = start;
                    avail.endHour = end;

                    for(int j = 0;  j < currProfile.availability.size(); j++){
                        
                        if (currProfile.availability[j] == avail){
                            currProfile.availability.erase(currProfile.availability.begin()+j);
                        }

                    }
                }
                 profiles[i] = currProfile;           
            }
            
            else if (choice == 3){
                profiles.erase(profiles.begin()+i);
            }
            break;
        }


    }


}

int main() {
    
    int choice;

    while (true) {
        cout << "===== Study Buddy App =====\n";
        cout << "1. Create Profile\n";
        cout << "2. View Profiles\n";
        cout << "3. Find Matches\n";
        cout << "4. Edit Profile\n";
        cout << "5. Exit\n";
        cout << "Enter choice (pick a number): ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: createProfile(); break;
            case 2: viewProfiles(); break;
            case 3: findMatches(); break;
            case 4: editProfile(); break;
            case 5: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}
