//
// Created by anton on 21.03.15.
//

#ifndef _OPENCV_CVAVERAGE_H_
#define _OPENCV_CVAVERAGE_H_

#include "OpenCV.h"

class cvAverage : public Nan::ObjectWrap {
public:

    static Nan::Persistent<FunctionTemplate> constructor;
    static void Init(Local<Object> target);
    static NAN_METHOD(New);
    cvAverage();

    JSFUNC(Process);

};

#endif //_OPENCV_CVAVERAGE_H_
