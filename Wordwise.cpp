#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

class User {
private:
    string username;
    string password;
    int quizScore;
    int flashcardsReviewed;
public:
    User() : quizScore(0), flashcardsReviewed(0) {}

    void registerUser() {
        cout << "\n--- Register ---\n";
        cout << "Choose a username: ";
        cin >> username;
        cout << "Choose a password: ";
        cin >> password;

        ofstream out("users/" + username + ".txt");
        out << username << "\n" << password << "\n" << quizScore << "\n" << flashcardsReviewed;
        out.close();

        cout << "Registration successful!\n";
    }

    bool loginUser() {
        cout << "\n--- Login ---\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        ifstream in("users/" + username + ".txt");
        if (!in) {
            cout << "User not found!\n";
            return false;
        }

        string storedUser, storedPass;
        in >> storedUser >> storedPass >> quizScore >> flashcardsReviewed;

        if (storedPass == password) {
            cout << "Login successful!\n";
            return true;
        } else {
            cout << "Wrong password!\n";
            return false;
        }
    }

    void saveProgress() {
        ofstream out("users/" + username + ".txt");
        out << username << "\n" << password << "\n" << quizScore << "\n" << flashcardsReviewed;
        out.close();
    }

    void updateQuizScore(int score) {
        quizScore += score;
    }

    void updateFlashcards(int count) {
        flashcardsReviewed += count;
    }

    void showProgress() const {
        cout << "\n--- Your Progress ---\n";
        cout << "Total Quiz Score: " << quizScore << "\n";
        cout << "Flashcards Reviewed: " << flashcardsReviewed << "\n";
    }
};

class Vocabulary {
public:
    static void showWords() {
        cout << "\n--- Vocabulary Lessons ---\n";
        map<string, string> vocab = {
            {"abundant", "existing or available in large quantities"},
            {"benevolent", "well meaning and kindly"},
            {"candid", "truthful and straightforward"},
            {"diligent", "having or showing care in one's work"},
            {"eloquent", "fluent or persuasive in speaking"},
            {"futile", "incapable of producing any useful result"},
            {"gratify", "to give pleasure or satisfaction"},
            {"humility", "a modest view of one's importance"},
            {"invoke", "cite or appeal as an authority"},
            {"keen", "having or showing eagerness or enthusiasm"}
        };

        for (auto &pair : vocab) {
            cout << pair.first << ": " << pair.second << "\n";
        }
    }
};

class Flashcards {
public:
    static void start(User &user) {
        cout << "\n--- Flashcards ---\n";
        map<string, string> flashcards = {
            {"eloquent", "fluent or persuasive in speaking"},
            {"resilient", "able to recover quickly from difficulties"},
            {"zealous", "showing great energy or enthusiasm"},
            {"meticulous", "very careful and precise"},
            {"tenacious", "not giving up easily"}
        };

        int count = 0;
        for (auto &card : flashcards) {
            cout << "\nWord: " << card.first << "\n";
            cout << "Press Enter to see the meaning...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Meaning: " << card.second << "\n";
            count++;
        }

        cout << "\nYou've reviewed " << count << " flashcards.\n";
        user.updateFlashcards(count);
    }
};

class Quiz {
private:
    struct Question {
        string question;
        vector<string> options;
        int answer;
    };

public:
    static void start(User &user) {
        cout << "\n--- Grammar Quiz ---\n";
        vector<Question> questions = {
            {"Choose the correct sentence:", {"He go to school.", "He goes to school.", "He going to school."}, 1},
            {"Which is a noun?", {"run", "happy", "city"}, 2},
            {"Which is a verb?", {"apple", "eat", "blue"}, 1},
            {"Choose the correct article for 'umbrella':", {"a", "an", "the"}, 1},
            {"Identify the adjective:", {"run", "beautiful", "quickly"}, 1},
            {"Which is a pronoun?", {"John", "he", "go"}, 1},
            {"Which sentence is in present continuous?", {"She is running.", "She ran.", "She will run."}, 0},
            {"Choose the past tense of 'go':", {"gone", "went", "goed"}, 1},
            {"Which one is a conjunction?", {"and", "but", "both"}, 0},
            {"Select the correct form: 'They ___ playing.'", {"is", "are", "am"}, 1}
        };

        int score = 0;
        srand(time(0));
        random_shuffle(questions.begin(), questions.end());

        for (int i = 0; i < 5; i++) {
            cout << "\nQ" << i + 1 << ": " << questions[i].question << "\n";
            for (int j = 0; j < questions[i].options.size(); j++) {
                cout << j + 1 << ". " << questions[i].options[j] << "\n";
            }

            int ans;
            cout << "Your answer: ";
            cin >> ans;

            if (ans - 1 == questions[i].answer) {
                cout << "Correct!\n";
                score += 10;
            } else {
                cout << "Wrong! Correct answer is: " << questions[i].options[questions[i].answer] << "\n";
            }
        }

        cout << "\nQuiz Completed. Your score: " << score << "/50\n";
        user.updateQuizScore(score);
    }
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mainMenu(User &user) {
    int choice;
    do {
        cout << "\n--- English Learning App ---\n";
        cout << "1. Vocabulary Lessons\n";
        cout << "2. Flashcards\n";
        cout << "3. Grammar Quiz\n";
        cout << "4. View Progress\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        clearScreen();

        switch (choice) {
            case 1:
                Vocabulary::showWords();
                break;
            case 2:
                cin.ignore();
                Flashcards::start(user);
                break;
            case 3:
                Quiz::start(user);
                break;
            case 4:
                user.showProgress();
                break;
            case 5:
                user.saveProgress();
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 5);
}

int main() {
    User user;
    int loginChoice;

    do {
        cout << "\nWelcome to Basic English Learning App!\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "Enter choice: ";
        cin >> loginChoice;

        if (loginChoice == 1) {
            user.registerUser();
            break;
        } else if (loginChoice == 2) {
            if (user.loginUser()) {
                break;
            }
        } else {
            cout << "Invalid option!\n";
        }
    } while (true);

    mainMenu(user);
    return 0;
}
