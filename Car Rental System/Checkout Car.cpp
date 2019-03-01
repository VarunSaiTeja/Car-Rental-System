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