//
// Created by Anton on 24/04/15.
//

#include "cvMultiplyS.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

Nan::Persistent<FunctionTemplate> cvMultiplyS::constructor;

void
cvMultiplyS::Init(Local<Object> target) {
    Nan::HandleScope scope;

    //Class
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(cvMultiplyS::New);
    constructor.Reset( ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("cvMultiplyS").ToLocalChecked());

    // Prototype
    Nan::SetPrototypeMethod(ctor, "process", Process);

    target->Set(Nan::New("cvMultiplyS").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(cvMultiplyS::New) {
        Nan::HandleScope scope;
        info.GetReturnValue().Set(info.Holder());
}


cvMultiplyS::cvMultiplyS(): Nan::ObjectWrap() {

}

NAN_METHOD(cvMultiplyS::Process) {
        Nan::HandleScope scope;

        Matrix *src = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
        src->mat.convertTo(src->mat, CV_8UC3, (float)info[1]->NumberValue(), (float)info[2]->NumberValue());

        info.GetReturnValue().Set(Nan::Null());
}

