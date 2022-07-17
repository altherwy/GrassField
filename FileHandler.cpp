//
//  FileHandler.cpp
//  GrassField
//
//  Created by Youssef Altherwy on 2014-10-28.
//  Copyright (c) 2014 jouzef therwy. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <iostream>
//#include <string>
#include <sstream>
using namespace std;


class FileHandler{
public:
    FileHandler(){
        
    }
    struct Point{
        float _x,_y,_z;
    };
    
    struct configuration{
        string _ID;
        Point start;
        Point tan1;
        Point tan2;
        Point tan3;
        Point tan4;
        Point end;
        
    };
    
    struct Blade{
        string _ID;
        configuration initial_configurations;
        
    };
};


