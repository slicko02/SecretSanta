//-------------------------------------------------------------------------------------------//
//--------SECRET-SANTA-GENERATOR-----FUNCTIONS:-BUDGET-NAMEPICKER-GIFT-IDEAS-----------------//
//-------------------------------------------------------------------------------------------//
//--------CREATOR:-SOPHIA-LICKO--------------------------------------------------------------//
//-------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------//
//LIBRARIES//
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <cstdlib>
#include <ctime> 
#include <fstream>
#include <numeric>
#include <random>
//-------------------------------------------------------------------------------------------//
//STD//
using namespace std;
//-------------------------------------------------------------------------------------------//
//GLOBAL CONSTANTS//
const std::string RED = "\033[31m";      // Red color
const std::string GREEN = "\033[32m";    // Green color
const std::string YELLOW = "\033[33m";   // Yellow color
const std::string BLUE = "\033[34m";     // Blue color
const std::string RESET = "\033[0m";     // Reset to default color
//-------------------------------------------------------------------------------------------//
//PARTICIPANT STRUCT DEFINITION//
struct Participant {
    string name;
    vector<string> interests;
    string significantOther;
    string secretSanta;
};
//-------------------------------------------------------------------------------------------//
//GET PARTICIPANT INFO SCOPED TO PARTICIPANT STRUCT DEFINITION//
Participant getParticipantInfo() {
    Participant p;
    cout << RED << "Enter participant's name: " << RESET;
    getline(cin, p.name);

    //three Interests
    for (int i = 0; i < 3; ++i) {
        string interest;
        cout << GREEN <<"Enter interest " << (i + 1) << ": " << RESET;
        getline(cin, interest);
        p.interests.push_back(interest);
    }

    //optional Significant Other
    cout << RED << "Enter name of significant other (if applicable, else leave blank): " << RESET;
    getline(cin, p.significantOther);

    return p;
}
//-------------------------------------------------------------------------------------------//
//FIND PARTICIPANT BY NAME FUNCTION//
Participant findParticipantByName(const vector<Participant>& participants, const string& name) {
    for (const auto& participant : participants) {
        if (participant.name == name) {
            return participant;
        }
    }
    //feturn a default Participant if not found
    return Participant{};
}
//-------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------//
//INTRODUCTION FUNCTION DEFINITION//
void introduction(){

cout << endl << endl <<  RED << "Christmas Time is Near!" << endl << endl;
cout << GREEN << "Secret Santa time is here. I am here to help you each pick your secret Santa target :)" << endl << endl;
cout << RED <<"Are you ready to get started? (Yes or No)" << RESET << endl << endl;
}
//-------------------------------------------------------------------------------------------//
//SANTA SLEIGH PRINT FUNCTION DEFINITION//
void santaSleighPrint(){
//not my art!
cout << endl << endl << endl <<
 "  *        *        *        __o    *       *" << endl <<
 "*      *       *        *   /_| _     *" << endl <<
 "  K  *     K      *        O'_)/ \  *    *" << endl <<
 " <')____  <')____    __*   V   \  ) __  *" << endl <<
 "  \ ___ )--\ ___ )--( (    (___|__)/ /*     *" << endl<<
 "*  |   |    |   |  * \ \____| |___/ /  *" << endl <<
 "   |*  |    |   | aos \____________/       *" << endl;
}
//-------------------------------------------------------------------------------------------//
//CHECK START RESPONSE FUNCTION DEFINITION//
void checkStartResponse(string& start){
    if ((start == "Yes" )|| (start == "yes")){
        cout << GREEN << "Perfect, let's begin then." << endl << endl;
    }
    else{
        cout << endl << RED << "Goodbye." << endl;
        exit(1);
    }
}
//-------------------------------------------------------------------------------------------//
//GET BUDGET FUNCTION DEFINITION//
double getBudget(double& budget){
    string response = " ";
    cout << RED << "First things first, what budget do you wish to set?" << endl << endl;
    cout << YELLOW << "$ " << RESET;
    cin >> budget;

    //tried to handle if not int case but causes seg fault >-<
    // if (budget){
    //     cout << "Invalid input. Budget must be an integer." << endl;
    //     budget = 0;
    //     return getBudget(budget);
    // }
    cout << endl << GREEN << "Awesome! Is "<< YELLOW << "$" << fixed << setprecision(2) << budget << GREEN << " the correct amount you want to set as the budget? (Yes or No)" << RESET << endl <<endl;
    cin >> response;

       if ((response == "Yes" )|| (response == "yes")){
        cout << endl << RED << "Perfect, let's continue.";
        return budget;
    }
    else{
        cout << RED << "I'm sorry for the trouble, please, try again." << RESET << endl;
        budget = 0.00;
        getBudget(budget);
    }
    return budget;
    
}
//-------------------------------------------------------------------------------------------//
//GET NUM OF PARTICIPANTS FUNCTION DEFINITION//
int getNumofParticipants(int& participants){
    string response = "";
    cout << endl << endl << GREEN << "Please enter the number of participants: "<< RESET << endl << endl;
    cin >> participants;
    cout << endl << RED << "Is " << YELLOW<<  participants << RED << " the correct amount of participants? (Yes or No)" << RESET << endl << endl;
    cin >> response;
    if ((response == "Yes" )|| (response == "yes")){
        cout << endl << GREEN << "Perfect, let's continue." << endl << endl;
        return participants;
    }
    else{
        cout << GREEN << "I'm sorry for the trouble, please, try again." << RESET << endl;
        participants = 0;
        getNumofParticipants(participants);
    }
    return participants;
}
//-------------------------------------------------------------------------------------------//
//ASSIGN SECRET SANTAS FUNCTION DEFINITION
void assignSecretSantas(vector<Participant>& participants) {
    vector<int> indices(participants.size());
    iota(indices.begin(), indices.end(), 0); //fill indices with 0, 1, ..., n-1

    auto rng = default_random_engine{};
    rng.seed(std::random_device()()); //seed with a real random value, if available

    bool validAssignment;
    do {
        shuffle(indices.begin(), indices.end(), rng); //shuffle the indices using rng

        validAssignment = true;
        for (size_t i = 0; i < participants.size(); ++i) {
            const auto& participant = participants[i];
            const auto& secretSantaTarget = participants[indices[i]];

            if (participant.name == secretSantaTarget.name || 
                (!participant.significantOther.empty() && participant.significantOther == secretSantaTarget.name)) {
                validAssignment = false;
                break;
            }
        }
    } while (!validAssignment);

    for (size_t i = 0; i < participants.size(); ++i) {
        participants[i].secretSanta = participants[indices[i]].name;
    }
}
//-------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------//
//WRITE SECRET SANTA ASSIGNMENTS TO FILES//
void writeAssignmentsToFile(const vector<Participant>& participants, double budget) {
    for (const auto& participant : participants) {
        ofstream file(participant.name + ".txt");
        auto target = findParticipantByName(participants, participant.secretSanta); //find the target participant

        file << "Your Secret Santa target is: " << participant.secretSanta << "\n" << endl;
        file << "Interests: " << endl << endl;
        for (const auto& interest : target.interests) {
            file << interest << " " << endl;
        }
        file << "\nBudget: $" << fixed << setprecision(2) << budget << "\n";
        file.close();
    }
}
//-------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------//
//START OF MAIN FUNCTION//
int main(){
 santaSleighPrint();
    introduction();

    string start;
    cin >> start;
    cout << endl;
    checkStartResponse(start);

    double budget = 0.00;
    cout << RED << "First things first, what budget do you wish to set?" << endl << endl;
    cout << YELLOW << "$ " << RESET;
    cin >> budget;
    //clearing the input buffer after reading budget
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int participants = 0;
    cout << endl << endl << GREEN << "Please enter the number of participants: " << RESET << endl << endl;
    cin >> participants;
    //clearing the input buffer again after reading number of participants
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<Participant> participantsList(participants);

    //get participant info
    for (int i = 0; i < participants; i++) {
        participantsList[i] = getParticipantInfo();
    }

    //assign Secret Santas
    assignSecretSantas(participantsList);

    //write assignments to files
    writeAssignmentsToFile(participantsList, budget);

    cout << "Secret Santa assignments have been completed and saved to files." << endl;

    return 0;
}
//END OF MAIN FUNCTION//
//-------------------------------------------------------------------------------------------//