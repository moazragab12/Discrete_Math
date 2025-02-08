#include<bits/stdc++.h>
#include <windows.h>

using namespace std;

int precedence(char op) {
    if (op == '~') return 3;
    if (op == '^') return 2;
    if (op == 'v') return 1;
    if (op == '(' || op == ')') return -1;
    return -1;
}


bool isOperator(char c) {
    return c == '~' || c == '^' || c == 'V';
}


// Function to convert an infix expression to a postfix expression
string infixToPostfix(const string &infix) {
    string postfix; // Stores the resulting postfix expression
    stack<char> opStack; // Stack to store operators and parentheses

    // Iterate through each character in the infix expression
    for (char c: infix) {
        // If the character is an alphanumeric character (except 'V'), add it to the postfix string
        if (isalnum(c) && c != 'V') {
            postfix += c;
        }
            // If the character is an opening parenthesis, push it onto the stack
        else if (c == '(') {
            opStack.push(c);
        }
            // If the character is a closing parenthesis, pop and append all operators
            // from the stack until an opening parenthesis is encountered
        else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top(); // Append the operator to the postfix string
                opStack.pop(); // Remove the operator from the stack
            }
            opStack.pop(); // Remove the opening parenthesis from the stack
        }
            // If the character is an operator
        else if (isOperator(c)) {
            // While the top of the stack has an operator with higher or equal precedence,
            // and it is not an opening parenthesis, pop it to the postfix string
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(c) && opStack.top() != '(') {
                postfix += opStack.top(); // Append the operator to the postfix string
                opStack.pop(); // Remove the operator from the stack
            }
            opStack.push(c); // Push the current operator onto the stack
        }
    }

    // Pop all remaining operators from the stack and append them to the postfix string
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }

    return postfix; // Return the resulting postfix expression
}

string convertImplies(const string &expression) {
    string result = "~(";
    int i = 0;
    int size = expression.size();
    while (i < size) {
        if (expression[i] == '>')
            result += ")V(";
        else result += expression[i];
        i++;
    }
    result += ")";

    return result;
}


bool evaluate(string expr, vector<int> &row, unordered_map<char, int> &operands_index, int indINTruthTable) {
    stack<bool> st; // Stack to store intermediate boolean values during evaluation

    // Iterate through each character in the postfix expression
    for (char c: expr) {
        if ((c >= 'a' && c <= 'z')) { // If the character is an operand (a variable)
            st.push(row[operands_index[c]]); // Push the corresponding truth value onto the stack
        } else if (c == '~') { // If the character is the NOT operator
            bool val = st.top();
            st.pop();
            st.push(!val);       // Push the negated value onto the stack
        } else if (c == '^') { // If the character is the AND operator
            bool val1 = st.top(); // Get the first operand from the stack
            st.pop();
            bool val2 = st.top(); // Get the second operand from the stack
            st.pop();
            st.push(val1 && val2); // Push the result of the AND operation onto the stack
        } else if (c == 'V') { // If the character is the OR operator
            bool val1 = st.top(); // Get the first operand from the stack
            st.pop();
            bool val2 = st.top(); // Get the second operand from the stack
            st.pop();
            st.push(val1 || val2); // Push the result of the OR operation onto the stack
        }
    }

    // Store the result of the expression evaluation in the specified truth table index
    row[indINTruthTable] = st.top();
    return st.top(); // Return the final result of the expression evaluation
}

//used to set the color of the output
void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

//converts the biconditional to logical form
string convertbiconditional(string &expression) {
    string result1 = "", result2 = "", ans, tmp1, tmp2;
    int i = 0, first = 1;
    int size = expression.size();
    while (i < size) {
        if (expression[i] == '<') {
            first = 0;

        } else if (first) result1 += expression[i];
        else {
            result2 += expression[i];
        }
        i++;
    }
    result1 = "(" + result1 + ")";
    result2 = "(" + result2 + ")";
    tmp1 = result1 + ">" + result2;
    tmp2 = result2 + ">" + result1;
    tmp1 = convertImplies(tmp1);
    tmp2 = convertImplies(tmp2);
    ans = "(" + tmp1 + ")^(" + tmp2 + ")";
    return ans;
}

