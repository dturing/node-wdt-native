//
// Created by anton on 21.03.15.
//

#ifndef _OPENCV_WTRIGGER_H_
#define _OPENCV_WTRIGGER_H_


#include "OpenCV.h"

class wTrigger : public Nan::ObjectWrap {
public:

    static Nan::Persistent<FunctionTemplate> constructor;
    static void Init(Local<Object> target);
    static NAN_METHOD(New);
    wTrigger();

    JSFUNC(Process);

};


#endif //_OPENCV_WTRIGGER_H_
