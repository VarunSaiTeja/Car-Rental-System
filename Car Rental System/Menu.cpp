system("color e0");

short int choice;
std::string design_line;

menu : system("cls");

ifstream design("design.txt");

while (getline(design, design_line))
{
    cout << design_line << endl;
}

design.close();

cout << endl
     << endl;

cout << " Varun Cars Rental Shop" << endl;
cout << " 1. Add a Car" << endl;
cout << " 2. Remove a Car" << endl;
cout << " 3. Display Cars" << endl;
cout << " 4. Rent a Car" << endl;
cout << " 5. Return a Car" << endl;
cout << " 6. Exit" << endl;
cout << " Choice : ";
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