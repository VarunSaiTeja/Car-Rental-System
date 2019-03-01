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