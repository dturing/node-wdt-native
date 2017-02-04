//
// Created by anton on 21.03.15.
//

#include "wExtremes.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

Nan::Persistent<FunctionTemplate> wExtremes::constructor;

void
wExtremes::Init(Local<Object> target) {
    Nan::HandleScope scope;

    //Class
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(wExtremes::New);
    constructor.Reset( ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("wExtremes").ToLocalChecked());

    // Prototype
    Nan::SetPrototypeMethod(ctor, "process", Process);

    target->Set(Nan::New("wExtremes").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(wExtremes::New) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(info.Holder());
}


wExtremes::wExtremes(): Nan::ObjectWrap() {

}

NAN_METHOD(wExtremes::Process) {
        Nan::HandleScope scope;

        Matrix *src = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
        cv::Mat gray;
        cv::cvtColor(src->mat, gray, CV_BGR2GRAY);

        unsigned char *data = gray.data;
        int w = gray.cols;
        int h = gray.rows;

        char finish=0;
        unsigned char threshold = 128;

        float left, right, top, bottom;

        for( int x=0; x<w && !finish; x++ ) {
            unsigned char *d = &data[ x ];
            for( int y=0; y<h-1 && !finish; y++ ) {
                d+=w;
                if( *d > threshold ) {
                    finish=1;
                    left = (float)x/w;
                }
            }
        }

        finish=0;
        for( int x=w-1; x>=0 && !finish; x-- ) {
            unsigned char *d = &data[ ((h-1)*w)+x ];
            for( int y=0; y<h-1 && !finish; y++ ) {
                d-=w;
                if( *d > threshold ) {
                    finish=1;
                    right = (float)x/w;
                }
            }
        }

        finish=0;
        for( int y=0; y<h-1 && !finish; y++ ) {
            unsigned char *d = &data[ (y*w) ];
            for( int x=0; x<w && !finish; x++ ) {
                d++;
                if( *d > threshold ) {
                    finish=1;
                    top = (float)y/h;
                }
            }
        }

        finish=0;
        for( int y=h-2; y>=0 && !finish; y-- ) {
            unsigned char *d = &data[ (y*w) ];
            for( int x=0; x<w && !finish; x++ ) {
                d++;
                if( *d > threshold ) {
                    finish=1;
                    bottom = (float)y/h;
                }
            }
        }

        v8::Local<v8::Array> arr = Nan::New<Array>(4);

        arr->Set(0, Nan::New<Number>(left));
        arr->Set(1, Nan::New<Number>(right));
        arr->Set(2, Nan::New<Number>(top));
        arr->Set(3, Nan::New<Number>(bottom));

        info.GetReturnValue().Set(arr);
}
