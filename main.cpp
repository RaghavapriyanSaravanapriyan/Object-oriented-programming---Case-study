#include <iostream>
#include <string>

using namespace std;

/* 
THE BASE CLASS - THE EMPLOYEE.

attributes are name, id
static members : totalactiveemployees and company budget

a constructor to initialise the name and id and also increment the total 
employees

a destructor to reclaim memory

*/

class Employee {
protected:
    string name;
    int id;
    
    // STATIC MEMBERS: Tracked globally across the whole company
    static int totalActiveEmployees;
    static double companyBudget;

public:
    Employee(string n, int i) : name(n), id(i) {
        totalActiveEmployees++;
    }

    // Virtual destructor for safe dynamic memory cleanup
    virtual ~Employee() {
        totalActiveEmployees--;
    }

    // POLYMORPHISM: Single pure virtual function handles both work and pay
    virtual double simulateMonth() = 0;
    virtual void displayDetails() = 0;

    int getId() const { return id; }
    string getName() const { return name; }

    // Static methods
    static int getTotalActive() { return totalActiveEmployees; }
    static double getBudget() { return companyBudget; }
    static void deductFromBudget(double amount) { companyBudget -= amount; }
};

// Initialize static members
int Employee::totalActiveEmployees = 0;
double Employee::companyBudget = 1000000.00; // $1 Million starting budget

// LEVEL 1: INTERMEDIATE CATEGORIES

class SalariedEmployee : public Employee {
protected:
    double annualSalary;
public:
    SalariedEmployee(string n, int i, double salary) 
        : Employee(n, i), annualSalary(salary) {}

    // Base logic for salaried pay (1 month = Annual / 12)
    virtual double calculateMonthlyPay() {
        return annualSalary / 12.0;
    }
};

class HourlyEmployee : public Employee {
protected:
    double hourlyRate;
    int hoursWorked;
public:
    HourlyEmployee(string n, int i, double rate) 
        : Employee(n, i), hourlyRate(rate), hoursWorked(0) {}

    // Base logic for hourly pay (Assumes standard 160-hour month)
    virtual double calculateMonthlyPay() {
        hoursWorked += 160; 
        double pay = hoursWorked * hourlyRate;
        hoursWorked = 0; // Reset for next month
        return pay;
    }
};


// LEVEL 2: PREDEFINED SPECIALIZED ROLES

class SeniorEngineer : public SalariedEmployee {
public:
    SeniorEngineer(string n, int i) : SalariedEmployee(n, i, 250000.0) {}

    double simulateMonth() override {
        cout << "[WORK] Senior Engineer " << name << " designed system architectures this month.\n";
        return calculateMonthlyPay();
    }

    void displayDetails() override {
        cout << "ID: " << id << " | Name: " << name << " | Role: Senior Engineer | Base: $250k/yr\n";
    }
};

class JuniorEngineer : public SalariedEmployee {
public:
    JuniorEngineer(string n, int i) : SalariedEmployee(n, i, 75000.0) {}

    double simulateMonth() override {
        cout << "[WORK] Junior Engineer " << name << " completed code sprints and training this month.\n";
        return calculateMonthlyPay();
    }

    void displayDetails() override {
        cout << "ID: " << id << " | Name: " << name << " | Role: Junior Engineer | Base: $75k/yr\n";
    }
};

class Manager : public SalariedEmployee {
public:
    Manager(string n, int i) : SalariedEmployee(n, i, 95000.0) {}

    double simulateMonth() override {
        cout << "[WORK] Manager " << name << " handled department operations and planning this month.\n";
        return calculateMonthlyPay();
    }

    void displayDetails() override {
        cout << "ID: " << id << " | Name: " << name << " | Role: Department Manager | Base: $95k/yr\n";
    }
};

class FloorTechnician : public HourlyEmployee {
public:
    FloorTechnician(string n, int i) : HourlyEmployee(n, i, 30.0) {} // $30/hr

    double simulateMonth() override {
        cout << "[WORK] Floor Tech " << name << " logged 160 hours on the assembly floor this month.\n";
        return calculateMonthlyPay();
    }

    void displayDetails() override {
        cout << "ID: " << id << " | Name: " << name << " | Role: Floor Tech | Rate: $" << hourlyRate << "/hr\n";
    }
};

