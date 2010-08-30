#include <iostream>
#include <fstream>

using namespace std;
class FileTransfer
{
    public: string *ReadFile(char *filename)
    {
        string content[200];
        ifstream inFile;
        int lineNumber=0;


        inFile.open(filename);
        if (!inFile) {
            cout << "Unable to open file";
        }else
        {

            while (inFile.eof()) {
                getline(inFile,content[lineNumber]);
                lineNumber++;
            }

            inFile.close();
            cout << "File has been read, will be analysed!";
        }
        return content;
    }
    public: void WriteFile(char *filename,string *content,int length)
    {
        ofstream output (filename);
        if (output.is_open())
        {
            for(int i=0;i<length;i++)
            {
                output << content[i];
            }
            output.close();
        }
        else
            cout << "Unable to open output file";
    }
};

int main()
{
   FileTransfer filetransfer;
   string result[20];
   result[0]="this is a test";
   filetransfer.ReadFile("read");
   filetransfer.WriteFile("write",result,1);
}
