#include <iostream>
#include <cmath>
#include <string>
#include "symbolicc++.h"
#include <stdlib.h>

using namespace std;

Symbolic y_("y");
Symbolic x_("x");
Symbolic example = ln(1+x_);
Symbolic necessary_function = 60*3.14/180;
Symbolic uravnenie = cos(x_)*y_ + x_;

int factorial_(int temp)
{
    int summ =1;
    for (int i = 1; i<temp+1; i++)
        summ*= i;
    return summ;
}

class Teylor
{
public:
    Symbolic function_;
    Symbolic necessary_function_;
    Teylor *Next, *Prev;
    Symbolic factor;
};

class Ryad
{
public:
    Symbolic summ;
    Ryad():Head(NULL),Tail(NULL){summ = 0.0;};
    Teylor *Head, *Tail;
    void Add(Symbolic y, Symbolic dobavka, int factor, Symbolic necessary_zapas, Symbolic dobavka_double);
    void Show(string tochka);
    void Integral(Symbolic y,Symbolic dobavka, int i, Symbolic necessary_zapas, Symbolic dobavka_double, string tochka);
    void Show_Integral(string tochka);
    void Derivative(Symbolic y,Symbolic dobavka, int i, Symbolic necessary_zapas, Symbolic dobavka_double, string tochka);
    void Show_Derivative(string tochka);
    void necessary_Show();
    friend void operator + (const Symbolic prosto_chislo, Ryad& lst)
    {
        Teylor *temp = lst.Tail;
        temp->function_ = temp->function_ + (prosto_chislo*temp->factor);
        temp->necessary_function_ = double(temp->necessary_function_+prosto_chislo);
        lst.summ += prosto_chislo;
    }
    friend void operator - (const Symbolic prosto_chislo, Ryad& lst)
    {
        Teylor *temp = lst.Tail;
        temp->function_ = temp->function_ - (prosto_chislo*temp->factor);
        temp->necessary_function_ = double(temp->necessary_function_-prosto_chislo);
        lst.summ -= prosto_chislo;
    }
    friend void operator * (const Symbolic prosto_chislo, Ryad& lst)
    {
        Teylor *temp = lst.Head;
        while (temp!= NULL)
        {
            temp->necessary_function_ = temp->necessary_function_*prosto_chislo;
            temp->function_ = temp->function_*prosto_chislo;
            temp = temp->Next;
        }
        lst.summ = lst.summ*prosto_chislo;
    }
    friend void operator / (const Symbolic prosto_chislo, Ryad& lst)
    {
        Teylor *temp = lst.Head;
        while (temp!= NULL)
        {
            temp->necessary_function_ = temp->necessary_function_/prosto_chislo;
            temp->factor = temp->factor*prosto_chislo;
            temp = temp->Next;
        }
        lst.summ = lst.summ/prosto_chislo;
    }
    ~Ryad()
     {
         while (Head)
         {
             Tail = Head->Next;
             delete Head;
             Head = Tail;
         }
     }
};

Symbolic our_function(Symbolic x)
{
    Symbolic y = ln(1+x);
    return y;

}

void Ryad::Derivative(Symbolic y,Symbolic dobavka, int i, Symbolic necessary_zapas, Symbolic dobavka_double, string tochka)
{
   Teylor *temp = new Teylor;
   temp->Next = NULL;
   dobavka = ((x_-tochka)^i)[tochka == atof(tochka.c_str())];
   temp->function_ = y*dobavka;
   temp->factor = factorial_(i);
   temp->function_ = temp->function_/temp->factor;
   temp->function_ = df(temp->function_[x_], x_);
   temp->necessary_function_ = necessary_zapas*dobavka/factorial(i);
   temp->necessary_function_ = df(temp->necessary_function_[1],x_)[x_ == necessary_function];

   summ = summ+temp->necessary_function_;

   if (Head != NULL)
   {
       temp->Prev = Tail;
       Tail->Next = temp;
       Tail = temp;
   }
   else
   {
       temp->Prev = NULL;
       Head = Tail = temp;
   }
}

void Ryad::Integral(Symbolic y,Symbolic dobavka, int i, Symbolic necessary_zapas, Symbolic dobavka_double, string tochka)
{
   Teylor *temp = new Teylor;
   temp->Next = NULL;
   dobavka = ((x_-tochka)^i)[tochka == atof(tochka.c_str())];
   temp->function_ = y*dobavka;
   temp->factor = factorial_(i);
   temp->function_ = temp->function_/temp->factor;
   temp->function_ = integrate(temp->function_[x_], x_);
   temp->necessary_function_ = integrate((necessary_zapas*dobavka/factorial(i))[x_], x_)[x_ == necessary_function];

   summ = summ+temp->necessary_function_;

   if (Head != NULL)
   {
       temp->Prev = Tail;
       Tail->Next = temp;
       Tail = temp;
   }
   else
   {
       temp->Prev = NULL;
       Head = Tail = temp;
   }

}

