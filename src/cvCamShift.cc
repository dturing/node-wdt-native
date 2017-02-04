//
// Created by anton on 21.03.15.
//

#include "cvCamShift.h"

#include "OpenCV.h"
#include "Matrix.h"
#include <nan.h>

Nan::Persistent<FunctionTemplate> cvCamShift::constructor;

void
cvCamShift::Init(Local<Object> target) {
    Nan::HandleScope scope;

    //Class
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(cvCamShift::New);
    constructor.Reset( ctor);
    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("cvCamShift").ToLocalChecked());

    // Prototype
    Nan::SetPrototypeMethod(ctor, "process", Process);

    target->Set(Nan::New("cvCamShift").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(cvCamShift::New) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(info.Holder());
}


cvCamShift::cvCamShift(): Nan::ObjectWrap() {

}

NAN_METHOD(cvCamShift::Process) {
        Nan::HandleScope scope;

        static float lastangle = -10000;

        Matrix *src = Nan::ObjectWrap::Unwrap<Matrix>(info[0]->ToObject());
        cv::Mat image = cv::Mat::zeros(src->mat.size(), CV_8UC1 );
        cv::cvtColor(src->mat, image, CV_RGB2GRAY, 1);
        //src->mat.convertTo(image, CV_8UC1);

        //CvRect ROI;
        //GetROI( &ROI, image->width, image->height );
        cv::Rect window;
        window.x = window.y = 0;
        window.width = image.cols;
        window.height = image.rows;

        cv::RotatedRect box = cv::CamShift(image, window, cv::TermCriteria(CV_TERMCRIT_EPS|CV_TERMCRIT_ITER, 10, 1.0 ));

        cv::Point2f ptf[4];
        box.angle *= -1;
        box.points(ptf);
        cv::Point pt[4];
        for( int i=0; i<4; i++ ) {
            pt[i].x = (int)ptf[i].x;
            pt[i].y = (int)ptf[i].y;
        }

        float sh=90;
        if( lastangle != -10000 ) {
            while( box.angle < lastangle-sh ) box.angle+=sh*2;
            while( box.angle > lastangle+sh ) box.angle-=sh*2;
        }
        while( box.angle < 0 ) box.angle += 360;
        box.angle = fmod( box.angle, 360 );

        lastangle = box.angle;

        float max = sqrt((image.cols*image.cols)+(image.rows*image.rows));

        v8::Local<v8::Array> arr = Nan::New<Array>(5);

        arr->Set(0, Nan::New<Number>((box.center.x + window.x)/image.cols)); // x
        arr->Set(1, Nan::New<Number>((box.center.y + window.y)/image.rows)); // y
        arr->Set(2, Nan::New<Number>((float)box.size.width/max)); // width
        arr->Set(3, Nan::New<Number>((float)box.size.height/max)); // height
        arr->Set(4, Nan::New<Number>(box.angle)); // angle

        info.GetReturnValue().Set(arr);
}
