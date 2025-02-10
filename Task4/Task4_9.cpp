#include <iostream>
#include <string>
using namespace std;

string decipher(int a, int b, string message)
{   
    for (int i = 0; i < message.size(); i++) {    //making sure the input has uppercase letters only
        message[i] = toupper(message[i]);
    }

    int index, a_inv = 1;
    char letter;
    string output = "";
    string english = " ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //English alphabet to be used for deciphering

    for (int i = 0; i < 27; i++)
    {
        int flag = (a * i) % 27;   //getting multiplicative inverse of "a", where (a*i)%26 == 1
        if (flag == 1)
        {
            a_inv = i;
            cout << i << endl;
            break;
        }
    }

    for (int i = 0; i < message.size(); i++) {
        if (message[i] == ' ') {    //setting index of "space" to zero
            letter = 0;
        }
        else {
            letter = message[i] - 64; //subtracting 64 from letter's ASCII value to return it to index numbers ranging from 1 to 27 
        }
        index = ((a_inv * (letter - b + 27)) % 27);     //after subtracting "b", add 27 to keep the index value positive while not affecting the value when doing the modulus operation
        output += english[index];   //adding to the output the deciphered letter based on the index value
    }
    return output;
}

int main()
{
    int a, b, message;
    cin << a << b << message;
    //string message = "VDL DLDCJHVPMDUBJIQHB";
    cout << decipher(a, b, message);
}
