// Car Rental System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <Windows.h>
#include <fstream>
#include <string.h>
#include <time.h>
#include <iostream>
using namespace std;

class Car_Model
{
  public:
    char manufacturer[10], model[10], color[10];
    float fuel_capacity, mileage, cost_per_day;
    short int seating_capacity;
};

class Cars_In_Shed
{
  public:
    char car_num[12], model[10];
};

class Cars_Rented : public Cars_In_Shed
{
  public:
    char customer_name[10], mobile_num[12];
    SYSTEMTIME time;
    short int rented_days, cost_per_day;
};

void add_car()
{
    Car_Model New_Model, Old_Model;
    Cars_In_Shed New_Car, Old_Car;
    bool model_found = FALSE, car_num_found = FALSE;

    cout << "Car Model : ";
    cin >> New_Car.model;
    strcpy(New_Model.model, New_Car.model);
    ifstream model_check("car models.db");

    if (!model_check.eof())
        model_check.read((char *)&Old_Model, (sizeof(Car_Model)));

    while (!model_check.eof())
    {
        if (!strcmp(Old_Model.model, New_Car.model))
        {
            model_found = TRUE;
            break;
        }
        model_check.read((char *)&Old_Model, (sizeof(Car_Model)));
    }
    model_check.close();

    if (!model_found)
    {
        cout << "Manufacturer : ";
        cin >> New_Model.manufacturer;
        cout << "Seating Capacity : ";
        cin >> New_Model.seating_capacity;
        cout << "Fuel Capacity : ";
        cin >> New_Model.fuel_capacity;
        cout << "Mileage : ";
        cin >> New_Model.mileage;
        cout << "Color : ";
        cin >> New_Model.color;
        cout << "Cost Per Day : ";
        cin >> New_Model.cost_per_day;

        ofstream add_car_model_db("car models.db", ios::app);
        add_car_model_db.write((char *)&New_Model, (sizeof(Car_Model)));
        add_car_model_db.close();
    }

    system("cls");
    cout << "Car Number : ";
    cin >> New_Car.car_num;

    ifstream dup_car("cars in shed.db");

    if (!dup_car.eof())
    {
        dup_car.read((char *)&Old_Car, (sizeof(Cars_In_Shed)));

        do
        {
            if (!strcmp(Old_Car.car_num, New_Car.car_num))
            {
                car_num_found = TRUE;
            }
            dup_car.read((char *)&Old_Car, (sizeof(Cars_In_Shed)));
        } while (!dup_car.eof());

        dup_car.close();
    }

    if (!car_num_found)
    {
        ofstream add_car_in_shed_db("cars in shed.db", ios::app);
        add_car_in_shed_db.write((char *)&New_Car, (sizeof(Cars_In_Shed)));
        add_car_in_shed_db.close();
    }
    else
    {
        system("cls");
        cout << "Car " << New_Car.car_num << " is already present in database" << endl
             << endl;
        return;
    }

    cout << "Car is added to database" << endl
         << endl;
}

void remove_car()
{
    Cars_In_Shed My_Car;
    bool car_found = FALSE;
    char car_num_remove[12];
    cout << "Car Number to Delete : ";
    cin >> car_num_remove;
    system("cls");

    ifstream db("cars in shed.db");
    ofstream temp("temp.db");

    db.read((char *)&My_Car, (sizeof(Cars_In_Shed)));

    do
    {
        if (strcmp(My_Car.car_num, car_num_remove))
        {
            temp.write((char *)&My_Car, (sizeof(Cars_In_Shed)));
        }
        else
        {
            car_found = TRUE;
        }
        db.read((char *)&My_Car, (sizeof(Cars_In_Shed)));
    } while (!db.eof());

    db.close();
    temp.close();
    remove("cars in shed.db");
    rename("temp.db", "cars in shed.db");

    if (car_found)
    {
        cout << "Car has been removed from DataBase" << endl;
    }
    else
    {
        cout << "Oops, Car " << car_num_remove << " Not found" << endl;
    }
}

