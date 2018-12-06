//
// Created by Kin on 2018-12-06.
// Copyright © 2018 jihuisoft. All rights reserved.
//
//
//#include <KinBase/Object.h>
//
//class CarId{};
//class CarName{};
//
//class Car : public KinBase::Object
//{
//public:
//    void SetCarName(std::string name)
//    {
//        SetProperty<std::string>("name");
//    }
//    
//    std::string Name()
//    { 
//        this->GetProperty<CarName>();
//    }
//    void Name(std::string Name)
//    {
//        this->SetProperty<CarName>("benchi");
//    }
//};
//
//
//
//class Driver :public KinBase::Object
//{
//};
//
//void Test()
//{
//    Car c;
//    
//    c.SetProperty<CarId>("mycar");
//    
//
//    Driver driver;
//    
//    driver.SetProperty<Car>(c);
//    
//}