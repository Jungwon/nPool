#include "json.h"

// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// custom source
#include "utilities.h"

char* JSON::Stringify(Handle<Value> v8Handle)
{
    HandleScope scope;

    // get handle to stringify function
    Handle<Object> contextObject = Context::GetCurrent()->Global();
    Handle<Object> JSON = contextObject->Get(v8::String::New("JSON"))->ToObject();
    Handle<Function> Stringify = Handle<Function>::Cast(JSON->Get(v8::String::New("stringify")));

    // execute stringify
    Handle<Value> stringifyResult = Stringify->Call(JSON, 1, &(v8Handle));

    // only process if the result is valid
    char* returnString = NULL;
    if(stringifyResult->IsUndefined() == false)
    {
        // get reference and length of stringified result
        String::Utf8Value utf8ObjectString(Handle<String>::Cast(stringifyResult));
        const char* utf8String = Utilities::ToCString(utf8ObjectString);
        uint32_t utf8StringLength = strlen(utf8String);
        
        // copy the stringified result
        returnString = (char *)malloc(utf8StringLength + 1);
        memset(returnString, 0, utf8StringLength + 1);
        memcpy(returnString, *utf8ObjectString, utf8StringLength);
    }

    scope.Close(Undefined());

    return returnString;
}

Handle<Value> JSON::Parse(char* jsonObject)
{
    HandleScope scope;

    // get handle to parse function
    Handle<Object> contextObject = Context::GetCurrent()->Global();
    Handle<Object> JSON = contextObject->Get(v8::String::New("JSON"))->ToObject();
    Handle<Function> Parse = Handle<Function>::Cast(JSON->Get(v8::String::New("parse")));

    // execute parse
    Handle<Value> jsonString = String::New(jsonObject);
    Local<Value> v8Handle = Parse->Call(JSON, 1, &jsonString);

    return scope.Close(v8Handle);
}