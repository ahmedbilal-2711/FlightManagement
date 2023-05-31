#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <stdexcept>

using namespace std;
string availableFlights[15][10];
string FLIGHTS[35][10];
class User
{
public:
    User() {}
    User(string cnic, string password) : cnic(cnic), password(password) {}

    bool login(string users[][3], int num_users)
    {

        for (int i = 0; i < num_users; i++)
        {
            if (users[i][0] == cnic && users[i][1] == password)
            {
                cout << "\nLogin successful!" << endl;
                return true;
            }
        }
        cout << "\nInvalid username or password." << endl;
        return false;
    }
    bool login(string users[][2], int num_users)
    {
        for (int i = 0; i < num_users; i++)
        {
            if (users[i][0] == cnic && users[i][1] == password)
            {
                cout << "\nLogin successful!" << endl;
                return true;
            }
        }
        cout << "\nInvalid username or password." << endl;
        return false;
    }
    int userMenu()
    {
        int choice = 0;
        while (choice < 1 || choice > 3)
        {
            cout << "Main Menu:" << endl;
            cout << "1. Login as Admin" << endl;
            cout << "2. Login as User" << endl;
            cout << "3. view schedule" << endl;
            cout << "Enter your choice: " << endl;
            cin >> choice;
            if (choice < 1 || choice > 3)
            {
                cout << "Invalid input. Please enter a number from 1 to 5." << endl;
            }
        }

        return choice;
    }

