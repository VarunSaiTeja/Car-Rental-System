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
