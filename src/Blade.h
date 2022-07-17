//
//  Blade.h
//  GrassField
//
//  Created by Youssef Altherwy on 2014-11-21.
//  Copyright (c) 2014 jouzef therwy. All rights reserved.
//

#ifndef __GrassField__Blade__
#define __GrassField__Blade__

#include <stdio.h>
#include <math.h>
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include "timer.h"
#include <math.h>
#endif /* defined(__GrassField__Blade__) */
using namespace Eigen;
using namespace std;
struct Point{
    float x,y,z;
    float ox,oy,oz; // original values, once initilized is unchangeable
    Point(float _x,float _y, float _z, float _ox, float _oy, float _oz){
        x = _x;
        y = _y;
        z = _z;
        ox = _ox;
        oy = _oy;
        oz = _oz;
        
    }
    Point(){
        x = 0.0;
        y = 0.0;
        z = 0.0;
        ox = 0.0;
        oy = 0.0;
        oz = 0.0;
    }
    void shifting(float _xBase, float _yBase, float _xShift, float _yShift, float _zValue){
        this->y = _yBase + _yShift;
        this->z = _zValue;
        this->ox = this->x;
        this->oy = this->y;
        this->oz = this->z;
    }
    void pointShiftingLeft(float _xBase, float _yBase, float _xShift, float _yShift, float _zValue){
        
        this->x = _xBase - _xShift;
        shifting( _xBase, _yBase,  _xShift,  _yShift, _zValue);
    }
    void pointShiftingRight(float _xBase, float _yBase, float _xShift, float _yShift, float _zValue){
        
        this->x = _xBase + _xShift;
        shifting( _xBase, _yBase,  _xShift,  _yShift, _zValue);
    }
};

struct Blade{
    int iD;
    Point* start;
    Point* tan1;
    Point* tan2;
    Point* end;
    Point* tan3;
    Point* tan4;
    
    // for the rotation kind of effect
    const float _xShifting1 = 0.00; // 0.03
    const float _xShifting2 = 0.01; // 0.04
    const float _yShifting1 = 0.1; // 0.4
    const float _yShifting2 = 0.6; // 1.0
    const float _xEnd = 0.4; // 0.5
    
    // the rotation like angle value
    const float _rotationSpeed = 0.0009;
    bool goReverse = false;
    bool stop_motion = false;
    
    Blade(int _iD, Point* _start, Point* _tan1, Point* _tan2, Point* _end, Point* _tan3, Point* _tan4){
        iD = _iD;
        start = new Point(_start->x, _start->y, _start->z, _start->ox, _start->oy, _start->oz);
        tan1 =  new Point(_tan1->x, _tan1->y, _tan1->z, _tan1->ox, _tan1->oy, _tan1->oz);
        tan2 =  new Point(_tan2->x, _tan2->y, _tan2->z, _tan2->ox, _tan2->oy, _tan2->oz);
        end =   new Point(_end->x, _end->y, _end->z, _end->ox, _end->oy, _end->oz);
        tan3 =  new Point(_tan3->x, _tan3->y, _tan3->z, _tan3->ox, _tan3->oy, _tan3->oz);
        tan4 =  new Point(_tan4->x, _tan4->y, _tan4->z, _tan4->ox, _tan4->oy, _tan4->oz);
    }
    Blade(int _iD){
        iD = _iD;
        start = new Point();
        tan1 = new Point();
        tan2 = new Point();
        end = new Point();
        tan3 = new Point();
        tan4 = new Point();
    }
    
    void createBlade(Point* _startPoint, float _endY, string mod, float _x1, float _x2, float _y1, float _y2);
    void motion(Vector2f);
    void sine_waveMotions(long currentTime, Vector2f, timer tim );
    
    // for a blade, specify the rotations limits. Call it before starting motions
    Vector2f rotation_limits(float _max, float _min);
    
    vector<vector<Vector3f> > drawBlade();
    
    
    
     // Bezier Curve Cubic Function
    float bezierCurve(float t, float P0,float P1, float P2, float P3) {
        // Cubic bezier Curve
        float point = (pow((1-t), 3.0) * P0) +
        (3 * pow((1-t),2) * t * P1) +
        (3 * (1-t) * t * t * P2) +
        (pow(t, 3) * P3);
        return point;
    }
    
    
    float calculateXZnewLocation(float y1, float y2, float x1){
        float x2 = 0.0;
        if (y1 == y2)
            return x1;
        float temp = 0.0;
        if (y1 > y2 ){
            temp = (y1-y2)/2;
            x2 = x1 - temp;
        }
        else{
            temp = (y2-y1)/2;
            x2 = x1 + temp;
        }
        
        return x2;
    }
    
    Vector2f rotateByAngle(float angle, float oldX, float oldY){
        Vector2f newPoint(0,0);
        float newX = oldX * cos(angle) - oldY * sin(angle);
        float newY = oldX * sin(angle) + oldY * cos(angle);
        newPoint(0) = newX; newPoint(1) = newY;
        return newPoint;
    }
};