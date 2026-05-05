#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// Structures for Members, Tasks, and Notes

struct Member {
    int id;
    string name;
    string role;
};

struct Task {
    int id;
    int member_id;
    string task;
    string status;
};

struct Note {
    int id;
    string type;
    string title;
    string body;
    string date;
};

// Three vectors store all members, tasks, and notes, like an array but dynamic

vector<Member> members;
vector<Task> tasks;
vector<Note> notes;

// It keeps track of the next unique ID to assign

int next_id = 1;

// .txt files used to save data

string members_file = "members.txt";
string tasks_file = "tasks.txt";
string notes_file = "notes.txt";

// ===== Extra Functions =====
// Gives a unique ID for new members, tasks, and notes

int get_next_id() {
    int current_id = next_id;
    next_id++;
    return current_id;
}

// Returns points based on the status of a task

int get_points(string status) {
    if (status == "Complete") {
        return 3;
    } else if (status == "In Progress") {
        return 1;
    } else if (status == "Incomplete") {
        return 0;
    }
    return 0;
}

// Splits a string into parts using a seperator character and returns a vector of the parts

vector<string> split_string(string line, char separator) {
    vector<string> parts;
    string part = "";

    for (int i = 0; i < line.size(); i++) {
        if (line[i] == separator) {
            parts.push_back(part);
            part = "";
        } else {
            part += line[i];
        }
    }

    parts.push_back(part);
    return parts;
}

// A function that removes spaces and newline characters from the beginning and end of a string and returns the cleaned string

string strip(string str) {
    while (!str.empty() && (str[0] == ' ' || str[0] == '\n' || str[0] == '\r')) {
        str.erase(0, 1);
    }

    while (!str.empty() && (str.back() == ' ' || str.back() == '\n' || str.back() == '\r')) {
        str.pop_back();
    }
    return str;
}

// A function that gets today's date and returns it as string

string get_current_date() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%m-%d-%Y", ltm);

    return string(buffer);
}

// A function that searches for a member by their ID by looping through the member vector, and returns a pointer to a specific member if found, otherwise return nullptr if not found

Member* get_member(int member_id) {
    for (int i = 0; i < members.size(); i++) {
        if (members[i].id == member_id) {
            return &members[i];
        }
    }
    return nullptr;
}

// A function that calculate the total contribution score for a member by looping through all tasks, finding one that belongs to a specific member, and adding up points based on the tasks' statuses, before returning the total points

int calculate_points(int member_id) {
    int total_points = 0;

    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].member_id == member_id) {
            total_points += get_points(tasks[i].status);
        }
    }

    return total_points;
}

// A function that shows a numbered list of options to the user before asking for an input, and will keep on looping until the number being input in is valid, which will be returned as a string or no empty string if none of the options are available

string pick_from_list(string prompt, vector<string> options) {
    if (options.empty()) {
        cout << "No options available." << endl;
        return "";
    }

    for (int i = 0; i < options.size(); i++) {
        cout << i + 1 << ". " << options[i] << endl;
    }

    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        input = strip(input);

        bool is_number = true;

        if (input == "") {
            is_number = false;
        }

        for (int i = 0; i < input.size(); i++){
            if (input[i] < '0' || input [i] > '9') {
                is_number = false;
            }
        }

        if (is_number) {
            int choice = stoi(input);
            if (choice >= 1 && choice <= options.size()) {
                return options[choice - 1];
            }
        }

        cout << "Invalid number. Please try again." << endl;
    }
}

// Saves the members, tasks, and notes data to their respective .txt files

void save_data() {
    ofstream f_members(members_file);
    for (int i = 0; i < members.size(); i++) {
        f_members << members[i].id << "|" << members[i].name << "|" << members[i].role << endl;
    }
    f_members.close();

    ofstream f_tasks(tasks_file);
    for (int i = 0; i < tasks.size(); i++) {
        f_tasks << tasks[i].id << "|" << tasks[i].member_id << "|" << tasks[i].task << "|" << tasks[i].status << endl;
    }
    f_tasks.close();

    ofstream f_notes(notes_file);
    for (int i = 0; i < notes.size(); i++) {
        f_notes << notes[i].id << "|" << notes[i].type << "|" << notes[i].title << "|" << notes[i].body << "|" << notes[i].date << endl;
    }
    f_notes.close();
}