// MAIN FUNCTION - Interactive Dashboard
int main() {
    int maxCapacity = 50;
    
    // DYNAMIC MEMORY: Allocating array of pointers
    Employee** roster = new Employee*[maxCapacity];
    int currentHeadcount = 0;
    int nextId = 1001;
    bool running = true;

    cout << "--- Enterprise Employee & Budget System ---\n";
    cout << "Welcome, HR Administrator.\n";

    while (running) {
        cout << "\n==========================================\n";
        cout << "ACTIVE EMPLOYEES: " << Employee::getTotalActive() << " / " << maxCapacity << "\n";
        cout << "COMPANY BUDGET: $" << Employee::getBudget() << "\n";
        cout << "==========================================\n";
        cout << "1. Hire New Employee\n";
        cout << "2. Simulate 1 Month & Process Auto-Payroll\n";
        cout << "3. Terminate Employee\n";
        cout << "4. System Shutdown (Exit)\n";
        cout << "Select action: ";
        
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (currentHeadcount >= maxCapacity) {
                cout << ">> ERROR: Maximum headcount reached.\n";
                continue;
            }
            
            cout << "\n--- Open Positions ---\n";
            cout << "1. Senior Engineer ($250k/yr)\n";
            cout << "2. Junior Engineer ($75k/yr)\n";
            cout << "3. Department Manager ($95k/yr)\n";
            cout << "4. Floor Technician ($30/hr)\n";
            cout << "Select role to hire: ";
            int roleChoice;
            cin >> roleChoice;

            string name;
            cout << "Enter Employee First Name: ";
            cin >> name;
            
            // DYNAMIC MEMORY: Instantiating specific subclasses at runtime
            if (roleChoice == 1) roster[currentHeadcount] = new SeniorEngineer(name, nextId);
            else if (roleChoice == 2) roster[currentHeadcount] = new JuniorEngineer(name, nextId);
            else if (roleChoice == 3) roster[currentHeadcount] = new Manager(name, nextId);
            else if (roleChoice == 4) roster[currentHeadcount] = new FloorTechnician(name, nextId);
            else { cout << ">> Invalid role.\n"; continue; }

            cout << ">> SUCCESS: " << name << " hired successfully. ID assigned: " << nextId << "\n";
            currentHeadcount++;
            nextId++;
        } 
        else if (choice == 2) {
            double totalPayout = 0;
            cout << "\n--- End of Month: Activity & Payroll Report ---\n";
            
            for (int i = 0; i < currentHeadcount; i++) {
                if (roster[i] != nullptr) {
                    roster[i]->displayDetails();
                    
                    // polymorphism : This one call prints the specific work done AND calculates the correct pay
                    double pay = roster[i]->simulateMonth(); 
                    
                    cout << "   -> Monthly Salary Credited: $" << pay << "\n\n";
                    totalPayout += pay;
                }
            }
            
            // Deduct the total payroll from the shared static budget
            Employee::deductFromBudget(totalPayout);
            
            cout << ">> TOTAL MONTHLY PAYROLL DEDUCTED: $" << totalPayout << "\n";
            if (Employee::getBudget() < 0) {
                cout << ">> CRITICAL WARNING: COMPANY BUDGET IS NEGATIVE.\n";
            }
        } 
        else if (choice == 3) {
            int fireId;
            cout << "Enter Employee ID to terminate: ";
            cin >> fireId;
            bool found = false;
            
            for (int i = 0; i < currentHeadcount; i++) {
                if (roster[i] != nullptr && roster[i]->getId() == fireId) {
                    cout << ">> Terminating " << roster[i]->getName() << "...\n";
                    
                    // DYNAMIC MEMORY: Freeing memory for a specific object
                    delete roster[i]; 
                    
                    // Shift array to avoid gaps
                    for (int j = i; j < currentHeadcount - 1; j++) {
                        roster[j] = roster[j + 1];
                    }
                    roster[currentHeadcount - 1] = nullptr;
                    currentHeadcount--;
                    found = true;
                    break;
                }
            }
            if (!found) cout << ">> ERROR: Employee ID not found.\n";
        } 
        else if (choice == 4) {
            running = false;
        }
    }

    // DYNAMIC MEMORY: Complete cleanup to prevent memory leaks
    cout << "\nInitiating system shutdown. Freeing memory...\n";
    for (int i = 0; i < currentHeadcount; i++) {
        if (roster[i] != nullptr) {
            delete roster[i];
        }
    }
    delete[] roster;
    cout << "Memory cleared. System offline.\n";

    return 0;
}