/* This file is part of natlab.
 * https://github.com/mvasilkov/natlab
 * MIT+Ethics License | Copyright (c) 2022, 2023 Mark Vasilkov
 * See https://github.com/mvasilkov/natlab/blob/master/LICENSE
 */
#include <nan.h>

#include "thrust.c"

void SetState(const Nan::FunctionCallbackInfo<v8::Value> &args)
{
    if (args.Length() < 1 || !args[0]->IsBigInt())
    {
        Nan::ThrowTypeError("This function accepts a uint64 as a bigint");
        return;
    }

    v8::Isolate *isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    bool lossless = false;

    x = (uint64_t)args[0]->ToBigInt(context).ToLocalChecked()->Uint64Value(&lossless);

    if (!lossless)
    {
        Nan::ThrowTypeError("This function accepts a uint64 as a bigint");
        return;
    }

    x |= 1ULL;
}

void GetUint64(const Nan::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    v8::Local<v8::BigInt> result = v8::BigInt::NewFromUnsigned(isolate, next());

    args.GetReturnValue().Set(result);
}

void Init(v8::Local<v8::Object> exports)
{
    v8::Local<v8::Context> context = exports->GetCreationContext().ToLocalChecked();

    exports->Set(context, Nan::New("setState").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(SetState)->GetFunction(context).ToLocalChecked()).Check();
    exports->Set(context, Nan::New("getUint64").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(GetUint64)->GetFunction(context).ToLocalChecked()).Check();
}

NODE_MODULE(nativeThrust, Init)
