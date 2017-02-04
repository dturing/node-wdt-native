//
// Created by anton on 21.03.15.
//

#ifndef _OPENCV_WEXTREMES_H_
#define _OPENCV_WEXTREMES_H_


#include "OpenCV.h"

class wExtremes : public Nan::ObjectWrap {
public:

    static Nan::Persistent<FunctionTemplate> constructor;
    static void Init(Local<Object> target);
    static NAN_METHOD(New);
    wExtremes();

    JSFUNC(Process);

};


#endif //_OPENCV_WEXTREMES_H_
