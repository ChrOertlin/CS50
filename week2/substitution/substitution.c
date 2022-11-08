#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
bool checkKey(string key);
string encrypt(string key, string text);
int main(int argc, string argv[])
{
    // Check if an encryption key is given, if not exit
    if (argc < 2)
    {
        printf("Please specify encryption key.\n");
        return 1;
    }
    // Check if multiple command line arguments are given, if so exit
    if (argc > 2)
    {
        printf("Expected exactly 1 argument, more were given.\n");
        return 1;
    }

    string key = argv[1];
    // perform key checks
    if (checkKey(key) == false)
    {
        // If key check fails return error message and exit
        printf("Errorenous key provided. Key must be 26 unique characters and may not contain special signs.\n");
        return 1;
    }
    string plainText = get_string("plaintext: ");
    printf("ciphertext: %s\n", encrypt(key, plainText));

}
bool checkKey(string key)
{
    int n = strlen(key);
    // Check if key is 26 characters long
    if (n < 26)
    {
        return false;
    }
    // Check if any of the keys is not a character i.e. a-z / A-Z
    for (int i = 0; i < n; i++)
    {
        if (((key[i] >= 65 && key[i] <= 90) || (key[i] >= 97 && key[i] <= 122)) == false)
        {
            return false;
        }
        // initaite second for loop to check if this character already exists
        for (int j = i + 1; j < n; j++)
        {
            if (key[i] == key[j])
            {
                return false;
            }
        }
    }
    return true;
}

string encrypt(string key, string text)
{
    int n = strlen(text);
    int arrayPos = 0;

    for (int i = 0; i < n; i++)
    {
        // only encrypt letters
        if (isalpha(text[i]))
        {
            arrayPos = tolower(text[i]) - 97;
            // keep case flexibility in text with regards to key
            if (isupper(text[i]))
            {
                text[i] = toupper(key[arrayPos]);
            }
            else if (islower(text[i]))
            {
                text[i] = tolower(key[arrayPos]);
            }
        }
    }
    return text;
}