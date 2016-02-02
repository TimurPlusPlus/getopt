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
        if(argc == 1)   //Если не переадно аргументов
           return "-1";

        string *argv = new string[argc];                    //Строка для хранения аргументов.
        for(int i = 0; i < argc; i++)
            argv[i] = (const char*) argvC[i];               //Для удобства преобразуем к string

        for(int i = 1; i < argc; i++)                       //Разбираем параметры. Разделяем параметр и аргумент. Ищем help
        {
            while(argv[i] != "")
            {
                if(argv[i].substr(0, 2) == "--")             //В случае, если это параметр, а не его аргумент. /--a--b
                {
                    argv[i] = argv[i].substr(2, argv[i].length()); //a--b  b
                    string curArg = "";                     //Строка для хранения текущего аргумента.
                    int a = 2000;                             //Индекс вхождения "--" в строку.
                    int b = 2000;                             //Индекс вхождения "=" в строку.

                    if(argv[i].find("--") != string::npos)  //Если подстрока "--" найдена.
                        a = argv[i].find("--");
                    if(argv[i].find("=") != string::npos)
                        b = argv[i].find("=");

                    if(a == 2000 && b == 2000)                  //В строке остался только параметр и никаких лишних знаков типа "--" и "=".
                    {
                        curArg = argv[i];
                        argv[i] = "";
                        if(i + 1 < argc)            //Если еще есть аргументы.
                        {
                            if(argv[i + 1].substr(0, 2) == "--")    //Если следующий аргумент - тоже параметр.
                                arguments.push(" ");           //То для текущего параметра нет аргументов.
                        }
                        else
                            arguments.push(" ");           //Если больше не пришло параметров. То для текущего параметра нет аргументов.
                    }
                    else if (a < b)                         //Если "--" встретился раньше, чем "="
                    {
                        curArg = argv[i].substr(0, a);
                        argv[i] = argv[i].substr(a, argv[i].length());      //Отсекаем от параметр, который находится до "--" /--b
                        arguments.push(" ");
                    }
                    else if (b < a)
                    {
                        curArg = argv[i].substr(0, b + 1);                  //Берем параметр вместе с "=": a=
                        argv[i] = argv[i].substr(b + 1, argv[i].length());
                    }
                    options.push(curArg);                              //Заносим параметр в очередь со всеми параметрами.
                    if(curArg == "help")                                    //Если это параметр "help"
                    {
                        for(int iOption = 0; iOption < 3; iOption++)        //Перебираем все параметры, которыми располагаем.
                        {
                            if(!optArgs[iOption].required)                  //Если параметр не обязательный.
                                cout << "[";
                            cout << "--" << optArgs[iOption].optionName;    //Выводим параметр
                            if(optArgs[iOption].hasArgument)                //Если у параметра есть аргументы.
                            {
                                cout << " {";                               //Обозначение того, что должен быть выбран ОДИН ИЗ аргументов.
                                for(int iArg = 0; iArg < optArgs[iOption].args.size(); iArg++) //Перебираем аргументы параметра.
                                    cout << optArgs[iOption].args.at(iArg) << "|";         //Выводим аргументы.
                                cout << "} ";
                            }
                            if(!optArgs[iOption].required)
                                cout << "] ";
                        }
                        return "-1";                //Обозначение того, что параметров не осталось. Т.к help перекрывает другие параметры
                    }
                }
                else                                                    //Текущее значение есть аргумент параметра.
                {
                    arguments.push(argv[i]);                           //Заносим в набор аргументов.
                    argv[i] = "";
                }
            }
        }
        repacked = true;                        //Все строки разобраны. Параметры и их аргументы разделены.
    }
    if(options.size() == 0) return "-1";        //Если параметров больше не осталось, возвращаем признак конца.
    arg = arguments.front();                    //Помещаем аргумент в глобальную переменную.
    arguments.pop();
    string o = options.front();
    options.pop();
    return o;                                   //Возвращаем параметр.
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
