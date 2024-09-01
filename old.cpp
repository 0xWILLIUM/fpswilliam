#include <math.h>
#include <iostream>

const float pi = 22/7;

int calc_pts(int theta, int hypt, float* outp) {
    std::cout << "calcing the points";
    if (outp == nullptr) return -1; // check if the output is null
    
    float angle = theta % 90; // find the angle
    float quadrant = theta / 90; // 0 being topright, with increments being clockwise
    // calculate the points
    float x = sinf(angle) * hypt;
    float y = sqrt(pow(hypt, 2) - pow(x, 2)); 
    // move to outp
    angle = theta % 360;
    if (angle > 180 && angle < 270){
        x *= -1;
        y *= -1;
    } else {
        if (angle > 270) x *= -1 ;
        else if (angle > 90) y *= -1;
    }
    outp[0] = x;
    outp[1] = y;
    outp[2] = 0.0;

    return 1;
}

float calc_theta(float* p) { // assuming the centerpt = 0,0
    float x = p[0];
    float y = p[1];

    float angle = atanf(abs(x/y)) * (180 / pi);

    if (x < 0 && y < 0) angle += 180; 
    else {
        if (y < 0) angle += 90;
        else if (x < 0) angle += 270;
    }
    
    return angle;
}