// Loads the members, tasks, and notes data from their respective .txt files

void load_data() {
    members.clear();
    tasks.clear();
    notes.clear();

    string line;

    ifstream f_members(members_file);
    if (f_members.is_open()) {
        while (getline(f_members, line)) {
            line = strip(line);

            if (line == "") continue;

            vector<string> parts = split_string(line, '|');

            if (parts.size() >= 3) {
                Member member;
                member.id = stoi(parts[0]);
                member.name = parts[1];
                member.role = parts[2];

                members.push_back(member);
            }
        }
        f_members.close();
    }

    ifstream f_tasks(tasks_file);
    if (f_tasks.is_open()) {
        while (getline(f_tasks, line)) {
            line = strip(line);

            if (line == "") continue;

            vector<string> parts = split_string(line, '|');

            if (parts.size() >= 4) {
                Task task;
                task.id = stoi(parts[0]);
                task.member_id = stoi(parts[1]);
                task.task = parts[2];
                task.status = parts[3];

                tasks.push_back(task);
            }
        }
        f_tasks.close();
    }

    ifstream f_notes(notes_file);
    if (f_notes.is_open()) {
        while (getline(f_notes, line)) {
            line = strip(line);

            if (line == "") continue;

            vector<string> parts = split_string(line, '|');

            if (parts.size() >= 5) {
                Note note;
                note.id = stoi(parts[0]);
                note.type = parts[1];
                note.title = parts[2];
                note.body = parts[3];
                note.date = parts[4];

                notes.push_back(note);
            }
        }
        f_notes.close();
    }

    int max_id = 0;

    for (int i = 0; i < members.size(); i++) 
        if (members[i].id > max_id) max_id = members[i].id;

    for (int i = 0; i < tasks.size(); i++) 
        if (tasks[i].id > max_id) max_id = tasks[i].id;

    for (int i = 0; i < notes.size(); i++) 
        if (notes[i].id > max_id) max_id = notes[i].id;

    if (max_id > 0) {
        next_id = max_id + 1;
    }
}

// Function Prototypes

void members_tasks_menu();
void notes_menu();
void view_evaluations();

void add_member();
void view_members_and_tasks();
void delete_member();
void add_task();
void update_task_status();
void delete_task();

void add_note();
void view_note();
void delete_note();

// Main Menu 

void main_menu() {
    cout << "=================================================================" << endl;
    cout << "COLLABORATIVE GROUP CONTRIBUTION MONITORING AND EVALUATION SYSTEM" << endl;
    cout << "=================================================================" << endl;
    cout << "Welcome to the Collaborative Group Contribution Monitoring and Evaluation System!" << endl;
    cout << "This program allows you to manage group members, assign tasks, set notes and reminders, and evaluate contributions." << endl;
    
    while (true) {
       cout << "\nMain Menu" << endl;
       cout << "1. Members and Tasks" << endl;
       cout << "2. Notes and Reminders" << endl;
       cout << "3. Evaluation and Rankings" << endl;
       cout << "4. Exit" << endl;

       cout << "Please select an option (1-4): ";
       string choice;
       getline(cin, choice);
       choice = strip(choice);

         if (choice == "1") {
              members_tasks_menu();
         } else if (choice == "2") {
              notes_menu();
         } else if (choice == "3") {
              view_evaluations();
         } else if (choice == "4") {
              cout << "Exiting the program." << endl;
              break;
         } else {
              cout << "Invalid number. Please try again." << endl;
         }
    }
}

// Members and Tasks Menu 

void members_tasks_menu() {
    while (true) {
       cout << "\nMembers and Tasks" << endl;
       cout << "1. Add Member" << endl;
       cout << "2. View Members and Tasks" << endl;
       cout << "3. Delete Member" << endl;
       cout << "4. Add Task" << endl;
       cout << "5. Update Task Status" << endl;
       cout << "6. Delete Task" << endl;
       cout << "7. Back to Main Menu" << endl;

       cout << "Please select an option (1-7): ";
       string choice;
       getline(cin, choice);
       choice = strip(choice);

       if (choice == "1") {
           add_member();
       } else if (choice == "2") {
           view_members_and_tasks();
       } else if (choice == "3") {
           delete_member();
       } else if (choice == "4") {
           add_task();
       } else if (choice == "5") {
           update_task_status();
       } else if (choice == "6") {
           delete_task();
       } else if (choice == "7") {
           break;
       } else {
           cout << "Invalid number. Please try again." << endl;
       }
    }
}