void display_cars()
{
    Car_Model Car;
    Cars_In_Shed Car_in_shed;
    int cars_count = 0;
    cout << "\t\t\t\tVarun Cars Rental Shop" << endl
         << endl;
    cout << "___________________________________________________________________________________________________________" << endl;
    cout << "Car Model\t"
         << "Seating Capacity\t"
         << "Fuel Capacity\t\t"
         << "Mileage\t\t"
         << "Charges P/H\t"
         << "Avilable\t";
    cout << endl;
    cout << "___________________________________________________________________________________________________________" << endl;

    ifstream car_models_db("car models.db");
    ifstream cars_in_shed_db("cars in shed.db");

    if (!car_models_db.eof())
    {
        car_models_db.read((char *)&Car, (sizeof(Car_Model)));

        do
        {
            cars_count = 0;
            cars_in_shed_db.clear();
            cars_in_shed_db.seekg(0, ios::beg);
            cars_in_shed_db.read((char *)&Car_in_shed, (sizeof(Cars_In_Shed)));

            do
            {
                if (!strcmp(Car.model, Car_in_shed.model))
                    ++cars_count;

                cars_in_shed_db.read((char *)&Car_in_shed, (sizeof(Cars_In_Shed)));

            } while (!cars_in_shed_db.eof());

            cout << Car.model << "\t\t"
                 << Car.seating_capacity << " Members\t\t"
                 << Car.fuel_capacity << " Liters\t\t"
                 << Car.mileage << " KMPH\t\t"
                 << "Rs " << Car.cost_per_day << "\t\t"
                 << cars_count << " Car" << (cars_count > 1 ? "s" : "") << endl;

            car_models_db.read((char *)&Car, (sizeof(Car_Model)));
        } while (!car_models_db.eof());
    }
    else
    {
        cout << "Shed Empty";
    }

    car_models_db.close();
    cars_in_shed_db.close();
    cout << endl;
}

void checkout_car()
{
    SYSTEMTIME time;
    GetLocalTime(&time);

    Cars_In_Shed car_in_shed;
    Car_Model My_Car_Model;
    char model[12];
    short int cars_avilable = 0, taken = 0, req_cars, choice;
    bool model_found = FALSE;

    cout << "Enter Car Model : ";
    cin >> model;
    system("cls");

    ifstream fetch_models("car models.db");
    if (!fetch_models.eof())
        fetch_models.read((char *)&My_Car_Model, (sizeof(Car_Model)));

    while (!fetch_models.eof())
    {
        if (strcmp(model, My_Car_Model.model))
        {
            model_found = TRUE;
            break;
        }
        fetch_models.read((char *)&My_Car_Model, (sizeof(Car_Model)));
    }

    if (model_found)
    {
        ifstream fetch_cars("cars in shed.db");
        fetch_cars.read((char *)&car_in_shed, (sizeof(Cars_In_Shed)));

        do
        {
            if (!strcmp(car_in_shed.model, model))
            {
                ++cars_avilable;
            }
            fetch_cars.read((char *)&car_in_shed, (sizeof(Cars_In_Shed)));
        } while (!fetch_cars.eof());
        fetch_cars.close();

        if (cars_avilable)
        {
            cout << "How many cars required : ";
            cin >> req_cars;
            system("cls");

            if (cars_avilable >= req_cars)
            {
                Cars_Rented Rent_Car;
                Rent_Car.cost_per_day = My_Car_Model.cost_per_day;
                cout << "Enter Customer Details" << endl;
                cout << "Name : ";
                cin >> Rent_Car.customer_name;
                cout << "Mobile : ";
                cin >> Rent_Car.mobile_num;
                cout << "Rent For Days : ";
                cin >> Rent_Car.rented_days;
                system("cls");

                cout << "Order Details : " << endl
                     << endl;
                cout << "Name   : " << Rent_Car.customer_name << endl;
                cout << "Mobile : " << Rent_Car.mobile_num << endl;
                cout << "Number of Cars : " << req_cars << endl;
                cout << "Rent For Days  : " << Rent_Car.rented_days << endl;
                cout << "Charge Per Day : " << Rent_Car.cost_per_day << endl;
                cout << "Total Approx Charges : " << (Rent_Car.cost_per_day * Rent_Car.rented_days * req_cars) << endl;

                cout << endl
                     << "Confirm Details" << endl;
                cout << endl
                     << "1. Confirm" << endl
                     << "2. Return to Menu" << endl
                     << "Choice : ";
                cin >> choice;
                system("cls");

                if (choice == 1)
                {
                    GetLocalTime(&Rent_Car.time);
                    cout << "Cars Rented to Customer : " << endl
                         << endl;

                    ifstream take_car("cars in shed.db");
                    ofstream cars_in_shed_temp_db("temp.db");
                    ofstream put_car("cars rented.db", ios::app);
                    take_car.read((char *)&car_in_shed, (sizeof(Cars_In_Shed)));

                    do
                    {
                        if ((!strcmp(car_in_shed.model, model)) && (taken < req_cars))
                        {
                            strcpy(Rent_Car.car_num, car_in_shed.car_num);
                            strcpy(Rent_Car.model, car_in_shed.model);
                            put_car.write((char *)&Rent_Car, (sizeof(Cars_Rented)));
                            cout << car_in_shed.car_num << endl;
                            ++taken;
                        }
                        else
                        {
                            cars_in_shed_temp_db.write((char *)&car_in_shed, (sizeof(Cars_In_Shed)));
                        }

                        take_car.read((char *)&car_in_shed, (sizeof(Cars_In_Shed)));
                    } while (!take_car.eof());

                    take_car.close();
                    put_car.close();
                    cars_in_shed_temp_db.close();
                    remove("cars in shed.db");
                    rename("temp.db", "cars in shed.db");
                    cout << endl;
                }
                else
                {
                    return;
                }
            }
            else
            {
                cout << "Oops, only " << cars_avilable << " car" << (cars_avilable > 1 ? "s" : "") << " avilable in shed" << endl;
            }
        }
        else
        {
            cout << "Opps, " << model << " model cars not avilable in shed" << endl;
        }
    }
    else
    {
        cout << "Opps, Car Model " << model << " Not Registered";
    }
}