void Ryad::Add(Symbolic y, Symbolic dobavka, int i, Symbolic necessary_zapas, Symbolic dobavka_double)
{
   Teylor *temp = new Teylor;
   temp->Next = NULL;
   dobavka = (dobavka^i);
   temp->function_ = y*dobavka;
   temp->factor = factorial_(i);
   temp->necessary_function_ = double (necessary_zapas*(dobavka_double^i)/factorial(i));

   summ = summ+temp->necessary_function_;

   if (Head != NULL)
   {
       temp->Prev = Tail;
       Tail->Next = temp;
       Tail = temp;
   }
   else
   {
       temp->Prev = NULL;
       Head = Tail = temp;
   }
}

void Ryad::Show_Derivative(string tochka)
{
     Teylor *temp = Head;
     cout << "derivative(" << example << ") = (" << temp->function_ << ")";
     temp = temp->Next;
      while (temp != NULL)
     {
         cout <<" + (" << temp->function_ << ")";
         temp = temp->Next;
     }
     cout << endl << endl;
}

void Ryad::Show_Integral(string tochka)
{

     Teylor *temp = Head;
     cout << "integral(" << example << ") = (" << temp->function_ << ")";
     temp = temp->Next;
      while (temp != NULL)
     {
         cout <<" + (" << temp->function_ << ")";
         temp = temp->Next;
     }
     cout <<  endl;
}

void Ryad::Show(string tochka)
{
     Teylor *temp= Head;
     cout << example << "= (" << temp->function_ << ")" << "/" << temp->factor;
     temp = temp->Next;
      while (temp != NULL)
     {
         cout  << " + (" << temp->function_ << ")"<< "/" << temp->factor;
         temp = temp->Next;
     }
     cout << endl;
}

void Ryad:: necessary_Show()
{
    Teylor *temp= Head;
    cout << "= " << temp->necessary_function_;
    temp = temp->Next;
    while (temp != NULL)
     {
         cout  << " + " <<temp->necessary_function_;
         temp = temp->Next;
     }
     cout <<" = "<< summ << endl << endl;
}