void solve() {
    int number_of_premises;
    cout << "Enter the number of premises: ";
    cin >> number_of_premises;

    vector<string> premises(number_of_premises + 1);
    cout
            << "Enter the premises each one with no spaces and every premise in a single line and each sentence with small letter refers to it: \n";
    cout << "> is for implies , ^ is for and , V is for or , <  is for Biconditional, ~ is for not \n";
    vector<string> printing;
    for (int i = 0; i < number_of_premises; i++) {
        cin >> premises[i];
        printing.push_back(premises[i]);
        for (auto c: premises[i]) {
            if (c == '>') {
                premises[i] = convertImplies(premises[i]);
                break;
            } else if (c == '<') {
                premises[i] = convertbiconditional(premises[i]);
            }
        }
        premises[i] = infixToPostfix(premises[i]);
    }


    cout << "Enter the conclusion: ";
    cin >> premises[number_of_premises];
    // Save the original premise for later printing
    printing.push_back(premises[number_of_premises]);

    // Check for implications (">") in the premise and convert them
    for (auto c: premises[number_of_premises]) {
        if (c == '>') {
            premises[number_of_premises] = convertImplies(
                    premises[number_of_premises]); // Convert implications to equivalent logical form
            break;
        } else if (c == '<') {
            premises[number_of_premises] = convertbiconditional(premises[number_of_premises]);
            break;
        }
    }

    // Convert the modified premise from infix notation to postfix notation
    premises[number_of_premises] = infixToPostfix(premises[number_of_premises]);

    // Extract all unique operands (propositions) from all premises
    set<char> operands_set;
    for (const string &premise: premises) {
        for (char ch: premise) {
            if (isalpha(ch) && ch != 'V') { // If the character is a letter and not 'V' (logical OR)
                operands_set.insert(ch);   // Add it to the set of operands
            }
        }
    }

    // Assign indices to each unique operand
    unordered_map<char, int> operands_index;
    int idx = 0;
    for (char ch: operands_set) {
        operands_index[ch] = idx++; // Map each operand to a unique index
    }


    // Create the truth table
    int num_rows = 1 << operands_set.size(); // Number of rows in the truth table is 2^(number of operands)
    vector<vector<int>> truth_table(num_rows, vector<int>(operands_set.size() + number_of_premises + 1, 0));

    // Populate the truth table with all possible combinations of truth values for the operands
    for (int i = 0; i < operands_set.size(); i++) {
        int toggle = 1 << i; // Toggle interval for this operand
        int value = 0;       // Current truth value (0 or 1)
        for (int j = 0; j < num_rows; j++) {
            truth_table[j][i] = value; // Assign the truth value to the operand's column
            if ((j + 1) % toggle == 0) {
                value ^= 1; // Flip the truth value after every 'toggle' rows
            }
        }
    }

    // Initialize variables to track satisfiability and validity
    bool is_satisfiable = false; // True if at least one row satisfies all premises
    bool is_valid = true;       // True if all rows satisfy the premises

    // Evaluate each row of the truth table
    for (int row = 0; row < num_rows; row++) {
        vector<int> &current_row = truth_table[row];
        bool all_premises_true = true; // Flag to check if all premises are true for this row

        int tmpind = operands_index.size(); // Starting index for premise columns in the truth table
        for (const string &premise: premises) {
            // Evaluate the current premise and store the result in the truth table
            if (!evaluate(premise, current_row, operands_index, tmpind)) {
                all_premises_true = false; // If any premise is false, set the flag to false
                is_valid = false;         // The statement is not valid if any premise is false in any row
            }
            tmpind++; // Move to the next column for the next premise
        }

        // Check satisfiability: if all premises are true in this row
        if (all_premises_true) {
            is_satisfiable = true; // At least one row satisfies all premises
        } else {
            is_valid = false; // If not all premises are true in any row, it's not valid
        }
    }
/*********************printing the output**************************/
    cout << "operands : ";
    for (auto c: operands_set) {
        cout << c << "\t";
    }
    for (int i = 0; i < number_of_premises + 1; i++) {
        cout << printing[i] << "\t";
    }
    cout << endl;
    for (int i = 0; i < operands_set.size() + number_of_premises + 5; i++) {
        cout << "_______";
    }
    cout << endl;
    cout << "col\t  : ";
    for (int i = 0; i < operands_set.size() + number_of_premises + 1; i++) {
        cout << i + 1 << "\t";
    }
    cout << endl;
    for (int i = 0; i < truth_table.size(); i++) {
        SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << "row\t " << i + 1 << ": ";
        for (int j = 0; j < truth_table[0].size(); j++) {
            if (truth_table[i][j] == 1) {
                SetColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            } else {
                SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            }
            cout << truth_table[i][j] << "\t";
        }
        cout << endl;
    }
    SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    for (int i = 0; i < operands_set.size() + number_of_premises + 5; i++) {
        cout << "_______";
    }
    cout << endl;
    if (is_valid) {
        cout << "The argument is valid.\n";
    } else {
        cout << "The argument is invalid.\n";
    }

    if (is_satisfiable) {
        cout << "The premises are satisfiable.\n";
    } else {
        cout << "The premises are not satisfiable.\n";
    }
}

//entry point of the program
signed main() {

    int t = 1;
    while (t--) solve();
    return 0;
}
