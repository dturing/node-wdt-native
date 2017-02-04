//
// Created by anton on 21.03.15.
//

#include "cvAverage.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

Nan::Persistent<FunctionTemplate> cvAverage::constructor;

cv::Mat mat;

void
cvAverage::Init(Local<Object> target) {
    Nan::HandleScope scope;

    //Class
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(cvAverage::New);
    constructor.Reset( ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("cvAverage").ToLocalChecked());

    // Prototype
    Nan::SetPrototypeMethod(ctor, "process", Process);

    target->Set(Nan::New("cvAverage").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(cvAverage::New) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(info.Holder());
}


cvAverage::cvAverage(): Nan::ObjectWrap() {
    mat = cv::Mat::zeros(1, 1, CV_32FC3);
}

NAN_METHOD(cvAverage::Process) {
        Nan::HandleScope scope;

        Matrix *src = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
        cv::Mat srcMat = cv::Mat::zeros(src->mat.size(), CV_8UC3);
        src->mat.convertTo(srcMat, CV_8UC3);

        if (src->mat.size() != mat.size()) {
            mat = cv::Mat::zeros(src->mat.size(), CV_32FC3);
        }

        cv::accumulateWeighted(srcMat, mat, (double)info[1]->NumberValue());

        mat.convertTo(src->mat, CV_8UC3);

        info.GetReturnValue().Set(Nan::Null());
}
