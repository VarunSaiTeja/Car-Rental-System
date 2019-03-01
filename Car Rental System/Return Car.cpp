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
            {
                rented = TRUE;
                break;
            }
            cars_rented_check.read((char *)&Rented_Car, sizeof(Cars_Rented));
        } while (!cars_rented_check.eof());
    }
    cars_rented_check.close();

    if (rented)
    {
        short int confirm, total_cost = 0, total_days = 1, total_months, total_years, late_charge_per_day = 0;
        SYSTEMTIME time_now;
        GetLocalTime(&time_now);
        Cars_In_Shed car_in_shed;
        cout
            << "Cars Rented to " << Rented_Car.customer_name << " :" << endl
            << endl;
        cout << "_________________________________________________________________________________________________________________________________" << endl;
        cout << "Car Number\t"
             << "Car Model\t"
             << "Checked Out\t"
             << "Checked In\t"
             << "Rented For\t"
             << "Extra Days\t"
             << "Charges + Late Charges"
             << endl;
        cout << "_________________________________________________________________________________________________________________________________" << endl
             << endl;
        ifstream dis_rented_cars("cars rented.db");
        dis_rented_cars.read((char *)&Rented_Car, sizeof(Cars_Rented));

        do
        {
            if (!strcmp(mobile_num, Rented_Car.mobile_num))
            {
                total_days = 1;
                late_charge_per_day = 0;
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
                     << (total_days > Rented_Car.rented_days ? Rented_Car.rented_days : total_days) << " Days\t\t"
                     << (total_days > Rented_Car.rented_days ? (total_days - Rented_Car.rented_days) : 0) << " Days\t\t";

                if (total_days > Rented_Car.rented_days)
                {
                    late_charge_per_day = Rented_Car.cost_per_day * 1.1;
                    cout << "(" << Rented_Car.cost_per_day << " X " << Rented_Car.rented_days << ")"
                         << " + (" << late_charge_per_day << " X " << (total_days - Rented_Car.rented_days) << ")"
                         << " = " << ((late_charge_per_day * (total_days - Rented_Car.rented_days)) + (Rented_Car.cost_per_day * Rented_Car.rented_days));
                }
                else
                {
                    cout << Rented_Car.cost_per_day << " X " << total_days
                         << " = " << (Rented_Car.cost_per_day * total_days);
                }
                cout << endl;

                total_cost += ((total_days > Rented_Car.rented_days ? Rented_Car.rented_days : total_days) * Rented_Car.cost_per_day);
                total_cost += late_charge_per_day * (total_days - Rented_Car.rented_days);
            }

            dis_rented_cars.read((char *)&Rented_Car, sizeof(Cars_Rented));
        } while (!dis_rented_cars.eof());

        dis_rented_cars.close();

        cout << endl;
        cout << "_________________________________________________________________________________________________________________________________" << endl;
        cout << "Total Cost : " << total_cost << endl
             << endl;
        cout << "Return All Cars Now" << endl;
        cout << "1. Confirm" << endl
             << "2. Menu" << endl
             << endl
             << "Choice : ";

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
                    strcpy(car_in_shed.car_num, Other_Car.car_num);
                    strcpy(car_in_shed.model, Other_Car.model);
                    put_car.write((char *)&car_in_shed, sizeof(Cars_In_Shed));
                }
                else
                {
                    cars_rented_temp_db.write((char *)&Other_Car, sizeof(Cars_Rented));
                }

                take_car.read((char *)&Other_Car, (sizeof(Cars_Rented)));
            } while (!take_car.eof());

            take_car.close();
            put_car.close();
            cars_rented_temp_db.close();
            remove("cars rented.db");
            rename("temp.db", "cars rented.db");
            system("cls");
            cout << "Cars returned to shed" << endl
                 << endl
                 << "Collect " << total_cost << " Rs from the " << Rented_Car.customer_name << endl
                 << endl;
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