// Notes and Reminders Menu 

void notes_menu() {
    while (true) {
       cout << "\nNotes and Reminders" << endl;
       cout << "1. Add Note/Reminder" << endl;
       cout << "2. View Notes" << endl;
       cout << "3. Delete Note" << endl;
       cout << "4. Back to Main Menu" << endl;

       cout << "Please select an option (1-4): ";
       string choice;
       getline(cin, choice);
       choice = strip(choice);

       if (choice == "1") {
           add_note();
       } else if (choice == "2") {
           view_note();
       } else if (choice == "3") {
           delete_note();
       } else if (choice == "4") {
           break;
       } else {
           cout << "Invalid number. Please try again." << endl;
       }
    }
}

// Evaluation and Rankings Menu 

void view_evaluations() {
    cout << "\nEvaluation and Rankings" << endl;
    
    int total_tasks = tasks.size();
    int done_tasks = 0;

    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].status == "Complete") {
            done_tasks++;
        }
    }

    string rate = "0%";
    if (total_tasks > 0) {
        int percentage = (done_tasks * 100) / total_tasks;
        rate = to_string(percentage) + "%";
    }

    int total_points = 0;
    for (int i = 0; i < members.size(); i++) {
        total_points += calculate_points(members[i].id);
    }

    cout << "\nMembers: " << members.size() << endl;
    cout << "Total Tasks: " << total_tasks << endl;
    cout << "Tasks Completed: " << done_tasks << endl;
    cout << "Overall Completion Rate: " << rate << endl;
    cout << "Total Points: " << total_points << endl;

    if (members.empty()) {
        cout << "No members to evaluate." << endl;
        return;
    }

    vector<Member> ranked_members = members;

    for (int i = 0; i < ranked_members.size(); i++) {
        for (int j = i + 1; j < ranked_members.size(); j++) {
            if (calculate_points(ranked_members[j].id) > calculate_points(ranked_members[i].id)) {
                Member temp = ranked_members[i];
                ranked_members[i] = ranked_members[j];
                ranked_members[j] = temp;
            }
        }
    }
    
    cout << "\nMember Rankings:" << endl;
    for (int i = 0; i < ranked_members.size(); i++) {
        int score = calculate_points(ranked_members[i].id);

        int complete_tasks = 0;
        int in_progress_tasks = 0;
        int incomplete_tasks = 0;

        for (int j = 0; j < tasks.size(); j++) {
            if (tasks[j].member_id == ranked_members[i].id) {
                if (tasks[j].status == "Complete") {
                    complete_tasks++;
                } else if (tasks[j].status == "In Progress") {
                    in_progress_tasks++;
                } else if (tasks[j].status == "Incomplete") {
                    incomplete_tasks++;
                }
            }
        }

        string role_text = "";
        if (ranked_members[i].role != "") {
            role_text = " (" + ranked_members[i].role + ")";
        }

        cout << i + 1 << " - " << ranked_members[i].name << role_text << " - Points: " << score << endl;
        cout << "Completed Tasks: " << complete_tasks << endl;
        cout << "In Progress Tasks: " << in_progress_tasks << endl;
        cout << "Incomplete Tasks: " << incomplete_tasks << endl;
    }

    cout << "\nScore: Complete = 3 points | In Progress = 1 point | Incomplete = 0 points" << endl;
}

// ===== Members and Tasks Menu Functions =====
// A function that adds a new member to the members vector and saves the data

void add_member() {
    cout << "\nAdd Member" << endl;
    cout << "Enter member name: ";
    string name;
    getline(cin, name);
    name = strip(name);

    if (name == "") {
        cout << "Member name cannot be empty." << endl;
        return;
    }

    cout << "Enter member role: ";
    string role;
    getline(cin, role);
    role = strip(role);

    if (role == "") {
        cout << "Member role cannot be empty." << endl;
        return;
    }

    Member member;
    member.id = get_next_id();
    member.name = name;
    member.role = role;

    members.push_back(member);

    save_data();

    cout << "Member added successfully!" << endl;
    return;
}

// A function that allows users to view a list of members and tasks

