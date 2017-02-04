//
// Created by Anton on 24/04/15.
//

#ifndef OPENCV_WACCUMULATE_H
#define OPENCV_WACCUMULATE_H


#include "OpenCV.h"

class wAccumulate : public Nan::ObjectWrap {
public:

    static Nan::Persistent<FunctionTemplate> constructor;
    static void Init(Local<Object> target);
    static NAN_METHOD(New);
    wAccumulate();

    JSFUNC(Process);

};


#endif //OPENCV_WACCUMULATE_H
