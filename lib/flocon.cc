#ifndef BUILDING_NODE_EXTENSION
  #define BUILDING_NODE_EXTENSION
#endif // BUILDING_NODE_EXTENSION

#include "flocon.h"

#include <node.h>
#include <v8.h>

#include <stdio.h>  // printf
#include <unistd.h>
#include <chrono>   // std::chrono
#include <ctime>
#include <string>
#include "fast_int_to_string/fast_int_to_string.hpp"

using namespace v8;
using namespace std;

// using Clock = std::chrono::high_resolution_clock;
// using TimePoint = std::chrono::time_point<Clock>;

Flocon::Flocon() {};
Flocon::~Flocon() {};

void Flocon::Init(Local<Object> exports) {
  // Prepare constructor template
  Isolate* isolate = Isolate::GetCurrent();


  Local<FunctionTemplate> tplNew = FunctionTemplate::New(isolate, New);
  Local<FunctionTemplate> tplsnow = FunctionTemplate::New(isolate, Snow);

  v8::Local<v8::String> class_name = v8::String::NewFromUtf8(isolate, "Flocon").ToLocalChecked();
  v8::Local<v8::String> fn_name = v8::String::NewFromUtf8(isolate, "snow").ToLocalChecked();

  tplNew->SetClassName(class_name);
  tplNew->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tplNew->PrototypeTemplate()->Set(fn_name, tplsnow);

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Function> fn = tplNew->GetFunction(context).ToLocalChecked();
  fn->SetName(fn_name);
  exports->Set(context, class_name, fn);
}

void Flocon::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Flocon* obj = new Flocon();
  obj->_count = 0;
  obj->_current = 0;
  obj->_pid = (uint64_t)getpid() % 1024;

  struct tm then_tm;
  then_tm.tm_year = 2013-1900;
  then_tm.tm_mon = 0;
  then_tm.tm_mday = 1;
  then_tm.tm_hour = 8;
  then_tm.tm_min = 0;
  then_tm.tm_sec = 0;
  time_t timetThen = mktime(&then_tm);
  obj->_epoch = chrono::system_clock::from_time_t(timetThen/*1325376000*/); // January 1st, 2013
//  printf("%ld", timetThen);
  obj->Wrap(args.This());

  return args.GetReturnValue().Set(args.This());
}

void Flocon::Snow(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Flocon* obj = ObjectWrap::Unwrap<Flocon>(args.This());

  chrono::system_clock::time_point now = chrono::system_clock::now();
  auto diff = now - obj->_epoch;
  auto ms = chrono::duration_cast<chrono::milliseconds>(diff);
  uint64_t time = ms.count();

  if (obj->_count > 8191) {
    chrono::system_clock::time_point now_loop;
    uint64_t time_loop;

    while (true) {
      now_loop = chrono::system_clock::now();
      auto diff_loop = now_loop - obj->_epoch;
      auto ms_loop = chrono::duration_cast<chrono::milliseconds>(diff_loop);
      time_loop = ms_loop.count();

      if (time_loop > obj->_current) break;
    }
  }

  if (obj->_current < time) {
    obj->_current = time;
    obj->_count = 0;
  }
 
  uint64_t identifier = time << 23 | obj->_pid << 13 | (obj->_count++);

  char buffer[21];
  //snprintf(buffer, 21, "%llu", identifier);
  fast_uint64_to_string(identifier, buffer);
  return args.GetReturnValue().Set(String::NewFromOneByte(isolate, (uint8_t*)buffer, v8::NewStringType::kNormal).ToLocalChecked());
}
