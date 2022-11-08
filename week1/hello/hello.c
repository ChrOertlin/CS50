#include <stdio.h>
#include <cs50.h>
int main(void)
{
    // Ask for user input
    string name = get_string("What name have you been given?\t");
    // Parse user input to print statement with fixed string
    printf("hello, %s\n", name);
}