void return_car()
{
    char mobile_num[12];
    Cars_Rented Rented_Car, Other_Car;
    bool rented = FALSE;
    cout << "Customer Mobile Number : ";
    cin >> mobile_num;
    system("cls");

    ifstream cars_rented_check("cars rented.db");

    if (!cars_rented_check.eof())
    {
        cars_rented_check.read((char *)&Rented_Car, sizeof(Cars_Rented));
        do
        {
            if (!strcmp(mobile_num, Rented_Car.mobile_num))
                rented = TRUE;
            break;
            cars_rented_check.read((char *)&Rented_Car, sizeof(Cars_Rented));
        } while (!cars_rented_check.eof());
    }
    cars_rented_check.close();

    if (rented)
    {
        short int total_cost = 0, total_days = 1, total_months, total_years, late_charges = 0;
        SYSTEMTIME time_now;
        GetLocalTime(&time_now);
        Cars_In_Shed car_in_shed;
        cout
            << "Cars Rented to " << Rented_Car.customer_name << " :" << endl
            << endl;
        cout << "__________________________________________________________________________________________________________________" << endl;
        cout << "Car Number\t"
             << "Car Model\t"
             << "Checked Out\t"
             << "Checked In\t"
             << "Rented For\t"
             << "Extra Days\t"
             << "Charges + Late Charges"
             << endl;
        cout << "__________________________________________________________________________________________________________________" << endl
             << endl;
        ifstream dis_rented_cars("cars rented.db");
        dis_rented_cars.read((char *)&Rented_Car, sizeof(Cars_Rented));

        do
        {
            if (!strcmp(mobile_num, Rented_Car.mobile_num))
            {
                total_days = 0;
                if (time_now.wYear == Rented_Car.time.wYear)
                {
                    if (time_now.wMonth == Rented_Car.time.wMonth)
                    {
                        total_days += time_now.wDay - Rented_Car.time.wDay;
                    }

                    if (time_now.wMonth > Rented_Car.time.wMonth)
                    {
                        total_months = time_now.wMonth - Rented_Car.time.wMonth;
                        total_days += 30 - Rented_Car.time.wDay;
                        total_days += time_now.wDay;
                        total_days += (total_months - 1) * 30;
                    }
                }
                else
                {
                    if (time_now.wYear > Rented_Car.time.wYear)
                    {
                        total_years = time_now.wYear - Rented_Car.time.wYear;
                        total_months = 12 - Rented_Car.time.wMonth;
                        total_months += time_now.wMonth;
                        total_days += 30 - Rented_Car.time.wDay;
                        total_days += time_now.wDay;
                        total_days += (total_months - 1) * 30;
                        total_days += (total_years - 1) * 365;
                    }
                }

                cout << Rented_Car.car_num << "\t"
                     << Rented_Car.model << "\t\t"
                     << Rented_Car.time.wDay << "-"
                     << Rented_Car.time.wMonth << "-"
                     << Rented_Car.time.wYear << "\t"
                     << time_now.wDay << "-"
                     << time_now.wMonth << "-"
                     << time_now.wYear << "\t"
                     << Rented_Car.rented_days << " Days\t\t"
                     << (total_days - Rented_Car.rented_days) << " Days\t\t";

                if (total_days > Rented_Car.rented_days)
                {
                    cout << "(" << Rented_Car.cost_per_day << " X " << total_days << ")"
                         << " + (" << 50 << " X " << (total_days - Rented_Car.rented_days) << ")"
                         << " = " << ((50 * (total_days - Rented_Car.rented_days)) + (Rented_Car.cost_per_day * (total_days - Rented_Car.rented_days)));
                    late_charges += 50 * (total_days - Rented_Car.rented_days);
                }
                else
                {
                    cout << Rented_Car.cost_per_day << " X " << total_days
                         << " = " << (Rented_Car.cost_per_day * total_days);
                }
                cout << endl;

                total_cost += (total_days * Rented_Car.cost_per_day);
                total_cost += late_charges;
            }

            dis_rented_cars.read((char *)&Rented_Car, sizeof(Cars_Rented));
        } while (!dis_rented_cars.eof());

        dis_rented_cars.close();

        cout << endl;
        cout << "__________________________________________________________________________" << endl;
        cout << "Total Cost : " << total_cost << endl
             << endl;
        cout << "Return All Cars Now" << endl;
        cout << "1. Confirm" << endl
             << "2. Menu" << endl
             << endl
             << "Choice : ";

        int confirm;
        scanf("%d", &confirm);

        switch (confirm)
        {
        case 1:
        {
            ifstream take_car("cars rented.db");
            ofstream cars_rented_temp_db("temp.db");
            ofstream put_car("cars in shed.db", ios::app);
            take_car.read((char *)&Other_Car, (sizeof(Cars_Rented)));

            do
            {
                if (!strcmp(Rented_Car.mobile_num, Other_Car.mobile_num))
                {
                    strcpy(car_in_shed.car_num, Rented_Car.car_num);
                    strcpy(car_in_shed.model, Rented_Car.model);
                    put_car.write((char *)&car_in_shed, sizeof(Cars_In_Shed));
                }
                else
                {
                    cars_rented_temp_db.write((char *)&Rented_Car, sizeof(Cars_Rented));
                }

                take_car.read((char *)&Other_Car, (sizeof(Cars_Rented)));
            } while (!take_car.eof());

            take_car.close();
            put_car.close();
            cars_rented_temp_db.close();
            remove("cars rented.db");
            rename("temp.db", "cars rented.db");
            cout << endl;
        }
        break;
        case 2:
        {
            system("cls");
            return;
        }
        default:
            break;
        }
    }
    else
    {
        cout << "Cars not rented to customer" << endl;
    }
}

int main()
{
    system("color e0");
    int choice;
menu:
    system("cls");
    cout << "Varun Cars Rental Shop" << endl;
    cout << "1. Add a Car" << endl;
    cout << "2. Remove a Car" << endl;
    cout << "3. Display Cars" << endl;
    cout << "4. Rent a Car" << endl;
    cout << "5. Return a Car" << endl;
    cout << "6. Exit" << endl;
    cout << "Choice : ";
    cin >> choice;
    system("cls");

    switch (choice)
    {
    case 1:
        add_car();
        break;
    case 2:
        remove_car();
        break;
    case 3:
        display_cars();
        break;
    case 4:
        checkout_car();
        break;
    case 5:
        return_car();
        break;
    case 6:
        exit(0);
        break;
    default:
        break;
    }

    system("pause");
    goto menu;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
