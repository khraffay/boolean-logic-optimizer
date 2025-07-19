#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;

//Main class
class QM
{
public:
    int VARIABLES;
    string dontcares;

    QM(int a)
    {
        VARIABLES = a;
        dontcares.append(a, '-');
    }

    //function to get the boolean term letters
    vector<string> getVars()
    {
        vector<string> v;
        string letters[] = { "a","b","c","d", "e" };
        for (int i = 0; i < this->VARIABLES; i++)
            v.push_back(letters[i]);

        return v;
    }

    /*function to convert decimal to binary
    * Eg: 14 = 1110
    */
    string decToBin(int n)
    {
        if (n == 0)
            return "0";

        string bin = "";
        while (n > 0) {
            bin = (n % 2 == 0 ? "0" : "1") + bin;
            n /= 2;
        }
        return bin;
    }

    //function to pad zeros to the binary equivalent of digits.
    //Eg: If there are 5 variables, 2, that is 10 in binary is represented as 00010
    string pad(string bin)
    {
        int max = VARIABLES - bin.length();
        for (int i = 0; i < max; i++)
            bin = "0" + bin;
        return bin;
    }

    //function to check if two terms differ by just one bit
    bool isGreyCode(string a, string b)
    {
        int flag = 0;
        for (int i = 0; i < a.length(); i++)
        {
            if (a[i] != b[i])
                flag++;
        }
        return (flag == 1);
    }

    //function to replace complement terms with don't cares
    // Eg: 0110 and 0111 becomes 011-
    string replace_complements(string a, string b)
    {
        string temp = "";
        for (int i = 0; i < a.length(); i++)
            if (a[i] != b[i])
                temp = temp + "-";
            else
                temp = temp + a[i];

        return temp;
    }

    //function to check if string b exists in vector a
    bool in_vector(vector<string> a, string b)
    {
        for (int i = 0; i < a.size(); i++)
            if (a[i].compare(b) == 0)
                return true;
        return false;
    }

    //function to reduce minterms
    vector<string> reduce(vector<string> minterms, int iteration)
    {
        vector<string> newminterms;
        vector<bool> checked(minterms.size(), false);

        cout << "Iteration " << iteration << " reducing minterms: ";
        for (const auto& minterm : minterms)
        {
            cout << minterm << " ";
        }
        cout << endl;

        for (size_t i = 0; i < minterms.size(); i++)
        {
            for (size_t j = i + 1; j < minterms.size(); j++)
            {
                if (isGreyCode(minterms[i], minterms[j]))
                {
                    checked[i] = true;
                    checked[j] = true;
                    string combined = replace_complements(minterms[i], minterms[j]);
                    if (!in_vector(newminterms, combined))
                        newminterms.push_back(combined);

                    cout << "Combining " << minterms[i] << " and " << minterms[j] << " to form " << combined << endl;
                }
            }
        }

        for (size_t i = 0; i < minterms.size(); i++)
        {
            if (!checked[i])
                newminterms.push_back(minterms[i]);
        }

        cout << "New minterms after reduction: ";
        for (const auto& newminterm : newminterms)
        {
            cout << newminterm << " ";
        }
        cout << endl << endl;

        return newminterms;
    }

    //Converting the boolean minterm into the variables
    // Eg: 011- becomes a'bc
    string getValue(string a)
    {
        string temp = "";
        vector<string> vars = this->getVars();
        if (a == dontcares)
            return "1";

        for (int i = 0; i < a.length(); i++)
        {
            if (a[i] != '-')
            {
                if (a[i] == '0')
                    temp = temp + vars[i] + "\'";
                else
                    temp = temp + vars[i];
            }
        }
        return temp;
    }

    //function to check if 2 vectors are equal
    bool VectorsEqual(vector<string> a, vector<string> b)
    {
        if (a.size() != b.size())
            return false;

        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        for (size_t i = 0; i < a.size(); i++)
        {
            if (a[i] != b[i])
                return false;
        }
        return true;
    }

    // function to remove redundant minterms
    vector<string> removeRedundantMinterms(vector<string> minterms)
    {
        vector<string> essentialMinterms;
        vector<string> remainingMinterms = minterms;

        while (!remainingMinterms.empty())
        {
            string minterm = remainingMinterms.back();
            remainingMinterms.pop_back();

            bool isEssential = true;
            for (const auto& essential : essentialMinterms)
            {
                if (replace_complements(minterm, essential) == essential)
                {
                    isEssential = false;
                    break;
                }
            }

            if (isEssential)
            {
                essentialMinterms.push_back(minterm);
            }
        }

        return essentialMinterms;
    }
};

//Main function
int main()
{
    char check = 'y';
    while (check == 'y')
    {
        int no;
        cout << endl << "Enter the number of variables (1-5):" << endl;
        cin >> no;
        if (no > 5 || no < 1)
        {
            cout << "Invalid number of variables (1-5)" << endl;
            continue;
        }
        QM q(no);

        string temp = "";
        cout << "Enter the minterms (RANGE=0-" << pow(2, no) - 1 << ") separated by comma:" << endl;
        cin >> temp;

        //splitting the input
        vector<string> minterms;
        istringstream f(temp);
        string s;
        while (getline(f, s, ','))
        {
            int t = atoi(s.c_str());
            minterms.push_back(q.pad(q.decToBin(t)));
        }

        sort(minterms.begin(), minterms.end());

        vector<string> reduced;
        int iteration = 1;
        do
        {
            reduced = q.reduce(minterms, iteration);
            if (q.VectorsEqual(minterms, reduced)) break;
            minterms = reduced;
            iteration++;
        } while (true);

        vector<string> essentialMinterms = q.removeRedundantMinterms(reduced);

        cout << "\nFinal reduced minterms: ";
        for (const auto& minterm : essentialMinterms)
        {
            cout << minterm << " ";
        }
        cout << endl;

        cout << "The reduced boolean expression in SOP form:" << endl;
        for (size_t i = 0; i < essentialMinterms.size() - 1; i++)
            cout << q.getValue(essentialMinterms[i]) << "+";
        cout << q.getValue(essentialMinterms.back()) << endl;

        cout << "Would you like to enter another expression? (y/n)" << endl;
        cin >> check;
    }
    cout << endl << "---END---" << endl << "Created by Raffay" << endl;
}
