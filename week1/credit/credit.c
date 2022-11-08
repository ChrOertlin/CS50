#include <cs50.h>
#include <stdio.h>
int checkManufacturer(int fn, int sn, int length, int remainderCheck);
int main(void)
{
    // ask for the credt card number
    long ccNumber = get_long("Please insert your credit card number:\t");
    // loop through the digits of the numbers
    // keep track of position
    int pos = 1;
    //initiate some variables needed in the loop
    int digit = 0;
    int first = 0;
    int second = 0;
    int sumDigProd = 0;
    int digProd = 0;
    int sumNonProdSumOfProd = 0;

    while (ccNumber != 0)
    {
        digit = ccNumber % 10;
        // reset sumDigitProduct
        sumDigProd = 0;
        // if position is even (i.e. every second) multiply by 2
        if (pos % 2 == 0)
        {
            digProd = digit * 2;
            // if digprod is below 10 simply add it to sumDigProd
            if (digProd < 10)
            {
                sumDigProd = digProd;
            }
            // else we need to get each digit and add that up
            else
            {
                while (digProd != 0)
                {
                    // add the remainder
                    sumDigProd += digProd % 10;
                    digProd /= 10;
                }
            }
            // add the sumDigProd to the sum of non products and sum of all digits of products
            sumNonProdSumOfProd += sumDigProd;
        }
        // if not even use it for the sum of non products and sum of all digits of products
        else if (pos % 2 != 0)
        {
            sumNonProdSumOfProd += digit;
        }
        // to get second position check if ccNumber remainder is smaller than 20 but larger than 9 because it has to have 2 digits at this point
        if (ccNumber > 10 && ccNumber <= 100)
        {
            second = digit;
        }
        // to get the first digit check if ccNumber is smaller than 10 but greater than 0
        if (ccNumber > 0  && ccNumber < 10)
        {
            first = digit;
        }
        ccNumber /= 10;
        if (ccNumber > 0)
        {
            pos++;
        }

    }
    // make checks and return outcome
    checkManufacturer(first, second, pos, sumNonProdSumOfProd);

}


int checkManufacturer(int fn, int sn, int length, int remainderCheck)
{
    string manufacturer = "INVALID\n";
    // first check for remainder of sum of all to be 0 if not return invalid
    if (remainderCheck % 10 != 0)
    {
        return printf("%s", manufacturer);
    }
    // determine origin and length contraints
    // American Express start with 34 or 37
    if (fn == 3 && (sn == 4 || sn == 7) && length == 15)
    {
        manufacturer = "AMEX\n";
    }
    // VISA starts with 4
    else if (fn == 4 && (length == 13 || length == 16))
    {
        manufacturer = "VISA\n";
    }
    // MasterCard starts with 51 52 53 54 55
    else if (fn == 5 && (sn == 1 || sn == 2 || sn == 3 || sn == 4 || sn == 5) && length == 16)
    {
        manufacturer = "MASTERCARD\n";
    }

    return printf("%s", manufacturer);
}