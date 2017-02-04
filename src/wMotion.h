//
// Created by anton on 21.03.15.
//

#ifndef _OPENCV_WMOTION_H_
#define _OPENCV_WMOTION_H_

#include "OpenCV.h"

class wMotion : public Nan::ObjectWrap {
public:

    static Persistent<FunctionTemplate> constructor;
    static void Init(Local<Object> target);
    static NAN_METHOD(New);
    wMotion();

    JSFUNC(Reset);
    JSFUNC(Process);

};


#endif //_OPENCV_WMOTION_H_
