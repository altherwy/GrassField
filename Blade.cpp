//
//  Blade.cpp
//  GrassField
//
//  Created by Youssef Altherwy on 2014-11-21.
//  Copyright (c) 2014 jouzef therwy. All rights reserved.
//

#include "Blade.h"

void Blade::createBlade(Point* _startPoint, float _yEnd, string mode, float _x1, float _x2, float _y1, float _y2){
    this->start = _startPoint;
    float _xBase = this->start->x; float _yBase = this->start->y;
    if (mode == "left") {
    this->tan1->pointShiftingLeft(_xBase, _yBase, _x1, _y1, this->start->z);
    this->tan2->pointShiftingLeft(_xBase, _yBase, _x1, _y2, this->start->z);
    this->tan3->pointShiftingLeft(_xBase, _yBase, _x2, _y2, this->start->z);
    this->tan4->pointShiftingLeft(_xBase, _yBase, _x2, _y1, this->start->z);
    this->end->pointShiftingLeft(_xBase, _yBase, _xEnd, _yEnd, this->start->z);
    }
    else
    {
        this->tan1->pointShiftingRight(_xBase, _yBase, _xShifting1, _yShifting1, this->start->z);
        this->tan2->pointShiftingRight(_xBase, _yBase, _xShifting1, _yShifting2, this->start->z);
        this->tan3->pointShiftingRight(_xBase, _yBase, _xShifting2, _yShifting2, this->start->z);
        this->tan4->pointShiftingRight(_xBase, _yBase, _xShifting2, _yShifting1, this->start->z);
        this->end->pointShiftingRight(_xBase, _yBase, _xEnd, _yEnd, this->start->z);
    }
}

void Blade::motion(Vector2f rotation_limits){
    Point* _end = this->end;
    
    if (_end->y < rotation_limits(0)) goReverse = true;
    if (_end->y > rotation_limits(1)) goReverse = false;
    
    if (goReverse)  _end->y += _rotationSpeed;
    else            _end->y -= _rotationSpeed;
    _end->x = this->calculateXZnewLocation(_end->oy, _end->y, _end->ox);
    _end->z  = this->calculateXZnewLocation(_end->oy, _end->y, _end->oz);
}


Vector2f Blade::rotation_limits(float _maxFactor, float _minFactor){
    Point* _end = this->end;
    Vector2f max_min_limits(0.0,0.0);
    float _max = _end->oy + _maxFactor;
    float _min = _end->oy - _minFactor;
    max_min_limits(1) = _max;
    max_min_limits(0) = _min;
    return max_min_limits;
}

vector<vector<Vector3f> > Blade::drawBlade(){
    int t=50;
    float _x1,_y1,_z1;
    float _x2, _y2, _z2;
    vector<Vector3f> first_curve, second_curve;
    for (int i=0; i < t; i++) {
        float pos = (float) i / (float) t;
        // first curve
        _x1 = this->bezierCurve(pos, this->start->x, this->tan1->x, this->tan2->x, this->end->x);
        _y1 = this->bezierCurve(pos, this->start->y, this->tan1->y, this->tan2->y, this->end->y);
        _z1 = this->bezierCurve(pos, this->start->z, this->tan1->z, this->tan2->z, this->end->z);
        Vector3f first_curve_point(_x1, _y1, _z1);
        first_curve.push_back(first_curve_point);
        
        
        // second curve
        _x2 = this->bezierCurve(pos, this->end->x, this->tan3->x, this->tan4->x, this->start->x);
        _y2 = this->bezierCurve(pos, this->end->y, this->tan3->y, this->tan4->y, this->start->y);
        _z2 = this->bezierCurve(pos, this->end->z, this->tan3->z, this->tan4->z, this->start->z);
        Vector3f second_curve_point(_x2, _y2, _z2);
        second_curve.push_back(second_curve_point);
    }

    vector<vector<Vector3f> > single_blade;
    single_blade.push_back(first_curve); single_blade.push_back(second_curve);
    return single_blade;
}

void Blade::sine_waveMotions(long currentTime, Vector2f rotation_limits, timer tim){
    float pi = 3.14159265359;
    float speed = 10;
    float wave = sin(speed * 2.0*pi* (currentTime));
    wave = (wave)/2.0;
    Point* _end = this->end;
    
    
    if (_end->y < rotation_limits(0)) goReverse = true;
    if (_end->y > rotation_limits(1)) goReverse = false;
    
    if (goReverse)
        _end->y += wave;
        
    else
        _end->y -= wave;
    
    _end->x = this->calculateXZnewLocation(_end->oy, _end->y, _end->ox);
    _end->z  = this->calculateXZnewLocation(_end->oy, _end->y, _end->oz);
    
}


/*
vector<GLfloat> drawBladeHelper(FileHandler::Point start, FileHandler::Point tan1, FileHandler::Point tan2, FileHandler::Point end){
    vector<GLfloat> vertices;
    int i=0;
    int t = 30;
    for (i = 0; i <= t; i++) {
        float pos = (float) i / (float) t;
        GLfloat x = bezierCurve(pos, start._x, tan1._x, tan2._x, end._x);
        GLfloat y = bezierCurve(pos, start._y, tan1._y, tan2._y, end._y);
        // In our case, the z should always be empty
        GLfloat z = bezierCurve(pos, start._z, tan1._z, tan2._z, end._z);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }
    
    return vertices;
}*/