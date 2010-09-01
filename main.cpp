/*
 * File:   main.cpp
 * Author: kianoosh
 *
 * Created on 1 September 2010, 10:55 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>


using namespace std;

/*
 *
 */
static int MAXLINENUMBER=200;
class FileTransfer
{
    public: void ReadFile(char *filename,string content[])
    {
        
        ifstream inFile;
        int lineNumber=0;
        string temp;
        cout<< "content of the file is:\n------------\n";


        inFile.open(filename);
        if (!inFile) {
            cout << "Unable to open file";
        }else
        {
            while (!inFile.eof()) {
                getline(inFile,temp);
                cout<<lineNumber<<" : " << temp<<"\n";
                content[lineNumber]=temp;                
                lineNumber++;
            }

            inFile.close();
            cout << "File has been read, will be analysed!";
        }                
    }
    public: void WriteFile(char *filename,string *content,int length)
    {
        ofstream output (filename);
        if (output.is_open())
        {
            for(int i=0;i<length;i++)
            {
                output << *(content+i);
            }
            output.close();
        }
        else
            cout << "Unable to open output file";
    }
};
class Analysis
{
    public: int numberOfLines;
    public: int StringToDigit(string number)
    {
        if(isdigit(number[0]))
        {
            numberOfLines=atoi(StringToCharArray(number));

        }
        return numberOfLines;
    }
    public: char *StringToCharArray(string s)
    {
        char *a=new char[s.size()+1];
        a[s.size()]=0;
        memcpy(a,s.c_str(),s.size());
        return a;
    }
};

int main(int argc, char** argv) {
   string temp;

   FileTransfer filetransfer;
   Analysis analysis;
   string result[MAXLINENUMBER];
   filetransfer.ReadFile("read",result);
   temp=result[0];
   cout<<"\n--------------------\n Line number is:"<< analysis.StringToDigit(temp);
   filetransfer.WriteFile("write",result,1);
   return 0;
}
