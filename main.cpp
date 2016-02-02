#include <iostream>
#include <queue>
using namespace std;

struct optionArguments
{
    string optionName;
    vector <string> args;
    bool required;
    bool hasArgument;
};
optionArguments *optArgs;

bool repacked = false;
queue <string> options;
queue <string> arguments;
string arg;

string getopt(int argc, char **argvC)
{
    if(!repacked)
    {
        if(argc == 1)   //���� �� �������� ����������
           return "-1";

        string *argv = new string[argc];                    //������ ��� �������� ����������.
        for(int i = 0; i < argc; i++)
            argv[i] = (const char*) argvC[i];               //��� �������� ����������� � string

        for(int i = 1; i < argc; i++)                       //��������� ���������. ��������� �������� � ��������. ���� help
        {
            while(argv[i] != "")
            {
                if(argv[i].substr(0, 2) == "--")             //� ������, ���� ��� ��������, � �� ��� ��������. /--a--b
                {
                    argv[i] = argv[i].substr(2, argv[i].length()); //a--b  b
                    string curArg = "";                     //������ ��� �������� �������� ���������.
                    int a = 2000;                             //������ ��������� "--" � ������.
                    int b = 2000;                             //������ ��������� "=" � ������.

                    if(argv[i].find("--") != string::npos)  //���� ��������� "--" �������.
                        a = argv[i].find("--");
                    if(argv[i].find("=") != string::npos)
                        b = argv[i].find("=");

                    if(a == 2000 && b == 2000)                  //� ������ ������� ������ �������� � ������� ������ ������ ���� "--" � "=".
                    {
                        curArg = argv[i];
                        argv[i] = "";
                        if(i + 1 < argc)            //���� ��� ���� ���������.
                        {
                            if(argv[i + 1].substr(0, 2) == "--")    //���� ��������� �������� - ���� ��������.
                                arguments.push(" ");           //�� ��� �������� ��������� ��� ����������.
                        }
                        else
                            arguments.push(" ");           //���� ������ �� ������ ����������. �� ��� �������� ��������� ��� ����������.
                    }
                    else if (a < b)                         //���� "--" ���������� ������, ��� "="
                    {
                        curArg = argv[i].substr(0, a);
                        argv[i] = argv[i].substr(a, argv[i].length());      //�������� �� ��������, ������� ��������� �� "--" /--b
                        arguments.push(" ");
                    }
                    else if (b < a)
                    {
                        curArg = argv[i].substr(0, b + 1);                  //����� �������� ������ � "=": a=
                        argv[i] = argv[i].substr(b + 1, argv[i].length());
                    }
                    options.push(curArg);                              //������� �������� � ������� �� ����� �����������.
                    if(curArg == "help")                                    //���� ��� �������� "help"
                    {
                        for(int iOption = 0; iOption < 3; iOption++)        //���������� ��� ���������, �������� �����������.
                        {
                            if(!optArgs[iOption].required)                  //���� �������� �� ������������.
                                cout << "[";
                            cout << "--" << optArgs[iOption].optionName;    //������� ��������
                            if(optArgs[iOption].hasArgument)                //���� � ��������� ���� ���������.
                            {
                                cout << " {";                               //����������� ����, ��� ������ ���� ������ ���� �� ����������.
                                for(int iArg = 0; iArg < optArgs[iOption].args.size(); iArg++) //���������� ��������� ���������.
                                    cout << optArgs[iOption].args.at(iArg) << "|";         //������� ���������.
                                cout << "} ";
                            }
                            if(!optArgs[iOption].required)
                                cout << "] ";
                        }
                        return "-1";                //����������� ����, ��� ���������� �� ��������. �.� help ����������� ������ ���������
                    }
                }
                else                                                    //������� �������� ���� �������� ���������.
                {
                    arguments.push(argv[i]);                           //������� � ����� ����������.
                    argv[i] = "";
                }
            }
        }
        repacked = true;                        //��� ������ ���������. ��������� � �� ��������� ���������.
    }
    if(options.size() == 0) return "-1";        //���� ���������� ������ �� ��������, ���������� ������� �����.
    arg = arguments.front();                    //�������� �������� � ���������� ����������.
    arguments.pop();
    string o = options.front();
    options.pop();
    return o;                                   //���������� ��������.
}


int main(int argc, char **argv)
{
    optionArguments a, b, c;
    a.optionName = "a";
    a.hasArgument = true;
    a.required = false;
    a.args.push_back("2");
    a.args.push_back("str1");
    a.args.push_back("3");

    b.optionName = "b=";
    b.hasArgument = true;
    b.required = true;
    b.args.push_back("4");
    b.args.push_back("5");
    b.args.push_back("6");

    c.optionName = "c";
    c.hasArgument = false;
    c.required = false;

    optArgs = new optionArguments[3];
    optArgs[0] = a;
    optArgs[1] = b;
    optArgs[2] = c;

    string opt = getopt(argc, argv);
    while(opt != "-1")
        {
            cout << "Hi! It is -- " << opt <<" and an argument is: " << arg << endl;
            opt = getopt(argc, argv);
        }
    return 0;
}