void view_members_and_tasks() {
    cout << "\nMembers and Tasks List" << endl;

    if (members.empty()){
        cout << "No members added yet." << endl;
    }
    
    for (int i = 0; i < members.size(); i++) {
        string role_text = "";
        if (members[i].role != "") {
            role_text = " (" + members[i].role + ")";
        }

        cout << "\n[" << members[i].id << "] " << members[i].name << role_text << endl;

        bool has_tasks = false;
        for (int j = 0; j < tasks.size(); j++) {
            if (tasks[j].member_id == members[i].id) {
                cout << " - " << tasks[j].task << endl;
                cout << " Status: " << tasks[j].status << endl;
                has_tasks = true;
            }
        }

        if (!has_tasks) {
            cout << "No tasks assigned yet." << endl;
        }
    }
}

// A function the allows users to delete an existing member along with their tasks

void delete_member() {
    cout << "\nDelete Member" << endl;

    if (members.empty()) {
        cout << "No members to delete." << endl;
        return;
    }

    vector<string> member_names;
    for (int i = 0; i < members.size(); i++) {
        member_names.push_back(to_string(members[i].id) + " - " + members[i].name);
    }

    cout << "Select a member to delete:" << endl;
    string chosen = pick_from_list("Choice: ", member_names);
    if (chosen == "") return;

    int member_id = stoi(strip(split_string(chosen, '-')[0]));
    Member* member = get_member(member_id);

    cout << "Are you sure you want to delete '" << member->name << "' and all their tasks? (yes/no): ";
    string confirm;
    getline(cin, confirm);
    confirm = strip(confirm);

    if (confirm != "yes") {
        cout << "Deletion cancelled." << endl;
        return;
    }

    vector<Member> new_members;
    for (int i = 0; i < members.size(); i++) {
        if (members[i].id != member_id) {
            new_members.push_back(members[i]);
        }
    }
    members = new_members;

    vector<Task> new_tasks;
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].member_id != member_id) {
            new_tasks.push_back(tasks[i]);
        }
    }
    tasks = new_tasks;

    save_data();
    cout << "Member and their tasks deleted successfully!" << endl;
}

// A function that allows users to add a task along with the description and status on it

void add_task() {
    cout << "\nAdd New Task" << endl;

    if (members.empty()) {
        cout << "No members available. Please add a member first." << endl;
        return;
    }

    vector<string> member_names;
    for (int i = 0; i < members.size(); i++) {
        member_names.push_back(to_string(members[i].id) + " - " + members[i].name);
    }

    cout << "Select a member to assign the task to:" << endl;
    string chosen = pick_from_list("Choice: ", member_names);
    if (chosen == "") return;

    int member_id = stoi(strip(split_string(chosen, '-')[0]));

    if (get_member(member_id) == nullptr) {
        cout << "Error: Selected member does not exist." << endl;
        return;
    }

    cout << "Enter task description: " << endl;
    string task_desc;
    getline(cin, task_desc);
    task_desc = strip(task_desc);

    if (task_desc == "") {
        cout << "Error: Please enter a task description first." << endl;
        return;
    }

    vector<string> status_list = {"Incomplete", "In Progress", "Complete"};
    cout << "Select task status:" << endl;
    string chosen_status = pick_from_list("Choice: ", status_list);
    if (chosen_status == "") return;

    Task task;
    task.id = get_next_id();
    task.member_id = member_id;
    task.task = task_desc;
    task.status = chosen_status;

    tasks.push_back(task);

    save_data();
    cout << "Task added successfully!" << endl;
}

// A function that allows users to update the status of the task that was selected

void update_task_status() {
    cout << "\nUpdate Task Status" << endl;
    
    if (tasks.empty()) {
        cout << "No tasks available to update." << endl;
        return;
    }

    vector<string> task_labels;
    for (int i = 0; i < tasks.size(); i++) {
        Member* member = get_member(tasks[i].member_id);
        string member_name = (member != nullptr) ? member->name : "Unknown";
        task_labels.push_back(member_name + " - " + tasks[i].task + " [" + tasks[i].status + "]");
    }

    cout << "Select a task to update:" << endl;
    string chosen_label = pick_from_list("Choice: ", task_labels);
    if (chosen_label == "") return;

    int chosen_index = -1;
    for (int i = 0; i < task_labels.size(); i++) {
        if (task_labels[i] == chosen_label) {
            chosen_index = i;
            break;
        }
    }

    if (chosen_index == -1) {
        cout << "Error: Invalid selection." << endl;
        return;
    }

    cout << "Current Status: " << tasks[chosen_index].status << endl;

    vector<string> status_list = {"Incomplete", "In Progress", "Complete"};
    cout << "Select new status:" << endl;
    string new_status = pick_from_list("Choice: ", status_list);
    if (new_status == "") return;

    tasks[chosen_index].status = new_status;

    save_data();
    cout << "Task status updated successfully!" << endl;
}

