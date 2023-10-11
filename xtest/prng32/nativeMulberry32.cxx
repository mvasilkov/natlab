/* This file is part of natlab.
 * https://github.com/mvasilkov/natlab
 * MIT+Ethics License | Copyright (c) 2022, 2023 Mark Vasilkov
 * See https://github.com/mvasilkov/natlab/blob/master/LICENSE
 */
#include <nan.h>

#include "mulberry32.c"

void SetState(const Nan::FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() < 1 || !args[0]->IsNumber())
    {
        Nan::ThrowTypeError("This function accepts a number");
        return;
    }

    v8::Isolate *isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    x = (uint32_t)args[0]->NumberValue(context).FromMaybe(0);
}

void GetUint32(const Nan::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Local<v8::Number> result = Nan::New<v8::Number>(next());

    args.GetReturnValue().Set(result);
}

void Init(v8::Local<v8::Object> exports)
{
    v8::Local<v8::Context> context = exports->GetCreationContext().ToLocalChecked();

    exports->Set(context, Nan::New("setState").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(SetState)->GetFunction(context).ToLocalChecked()).Check();
    exports->Set(context, Nan::New("getUint32").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(GetUint32)->GetFunction(context).ToLocalChecked()).Check();
}

NODE_MODULE(nativeMulberry32, Init)
