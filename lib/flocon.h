#ifndef FLOCON_H
#define FLOCON_H

#include <node.h>
#include <chrono>
#include <node_object_wrap.h>

using namespace std;

class Flocon : public node::ObjectWrap {
  public:
    static void Init(v8::Local<v8::Object> exports);

  private:
    Flocon();
    ~Flocon();

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Snow(const v8::FunctionCallbackInfo<v8::Value>& args);
    uint64_t _count;
    uint64_t _current;
    uint64_t _pid;
    chrono::system_clock::time_point _epoch;
};

#endif