    string passwordWriter()
    {
        std::string password = "";
        char ch;
        try
        {
            while ((ch = _getch()) != '\r')
            {
                if (ch == '\b')
                {
                    if (!password.empty())
                    {
                        password.erase(password.length() - 1);
                        std::cout << "\b \b";
                    }
                }
                else
                {
                    password += ch;
                    std::cout << "*";
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "An exception occurred: " << e.what() << std::endl;
            password = "";
        }
        return password;
    }

    bool isValidCNIC(string cnic)
    {
        return cnic.length() == 13;
    }
    bool isValidPassword(const std::string &password)
    {
        if (password.length() != 8)
        {
            return false;
        }

        bool hasSpecialChar = false;
        bool hasUppercase = false;
        bool hasLowercase = false;
        bool hasDigit = false;

        for (int i = 0; i < password.length(); i++)
        {
            char c = password[i];
            if (!std::isalnum(c))
            {
                hasSpecialChar = true;
            }
            if (std::isupper(c))
            {
                hasUppercase = true;
            }
            if (std::islower(c))
            {
                hasLowercase = true;
            }
            if (std::isdigit(c))
            {
                hasDigit = true;
            }
        }

        return hasSpecialChar && hasUppercase && hasLowercase && hasDigit;
    }

    string getCnic()
    {
        bool validCnic = false;
        string cnic = "";
        while (!validCnic)
        {
            cout << "Enter cnic" << endl;
            cin >> cnic;
            validCnic = isValidCNIC(cnic);
            if (validCnic)
            {
                for (char c : cnic)
                {
                    if (!isdigit(c))
                    {
                        validCnic = false;
                        cout << "Do not enter alphabets or special character in cnic " << endl;
                        break;
                    }
                }
            }
            else
            {
                cout << "Enter a valid cnic of 13 digits " << endl;
            }
        }
        return cnic;
    }

    string getPassword()
    {
        bool validPassword = false;
        string password = "";
        cout << "Enter password: " << endl;
        while (!validPassword)
        {
            password = passwordWriter();
            validPassword = isValidPassword(password);
            if (!validPassword)
            {
                cout << "\nEnter a valid Password " << endl;
                cout << " Password must be 8 characters long and use of minimum one special character, uppercase, lowercase, and numeric digit is must" << endl;
            }
        }
        return password;
    }

protected:
    string cnic;
    string password;
};

class Passenger : public User
{
public:
    Passenger(string cnic, string password) : User(cnic, password)
    {
        totalPassengers = 0;
        ifstream file;
        file.open("users.txt");

        for (int i = 0; i < 100; i++)
        {
            totalPassengers = i;

            if (file.eof())
            {

                return;
            }
            for (int j = 0; j < 3; j++)
            {

                file >> passengers[i][j]; // from file to array
            }
        }
    }

    string passengers[100][3];
    int totalPassengers;
    string bookedFlights[10][11];

    int num_passengers = sizeof(passengers) / sizeof(passengers[0]);

    int totalBookedFlights()
    {
        int flightsBooked = 0;

        ifstream inputfile;
        string filename = cnic.append(".txt");
        inputfile.open(filename);

        for (int i = 0; i < 10; i++)
        {
            if (inputfile.eof())
            {
                break;
            }
            flightsBooked = flightsBooked + 1;
            for (int j = 0; j < 11; j++)
            {
                inputfile >> bookedFlights[i][j];
            }
        }
        for (int i = 0; i < flightsBooked; i++)
        {
            if (bookedFlights[i][0].empty())
            {
                break;
            }
            cout << i + 1 << ". ";
            for (int j = 0; j < 11; j++)
            {
                cout << bookedFlights[i][j] << " ";
            }
            cout << endl;
        }

        return flightsBooked;
    }

    bool registerUser(string visaStatus)
    {
        int lastrowindex = 0;
        for (int i = 0; i < num_passengers; i++)
        {

            if (passengers[i][0] == cnic)
            {
                cout << "\nUser already exists You may login\n";

                return false;
            }
            if (passengers[i][0].empty())
            {
                lastrowindex = i;
                break;
            }
        }

        passengers[lastrowindex][0] = cnic;
        passengers[lastrowindex][1] = password;
        ofstream outfile;
        outfile.open("users.txt", ios::app);
        outfile << "\n";
        outfile << cnic << " ";
        outfile << password << " ";
        outfile << visaStatus;
        return true;
    }
    void bookFlight(int flightNumber, int seatsBooked)
    {
        int seatsLeft;
        string str;
        int price;
        int hoursRequired;
        string flightType;
        int pricePerHour;
        int totalPrice;

        int userIndex = 0;
        for (int i = 0; i < 10; i++)
        {
            userIndex = i;

            if (passengers[i][0] == cnic)
            {
                break;
            }
        }
        if (availableFlights[flightNumber][7] == "local" || (availableFlights[flightNumber][7] == "international" && passengers[userIndex][2] == "stamped"))
        {
            try
            {
                seatsLeft = stoi(availableFlights[flightNumber][9]) - seatsBooked;
                str = to_string(seatsLeft);
                flightType = availableFlights[flightNumber][7];
                hoursRequired = stoi(availableFlights[flightNumber][8]);

                if (flightType == "international")
                {
                    pricePerHour = 25000;
                }
                else if (flightType == "local")
                {
                    pricePerHour = 10000;
                }

                totalPrice = seatsBooked * pricePerHour;
            }
            catch (const invalid_argument &e)
            {
                cout << "Error: " << e.what() << endl;
            }
            ofstream outfile;
            string filename = cnic.append(".txt");
            outfile.open(filename, ios::app);
            outfile << availableFlights[flightNumber][0] << " "; // from array to file
            outfile << availableFlights[flightNumber][1] << " ";
            outfile << availableFlights[flightNumber][2] << " ";
            outfile << availableFlights[flightNumber][3] << " ";
            outfile << availableFlights[flightNumber][4] << " ";
            outfile << availableFlights[flightNumber][5] << " ";
            outfile << availableFlights[flightNumber][6] << " ";
            outfile << availableFlights[flightNumber][7] << " ";
            outfile << availableFlights[flightNumber][8] << " ";
            outfile << seatsBooked << " ";
            outfile << totalPrice << "\n";

            for (int i = 0; i < 35; i++)
            {
                if (FLIGHTS[i][0] == availableFlights[flightNumber][0])
                {
                    FLIGHTS[i][9] = str;
                    ofstream outfile;
                    outfile.open("flights.txt", ios::trunc);
                    for (int i = 0; i < 35; i++)
                    {
                        for (int j = 0; j < 10; j++)
                        {
                            outfile << FLIGHTS[i][j] << " "; // from array to file
                        }
                        outfile << endl;
                    }
                    break;
                }
            }

            outfile.close();
            cout << "YOUR TICKET HAS BEEN BOOKED SUCCESSFULLY." << endl;
            cout << "YOUR BOOKING WILL COST " << totalPrice;
        }
        else
        {
            cout << "Booking FAILED! \n Your passport is not stamped! Kindly book again after stamping the passport.";
        }
    }

    void cancelFlight(int allFlights, int flightNumber)
    {
        try
        {
            std::string cancelledFlightId = bookedFlights[flightNumber][0];
            int cancelledSeats = std::stoi(bookedFlights[flightNumber][9]);

            for (int i = 0; i < 35; i++)
            {
                if (FLIGHTS[i][0] == bookedFlights[flightNumber][0])
                {
                    FLIGHTS[i][9] = std::to_string(cancelledSeats + std::stoi(FLIGHTS[i][9]));
                    std::ofstream outfile;
                    outfile.open("flights.txt", std::ios::trunc);
                    for (int i = 0; i < 35; i++)
                    {
                        for (int j = 0; j < 10; j++)
                        {
                            outfile << FLIGHTS[i][j] << " "; // from array to file
                        }
                        outfile << std::endl;
                    }
                    break;
                }
            }

            float price = 0;
            std::ofstream outfile;
            outfile.open(cnic, std::ios::out | std::ios::trunc);
            for (int i = 0; i < allFlights; i++)
            {
                if (i == flightNumber)
                {
                    price = std::stof(bookedFlights[i][10]);
                    continue;
                }
                for (int j = 0; j < 11; j++)
                {
                    outfile << bookedFlights[i][j] << " ";
                }
                outfile << std::endl;
            }
            outfile.close();
            price = price - (0.25 * price);
            std::cout << "\n YOUR FLIGHT " << cancelledFlightId << " IS CANCELLED AFTER DEDUCTION OF 25%. RS" << price;
        }
        catch (const std::exception &e)
        {
            std::cerr << "An exception occurred: " << e.what() << std::endl;
        }
    }

    void updateFlight(int allFlights, int flightNumber)
    {

        int updateChoice;
        string update;
        cout << "Enter the number of the field you want to update: " << endl;
        cout << "1. Departure Date" << endl;
        cout << "3. Departure Time" << endl;
        cout << "5. Departure Place" << endl;
        cout << "9. Seats" << endl;
        cin >> updateChoice;
        updateChoice = updateChoice;
        cout << "What update you want to do\n";
        cin >> update;
        cout << update;
        bookedFlights[flightNumber][updateChoice] = update;

        cout << "\n"
             << bookedFlights[flightNumber][updateChoice];
        ofstream outfile;
        outfile.open(cnic, ios::out | ios::trunc);
        for (int i = 0; i < allFlights; i++)
        {

            for (int j = 0; j < 11; j++)
            {
                outfile << bookedFlights[i][j] << " ";
            }
            outfile << endl;
        }
        outfile.close();
    }

    int checkUser()
    {
        int index = 0;
        for (int i = 0; i < 100; i++)
        {
            if (cnic == passengers[i][0])
            {
                index = i;
                return index;
            }
        }
        return -1;
    }

    void update_user()
    {
        string cnic;
        int field_number;
        string new_value;

        cout << "Enter the cnic: ";

        cnic = getCnic();

        // Open input and output file streams
        ifstream in_file("users.txt");
        ofstream out_file("temp.txt");
        if (!in_file || !out_file)
        {
            cout << "Error opening file!" << endl;
            return;
        }

        bool found = false;
        string line;
        int line_number = 1;
        while (getline(in_file, line))
        {
            istringstream iss(line);
            string word;
            string words[9];
            int i = 0;
            while (iss >> word && i < 9)
            {
                words[i] = word;
                i++;
            }

            if (words[0] == cnic)
            {
                found = true;
                cout << "Enter the number of the field you want to update: " << endl;
                cout << "1. CNIC" << endl;
                cout << "2. Password" << endl;
                cout << "3. Visa Stamp" << endl;

                while (field_number < 1 || field_number > 3)
                {
                    cin >> field_number;
                    if (field_number < 1 || field_number > 3)
                    {
                        cout << "Enter the number of the field you want to update: " << endl;
                    }
                }
                cout << "Enter the new value: ";

                if (field_number == 1)
                {
                    new_value = getCnic();
                }
                else if (field_number == 2)
                {
                    new_value = getPassword();
                }
                else if (field_number == 3)
                {
                    
                        cout << "Enter 'stamped' or 'unstamped'" << endl;
                        cin >> new_value;
                    
                }

                words[field_number - 1] = new_value;

                // Concatenate words to form the new line
                string new_line = words[0];
                for (int i = 1; i < 9; i++)
                {
                    new_line += " " + words[i];
                }

                // Replace the old line with the new line in the output file
                out_file << new_line << endl;
            }
            else
            {
                // Just copy the old line to the output file
                out_file << line << endl;
            }
            line_number++;
        }

        // Close input and output file streams
        in_file.close();
        out_file.close();

        // Delete the old file and rename the temporary file
        remove("users.txt");
        rename("temp.txt", "users.txt");

        if (!found)
        {
            cout << "User CNIC not found." << endl;
        }
    }
    int checkSeats(int flightNumber)
    {
        int seatsLeft = stoi(availableFlights[flightNumber][9]);
        return seatsLeft;
    }
};
class Admin : public User
{
public:
    Admin(string cnic, string password) : User(cnic, password) {}
    string admins[100][2] = {{"1234567890987", "admiN12#"}, {"user2", "password2"}, {"user3", "password3"}};
    int num_admin = sizeof(admins) / sizeof(admins[0]);

    void adminmenu()
    {
        cout << "Admin Menu:" << endl;
        cout << "1. Change schedule of flights" << endl;
        cout << "2. Add new routes" << endl;
        cout << "3. Restrict number passengers" << endl;
        cout << "4. Update airline inquiry" << endl;
    }

    void update_flight2()
    {
        string flight_id;
        int field_number = 0;
        string new_value;
        cout << "Enter the flight ID: ";
        cin >> flight_id;
        ifstream in_file("flights.txt");
        ofstream out_file("temp.txt");
        if (!in_file || !out_file)
        {
            cout << "Error opening file!" << endl;
            return;
        }

        bool found = false;
        string line;
        int line_number = 1;
        while (getline(in_file, line))
        {
            istringstream iss(line);
            string word;
            string words[9];
            int i = 0;
            while (iss >> word && i < 9)
            {
                words[i] = word;
                i++;
            }

            if (words[0] == flight_id)
            {
                found = true;
                while (field_number < 1 || field_number > 9)
                {
                    cout << "Enter the number of the field you want to update: " << endl;
                    cout << "1. Departure Date" << endl;
                    cout << "2. Arrival Date" << endl;
                    cout << "3. Departure Time" << endl;
                    cout << "4. Arrival Time" << endl;
                    cout << "5. Departure Place" << endl;
                    cout << "6. Arrival Place" << endl;
                    cout << "7. Flight Type" << endl;
                    cout << "8. Flight Duration" << endl;
                    cout << "9. Seats Left" << endl;
                    cin >> field_number;
                    if (field_number < 1 || field_number > 9)
                    {
                        cout << "Invalid input. Please enter a number from 1 to 9." << endl;
                    }
                }

                cout << "Enter the new value: ";
                cin >> new_value;

                words[field_number - 1] = new_value;

                // Concatenate words to form the new line
                string new_line = words[0];
                for (int i = 1; i < 9; i++)
                {
                    new_line += " " + words[i];
                }

                // Replace the old line with the new line in the output file
                out_file << new_line << endl;
            }
            else
            {
                // Just copy the old line to the output file
                out_file << line << endl;
            }
            line_number++;
        }

        // Close input and output file streams
        in_file.close();
        out_file.close();

        // Delete the old file and rename the temporary file
        remove("flights.txt");
        rename("temp.txt", "flights.txt");

        if (!found)
        {
            cout << "Flight ID not found." << endl;
        }
    }

    int count_planes_arriving(string arrival_place, string arrival_date, string arrival_time)
    {
        ifstream file("flights.txt");
        int count = 0;

        if (file)
        {
            string line;
            while (getline(file, line))
            {
                istringstream iss(line);
                string word;
                string words[9];
                int i = 0;

                while (iss >> word && i < 9)
                {
                    words[i] = word;
                    i++;
                }

                if (words[6] == arrival_place && words[2] == arrival_date && words[4] == arrival_time)
                {
                    // Increment the count if arrival_place, arrival_date, and arrival_time match
                    count++;
                }
            }
            file.close();
        }
        else
        {
            cout << "Error opening file." << endl;
            return -1; // Return -1 to indicate an error
        }

        return count;
    }

    bool check_flight_clash(string plane_id, string departure_date, string arrival_date, string departure_time, string arrival_time, string departure_place, string arrival_place)
    {
        ifstream file("flights.txt");
        if (file)
        {
            string line;
            while (getline(file, line))
            {
                istringstream iss(line);

                string word;
                string words[9];
                int i = 0;
                while (iss >> word && i < 9)
                {
                    words[i] = word;
                    i++;
                }

                if (plane_id == words[0] && (departure_date == words[1] || arrival_date == words[2]))
                {
                    if (words[3] == departure_time)
                    {
                        // clash in departure time
                        cout << "Error: Plane with ID " << plane_id << " already has a flight scheduled at the same time on " << departure_date << endl;
                        return false;
                    }
                    else if (words[4] == arrival_time)
                    {
                        // clash in arrival time
                        cout << "Error: Plane with ID " << plane_id << " already has a flight scheduled at the same time on " << arrival_date << endl;
                        return false;
                    }
                }
            }
            file.close();
            if (count_planes_arriving(arrival_place, arrival_date, arrival_time) > 5)
            {
                cout << "Error: Plane with ID " << plane_id << " cannot be added to the flight schedule as airport is busy!" << endl;
                return false;
            }
        }
        else
        {
            cout << "Error opening file." << endl;
            return false;
        }

        return true;
    }

    void add_plane()
    {
        // Open the planes.txt file in append mode to add a new line
        ofstream out_file("flights.txt", ios::app);
        if (!out_file)
        {
            cerr << "Error opening file!" << endl;
            return;
        }

        // Get the last plane id from the planes.txt file
        string last_plane_id;
        ifstream file("flights.txt");
        if (file)
        {
            string line;
            while (getline(file, line))
            {
                last_plane_id = line.substr(0, line.find(" "));
            }
        }
        file.close();
        // Ask user to enter the details for the new plane
        string plane_id;
        string departure_date, arrival_date, departure_time, arrival_time, departure_place, arrival_place, flight_type, flight_duration, seats_left;
        cout << "Enter the details for the new plane:" << endl;
    planeId:
        cout << "Plane Id: ";
        getline(cin, plane_id);
        bool id_exists = false;
        string line;
        ifstream in_file("flights.txt");
        while (getline(in_file, line))
        {
            istringstream iss(line);
            string token;
            // Skip first six tokens
            for (int i = 0; i < 7; i++)
            {
                iss >> token;
            }

            // Get the flight type token
            iss >> token;
            // cout << token;
            if (line.substr(0, line.find(" ")) == plane_id && token == "International")
            {
                id_exists = true;
            }
        }
        in_file.close();
        if (id_exists)
        {
            cout << "Error: Plane ID already exists." << endl;
            goto planeId;
        }
        cout << "Departure Date: ";
        getline(cin, departure_date);
        cout << "Arrival Date: ";
        getline(cin, arrival_date);
        cout << "Departure Time: ";
        getline(cin, departure_time);
        cout << "Arrival Time: ";
        getline(cin, arrival_time);
        cout << "Departure Place: ";
        getline(cin, departure_place);
        cout << "Arrival Place: ";
        getline(cin, arrival_place);
        if (departure_place == "Karachi" || departure_place == "Lahore" || departure_place == "Islamabad" || departure_place == "Quetta" || departure_place == "Peshawar" ||
            arrival_place == "Karachi" || arrival_place == "Lahore" || arrival_place == "Islamabad" || arrival_place == "Quetta" || arrival_place == "Peshawar")
        {
            flight_type = "Local";
        }
        else
        {
            flight_type = "International";
        }
        cout << "Flight Duration: ";
        getline(cin, flight_duration);
        do
        {
            cout << "Seats Left: ";
            getline(cin, seats_left);
        } while (stoi(seats_left) > 50 || stoi(seats_left) < 0);
        if (check_flight_clash(plane_id, departure_date, arrival_date, departure_time, arrival_time, departure_place, arrival_place))

        {
            // Write the new line to the file
            out_file << plane_id << " " << departure_date << " " << arrival_date << " " << departure_time << " " << arrival_time << " "
                     << departure_place << " " << arrival_place << " " << flight_type << " " << flight_duration << " " << seats_left << endl;

            out_file.close();

            cout << "New plane schedule is added" << endl;
        }
        else
        {
            cout << "New plane schedule failed to add" << endl;
        }
    }
};

void displayFlights()
{

    ifstream file;
    file.open("flights.txt");
    for (int i = 0; i < 35; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            file >> FLIGHTS[i][j]; // from file to array
        }
    }
}
int findFlightsFrom(string city)
{
    cout << "YOU ARE LOOKING FOR FLIGHTS FROM " << city;
    int flightsFound = -1;
    string cityName;
    for (int i = 0; i < 35; i++)
    {
        size_t pos = FLIGHTS[i][5].find("(");
        cityName = FLIGHTS[i][5].substr(0, pos);
        if ((cityName) == (city))
        {
            flightsFound = flightsFound + 1;

            for (int j = 0; j < 10; j++)
            {
                availableFlights[flightsFound][j] = FLIGHTS[i][j];
            }
        }
    }
    cout << "\nTOTAL FLIGHTS FOUND : " << flightsFound;

    return flightsFound;
}
void printAvailableFlights(int totalFlights)
{
    for (int i = 0; i < totalFlights; i++)
    {
        cout << i + 1 << ". ";
        for (int j = 0; j < 10; j++)
        {
            cout << availableFlights[i][j] << " ";
        }
        cout << endl;
    }
}

int departureDetails()
{
    int departurePlace = 0;
    int numberOfFlights;
    cout << "Enter place of departure" << endl;
    while (departurePlace < 1 || departurePlace > 5)
    {
        cout << "1. Islamabad" << endl;
        cout << "2. Quetta" << endl;
        cout << "3. Peshawar" << endl;
        cout << "4. Lahore" << endl;
        cout << "5. Karachi" << endl;

        cin >> departurePlace;

        if (departurePlace < 1 || departurePlace > 5)
        {
            cout << "Invalid input. Please enter a number from 1 to 5." << endl;
        }
    }

    if (departurePlace == 1)
    {
        numberOfFlights = findFlightsFrom("islamabad");
    }
    else if (departurePlace == 2)
    {
        numberOfFlights = findFlightsFrom("quetta");
    }
    else if (departurePlace == 3)
    {
        numberOfFlights = findFlightsFrom("peshawar");
    }
    else if (departurePlace == 4)
    {
        numberOfFlights = findFlightsFrom("lahore");
    }
    else if (departurePlace == 5)
    {
        numberOfFlights = findFlightsFrom("karachi");
    }
    cout << "\nnumber of flights available " << numberOfFlights << endl;
    return numberOfFlights;
}
int main()
{
    cout << "WELCOME TO NAFS" << endl;
    int seatsBooked;
    int arrivalPlace;
    int numberOfFlights;
    int choice = 0;
    int passengerChoice = 0;
    bool loginStatus = false;
    displayFlights();
    User user;
    choice = user.userMenu();

    while (choice == 3)
    {
        numberOfFlights = departureDetails();
        printAvailableFlights(numberOfFlights);
        cout << "\nTO BOOK A FLIGHT YOU MUST LOGIN OR SIGNUP" << endl;
        choice = user.userMenu();
    }
    string cnic;
    string password;

    if (choice == 1)
    {
        cnic = user.getCnic();
        password = user.getPassword();
        Admin admin(cnic, password);
        if (admin.login(admin.admins, admin.num_admin))
        {
        adminLabel:
            int adminChoice = 0;
            while (adminChoice < 1 || adminChoice > 2)
            {
                cout << "Admin Menu:" << endl;
                cout << "1. Update Flight" << endl;
                cout << "2. Add Flight" << endl;
                cout << "Enter your choice: " << endl;
                cin >> adminChoice;
                if (adminChoice < 1 || adminChoice > 2)
                {
                    cout << "Enter a correct number" << endl;
                }
            }
            if (adminChoice == 1)
            {
                admin.update_flight2();
                goto adminLabel;
            }
            else if (adminChoice == 2)
            {
                admin.add_plane();
                goto adminLabel;
            }
        }
    }
    else if (choice == 2)
    {
        cout << "OK";
        while (!loginStatus)
        {
        loginLabel:
            while (passengerChoice < 1 || passengerChoice > 2)
            {
                cout << "Passenger Menu:" << endl;
                cout << "1. Login" << endl;
                cout << "2. Sign up" << endl;
                cout << "Enter your choice: " << endl;
                cin >> passengerChoice;
                if (passengerChoice < 1 || passengerChoice > 2)
                {
                    cout << "Invalid input. Please enter a correct number." << endl;
                }
            }

            cnic = user.getCnic();
            password = user.getPassword();
            Passenger passenger1(cnic, password);

            if (passengerChoice == 1)
            {
                loginStatus = passenger1.login(passenger1.passengers, passenger1.num_passengers);
            }
            else if (passengerChoice == 2)
            {
                int visa;
                string status;
                cout << "is your visa stamped?\n";
                cout << "1. yes" << endl;
                cout << "2. no" << endl;
                if (visa == 1)
                {
                    status = "stamped";
                }
                else
                {
                    status = "unstamped";
                }
                loginStatus = passenger1.registerUser(status);
            }
        }
        if (loginStatus)
        {
        start:

            int loginChoice = 0;
            cout << "\nPassenger Menu:" << endl;
            cout << "1. BOOK A FLIGHT" << endl;
            cout << "2. CANCEL FLIGHT" << endl;
            cout << "3. CHANGE SCHEDULE" << endl;
            cout << "4. UPDATE DETAILS " << endl;
            cout << "Enter your choice: " << endl;
            cin >> loginChoice;
            Passenger passenger(cnic, password);

            if (loginChoice == 1)
            {
                int flightNumber;
                int seatsLeft;
                int bookSeats = 0;
                numberOfFlights = departureDetails();
                printAvailableFlights(numberOfFlights);
                cout << "\n ENTER YOUR FLIGHT YOU WANT TO BOOK" << endl;
                cin >> flightNumber;
                flightNumber = flightNumber - 1;
                seatsLeft = passenger.checkSeats(flightNumber);

                cout << "ENTER NUMBER OF SEATS YOU WANT TO BOOK" << endl;
                cin >> bookSeats;
                while (bookSeats > seatsLeft)
                {
                    cout << "ONLY " << seatsLeft << " are left!!!! ENTER ACCORDINGLY" << endl;
                    cout << "BUT DUE TO COVID WE HAVE ONLY " << seatsLeft / 2 << " left!!!! ENTER ACCORDINGLY" << endl;
                    cin >> bookSeats;
                }

                if (seatsLeft > 0 && (seatsLeft / 2) >= bookSeats)
                {
                    passenger.bookFlight(flightNumber, bookSeats);
                    goto start;
                }
            }
            else if (loginChoice == 2)
            {
                int cancellation = 0;
                int totalFlightsBooked = 0;
                int flightCancelOption = 0;
                cout << "ARE YOU SURE YOU WANT TO CANCEL FLIGHT\n 1. YES\n 2. NO" << endl;
                cin >> cancellation;
                if (cancellation == 1)
                {
                    totalFlightsBooked = passenger.totalBookedFlights();
                    cout << "which flight do you want to cancel" << endl;
                    cin >> flightCancelOption;
                    flightCancelOption = flightCancelOption - 1;
                    passenger.cancelFlight(totalFlightsBooked, flightCancelOption);
                    goto start;
                }
                else
                {
                    goto start;
                }
            }

            else if (loginChoice == 3)
            {
                cout << "ok";
                int totalFlightsBooked = passenger.totalBookedFlights();
                cout << "which flight do you want to update" << endl;
                int flightUpdateOption;
                cin >> flightUpdateOption;
                flightUpdateOption = flightUpdateOption - 1;
                passenger.updateFlight(totalFlightsBooked, flightUpdateOption);
                goto start;
            }
            else if (loginChoice == 4)
            {
                passenger.update_user();
                //     int presenceStatus = 0;
                //     cout << "what do you want to update" << endl;
                //     presenceStatus = passenger.checkUser();
                //     cout << presenceStatus;
                //     if (presenceStatus >= 0)
                //     {
                //         int updateChoice = 0;
                //         string updatedCnic;
                //         string updatedPassword;
                //         string updatedVisa;
                //         cout << "Enter the number of the field you want to update: " << endl;
                //         cout << "1. CNIC" << endl;
                //         cout << "2. PASSWORD" << endl;
                //         cout << "3. VISA STATUS" << endl;
                //         cin >> updateChoice;
                //
                //             passenger.updateData(presenceStatus, updateChoice, updatedVisa);
                //         }
                //         cout << "After update you will have to login again";
                //         loginStatus = false;
                //
                //     }
                // goto loginLabel;
                goto start;
            }
        }
    }
    return 0;
}