void Vibor (Symbolic y)
{
    tryagain:
    int chislo; Ryad lst; string tochka; char vibor; double tochka_double; Symbolic dobavka_double;

    cin >> vibor;

    if (vibor == '6'){}

    else if ((vibor!= '6') && (vibor!= '5'))
    {
        cout << "Выберите количество членов ряда: ";
        cin >> chislo;
        cout << "Выберите точку в которой будет находиться производная: ";
        cin >> tochka;
        Symbolic zapas = our_function(tochka);
        Symbolic necessary_zapas;
        Symbolic x = tochka, dobavka = (x_-tochka);
        if (tochka == "0")
                dobavka = x_;
        tochka_double = atof(tochka.c_str());
        dobavka_double = (necessary_function - tochka_double);

        if (vibor == '1')
        {
            for (int i = 0; i<chislo; i++)
            {
                necessary_zapas = df(zapas[tochka], tochka)[tochka == atof(tochka.c_str())];
                zapas = df(zapas[tochka], tochka);
                lst.Add(zapas, dobavka, i+1, necessary_zapas, dobavka_double);
            }
            lst.Show(tochka);
            lst.necessary_Show();
            cout << endl << "Выберите действие (число): ";
            cout<< endl;
            goto tryagain;
        }
        else if (vibor == '3')
        {
            for (int i = 0; i<chislo; i++)
            {
                necessary_zapas =  df(zapas[tochka], tochka)[tochka == atof(tochka.c_str())];
                zapas = df(zapas[tochka], tochka);
                lst.Integral(zapas, dobavka, i, necessary_zapas, dobavka_double, tochka);
            }
            lst.Show_Integral(tochka);
            lst.necessary_Show();
            cout << endl << "Выберите действие (число): ";
            cout<< endl;
            goto tryagain;
        }
        else if (vibor == '2')
        {
            for (int i = 0; i<chislo; i++)
            {
                necessary_zapas =  df(zapas[tochka], tochka)[tochka == atof(tochka.c_str())];
                zapas = df(zapas[tochka], tochka);
                lst.Derivative(zapas,dobavka,i+1, necessary_zapas, dobavka_double, tochka);
            }
            lst.Show_Derivative(tochka);
            lst.necessary_Show();
            cout << endl << "Выберите действие (число): ";
            cout<< endl;
            goto tryagain;
        }
        else if (vibor == '4')
        {
            string pr_chislo; Symbolic prosto_chislo;

            for (int i = 0; i<chislo; i++)
            {
                necessary_zapas =  df(zapas[tochka], tochka)[tochka == atof(tochka.c_str())];
                zapas = df(zapas[x], x);
                lst.Add(zapas, dobavka, i+1, necessary_zapas, dobavka_double);
            }

            arifmetika:
            cout << "Выберите арифметическую операцию (число) :" << endl << "1)+" << endl << "2)-" << endl << "3)*" << endl << "4)/" << endl << "Ваш выбор: ";
            cin >> vibor;


            if (vibor == '1')
            {
                cout<< endl << "выберите число, которое хотели бы прибавить: ";
                cin >> pr_chislo;
                prosto_chislo = atof(pr_chislo.c_str());
                prosto_chislo+lst;
                lst.Show(tochka);
                lst.necessary_Show();
                misstake1:
                cout << "Хотите продолжить работу с этим рядом? (yes или no)" << endl << endl;
                cin >> pr_chislo;
                if (pr_chislo == "yes")
                    goto arifmetika;
                else if (pr_chislo == "no")
                {
                    cout << "Выберите действие (число):" << endl << endl;
                    goto tryagain;
                }
                else
                {
                    cout << "Возможно Вы ошиблись..." << endl<< endl;
                    goto misstake1;
                }
            }
            if (vibor == '2')
            {
                cout<< endl << "выберите число, которое хотели бы вычесть: ";
                cin >> pr_chislo;
                prosto_chislo = atof(pr_chislo.c_str());
                prosto_chislo-lst;
                lst.Show(tochka);
                lst.necessary_Show();
                misstake2:
                cout << "Хотите продолжить работу с этим рядом? (yes или no)" << endl << endl;
                cin >> pr_chislo;
                if (pr_chislo == "yes")
                    goto arifmetika;
                else if (pr_chislo == "no")
                {
                    cout << "Выберите действие (число):" << endl << endl;
                    goto tryagain;
                }
                else
                {
                    cout << "Возможно Вы ошиблись..." << endl<< endl;
                    goto misstake2;
                }
            }
            if (vibor == '3')
            {
                cout<< endl << "выберите число, на которое Вы бы хотели умножить: ";
                cin >> pr_chislo;
                prosto_chislo = atof(pr_chislo.c_str());
                prosto_chislo*lst;
                lst.Show(tochka);
                lst.necessary_Show();
                misstake3:
                cout << "Хотите продолжить работу с этим рядом? (yes или no)" << endl << endl;
                cin >> pr_chislo;
                if (pr_chislo == "yes")
                    goto arifmetika;
               else if (pr_chislo == "no")
                {
                    cout << "Выберите действие (число):" << endl << endl;
                    goto tryagain;
                }
                else
                {
                    cout << "Возможно Вы ошиблись..." << endl<< endl;
                    goto misstake3;
                }
            }
            if (vibor == '4')
            {
                cout<< endl << "выберите число, на которое Вы бы хотели поделить: ";
                cin >> pr_chislo;
                prosto_chislo = atof(pr_chislo.c_str());
                prosto_chislo/lst;
                lst.Show(tochka);
                lst.necessary_Show();
                misstake4:
                cout << "Хотите продолжить работу с этим рядом? (yes или no)" << endl << endl;
                cin >> pr_chislo;
                if (pr_chislo == "yes")
                    goto arifmetika;
                else if (pr_chislo == "no")
                {
                    cout << "Выберите действие (число):" << endl << endl;
                    goto tryagain;
                }
                else
                {
                    cout << "Возможно Вы ошиблись..." << endl<< endl;
                    goto misstake4;
                }

            }
        }
    }

    if (vibor == '5')
    {
        Symbolic C("C"), C2("C2");
        cout << "Дифференциальное уравнение первого порядка в общем случае выглядит: y' = g(x)y + f(x)";
        Symbolic yi = uravnenie;
        cout << endl << "y' = " << yi;
        Symbolic odnorodnoye = yi - yi[y == 0];
        Symbolic x_koeff = (odnorodnoye[y == 1])/odnorodnoye;
        Symbolic y_koeff = odnorodnoye[y == 1];
        cout << endl << "Решим соответствующее однородное: y' = " << odnorodnoye << endl;
        Symbolic y_1 = integrate(x_koeff[y],y);
        Symbolic x_1 = integrate(y_koeff[x_], x_);
        x_1 = x_1;
        Symbolic Ci("C'");
        y_1 =C*exp(x_1); // решение однороного
        cout << "Решение однородного: " << "y = " << y_1 << endl;
        Symbolic y_1i = Ci*exp(x_1)+C*df((exp(x_1))[x_], x_);
        Symbolic Constant = y_1i - yi[y == y_1];
        Symbolic Constanti = Constant - Constant[Ci == 0];
        x_koeff = Constanti[Ci == 1];
        y_koeff = Constant[Ci == 0];
        Ci = -y_koeff/x_koeff;
        y_1 = (integrate(Ci[x_],x_)+C2)*exp(x_1); // нормальное решение
        cout << "Решение простого дифференциального уравнения первого порядка: y = " << y_1;

        cout<<endl<<endl<< "Выберите действие (число): ";

        goto tryagain;
    }

}

int main()
{
     setlocale(LC_ALL, "rus");
     Symbolic y("y");
     cout << "Выберите действие (число):" << endl << "1)Моделирование отрезка ряда Тейлора;" << endl << "2)Дифференцирование ряда;";
     cout <<endl << "3)Интегрирование ряда;" << endl << "4)Арифмметические операции;" << endl << "5)Простое дифференциальное уравнение первого порядка;" << endl<< "6)Закончить;" << endl << endl;
     Vibor(y);
     return 0;
}
