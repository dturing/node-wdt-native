//
// Created by Anton on 28/04/15.
//

#ifndef OPENCV_CVDISPLAYFORMAT_H
#define OPENCV_CVDISPLAYFORMAT_H


#include "OpenCV.h"

class cvDisplayFormat : public Nan::ObjectWrap {
public:

    static Nan::Persistent<FunctionTemplate> constructor;
    static void Init(Local<Object> target);
    static NAN_METHOD(New);
    cvDisplayFormat();

    JSFUNC(Reset);
    JSFUNC(Process);

};


#endif //OPENCV_CVDISPLAYFORMAT_H
