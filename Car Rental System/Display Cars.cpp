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
         << "Charges P/D\t"
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