// A function that allows users to select a task and delete it

void delete_task() {
    cout << "\nDelete Task" << endl;

    if (tasks.empty()) {
        cout << "No tasks available to delete." << endl;
        return;
    }

    vector<string> task_labels;
    for (int i = 0; i < tasks.size(); i++) {
        Member* member = get_member(tasks[i].member_id);
        string member_name = (member != nullptr) ? member->name : "Unknown";
        task_labels.push_back(member_name + " - " + tasks[i].task + " [" + tasks[i].status + "]");
    }

    cout << "Select a task to delete:" << endl;
    string chosen_label = pick_from_list("Choice: ", task_labels);
    if (chosen_label == "") return;

    int chosen_index = -1;
    for (int i = 0; i < tasks.size(); i++) {
        if (task_labels[i] == chosen_label) {
            chosen_index = i;
            break;
        }
    }

    if (chosen_index == -1) {
        cout << "Error: Invalid selection." << endl;
        return;
    }

    int task_id = tasks[chosen_index].id;

    vector<Task> new_tasks;
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].id != task_id) {
            new_tasks.push_back(tasks[i]);
        }
    }
    tasks = new_tasks;

    save_data();
    cout << "Task deleted successfully!" << endl;
}

// A function that adds a new note to the notes list

void add_note() {
    cout << "\nAdd New Note/Reminder" << endl;

    cout << "Enter note title:" << endl;
    string title;
    getline(cin, title);
    title = strip(title);

    vector<string> note_types = {"Quick Note", "Reminder", "Urgent"};
    cout << "Select note type:" << endl;
    string chosen_type = pick_from_list("Choice: ", note_types);
    if (chosen_type == "") return;

    cout << "Enter content: " << endl;
    string body;
    getline(cin, body);
    body = strip(body);

    if (title == "" && body == "") {
        cout << "Error: Please enter a title or content." << endl;
        return;
    }

    Note note;
    note.id = get_next_id();
    note.type = chosen_type;
    note.title = (title == "") ? "(untitled)" : title;
    note.body = body;
    note.date = get_current_date();

    notes.push_back(note);

    save_data();
    cout << "Note saved successfully!" << endl;
}

// A function that lets users view all notes and reminders, starting from the most recent date

void view_note() {
    cout << "\nNotes and Reminders" << endl;

    if (notes.empty()) {
        cout << "No notes or reminders added yet." << endl;
        return;
    }

    for (int i = notes.size() - 1; i >= 0; i--) {
        cout << "\n[" << notes[i].type << "] " << notes[i].title << endl;
        if (notes[i].body != "") {
            cout << notes[i].body << endl;
        }
        cout << "Created on: " << notes[i].date << endl;
    }
}

// A function that lets users delete a note

void delete_note() {
    cout << "\nDelete Note" << endl;

    if (notes.empty()) {
        cout << "No notes to delete." << endl;
        return;
    }

    vector<string> note_labels;
    for (int i = 0; i <notes.size(); i++) {
        note_labels.push_back("[" + notes[i].type + "] " + notes[i].title);
    }

    cout << "Select a note to delete:" << endl;
    string chosen_label = pick_from_list("Choice: ", note_labels);
    if (chosen_label == "") return;

    int chosen_index = -1;
    for (int i = 0; i < note_labels.size(); i++) {
        if (note_labels[i] == chosen_label) {
            chosen_index = i;
            break;
        }
    }

    if (chosen_index == -1) {
        cout << "Error: Invalid selection." << endl;
        return;
    }

    int note_id = notes[chosen_index].id;

    vector<Note> new_notes;
    for (int i = 0; i < notes.size(); i++) {
        if (notes[i].id != note_id) {
            new_notes.push_back(notes[i]);
        }
    }
    notes = new_notes;

    save_data();
    cout << "Note deleted successfully!" << endl;
}

// Start of the program

int main() {
    load_data();
    main_menu();

    return 0;
}