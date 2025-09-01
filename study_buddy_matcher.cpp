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

vector<Profile> profiles;

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
    getline(cin, prof.name);

    cout << "Enter classes (type 'done' when finished):\n";
    while (true) {
        cout << "Class: ";
        getline(cin, userInput);
        if (userInput == "done") break;
        prof.classes.push_back(userInput);
    }

    cout << "Enter availability slots in format (Day StartHour EndHour). Type 'done' when finished.\n";
    cout << "Example: Mon 14 16 (for Monday 2PM-4PM)\n";

    //Availability loop
    while (true) {
        cout << "Availability: ";
        getline(cin, userInput);
        if (userInput == "done") break;

        avail.day = day;
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

    for (size_t i = 0; i < profiles.size(); i++) {
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

    for (size_t i = 0; i < profiles.size(); i++) {

        for (size_t j = i+1; j < profiles.size(); j++) {
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

int main() {
    int choice;
    while (true) {
        cout << "===== Study Buddy App =====\n";
        cout << "1. Create Profile\n";
        cout << "2. View Profiles\n";
        cout << "3. Find Matches\n";
        cout << "4. Exit\n";
        cout << "Enter choice (pick a number): ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: createProfile(); break;
            case 2: viewProfiles(); break;
            case 3: findMatches(); break;
            case 4: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}
