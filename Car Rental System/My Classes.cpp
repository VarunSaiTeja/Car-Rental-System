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