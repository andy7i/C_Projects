#include <iostream>
#include <fstream>
#include <cctype>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


using namespace std;


int main()
{
    srand( time(NULL) );

//    dataFile.open ("randomizedData.txt", ios::out);
    ofstream dataFile ("randomizedData");

    if (dataFile.is_open())
    {
        int first;
        int second;
        for (int i=0; i<300000; i++)
        {
            first = rand() % 1000;
            second = rand() % 100;
            dataFile << first << " " << second << endl;
        }
    }

    dataFile.close();
}
