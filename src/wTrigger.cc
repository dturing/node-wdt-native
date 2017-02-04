//
// Created by anton on 21.03.15.
//

#include "wTrigger.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

Nan::Persistent<FunctionTemplate> wTrigger::constructor;

void
wTrigger::Init(Local<Object> target) {
    Nan::HandleScope scope;

    //Class
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(wTrigger::New);
    constructor.Reset( ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("wTrigger").ToLocalChecked());

    // Prototype
    Nan::SetPrototypeMethod(ctor, "process", Process);

    target->Set(Nan::New("wTrigger").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(wTrigger::New) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(info.Holder());
}


wTrigger::wTrigger(): Nan::ObjectWrap() {

}

NAN_METHOD(wTrigger::Process) {
        Nan::HandleScope scope;

        Matrix *src = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
        cv::Mat yuv;
        cv::cvtColor(src->mat, yuv, CV_RGB2YCrCb);

        v8::Local<v8::String> name = info[1]->ToString();

        float motion, presence;

        int sz = yuv.cols*yuv.rows;
        int sz4 = ( (yuv.cols/2)*(yuv.rows/2));
        unsigned char *Y = yuv.data;
        unsigned char *U = Y + sz;
        unsigned char *V = U + sz4;

        int x0 = (int)(info[2]->IntegerValue()* yuv.cols/2); // x
        int y0 = (int)(info[3]->IntegerValue() * yuv.rows/2); // y
        int x1 = (int)(info[4]->IntegerValue() * yuv.cols/2); // w
        int y1 = (int)(info[5]->IntegerValue() * yuv.rows/2); // h
        int s = x1*y1;
        int bpl = yuv.cols/2;

        float vacc=0,uacc=0;
        for( int cy=y0; cy<y0+y1; cy++ ) {
            for( int cx=x0; cx<x0+x1; cx++ ) {
                int p = (cy*bpl)+cx;
                if( abs(V[p]-128) > 16 ) vacc++;
                if( abs(U[p]-128) > 16 ) uacc++;
            }
        }
        vacc /= s;
        uacc /= s;
        motion = MAX(0,MIN(1,vacc));
        presence = MAX(0,MIN(1,uacc));

        v8::Local<v8::Array> arr = Nan::New<Array>(3);

        arr->Set(0, Nan::New<Number>(motion));
        arr->Set(1, Nan::New<Number>(presence));
        arr->Set(2, name);

        info.GetReturnValue().Set(arr);
}
