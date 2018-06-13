

#define declear_jsvm_bind(clsname)\
	clsname* New##clsname##Function(const FunctionCallbackInfo<v8::Value>& args);\
	void clsname##WeakExternalReferenceCallback(Persistent<v8::Value>, void* parameter);\
	void clsname##FunctionInvocationCallback(const FunctionCallbackInfo<v8::Value>& args);\
	void Setup##clsname##ObjectToJs(Isolate* isolate,Handle<ObjectTemplate> global)


#define implement_jsvm_bind(clsname)\
	clsname* New##clsname##Function(const FunctionCallbackInfo<v8::Value>&  args)\
	{\
		if (args.Length() == 2)\
		{\
			Local<Value> v1 = args[0];\
			Local<Value> v2 = args[1];\
			return new clsname(v1->Int32Value(), v2->Int32Value());\
		}\
		else\
		{\
			return new clsname();\
		}\
	}\
	\
	void clsname##WeakExternalReferenceCallback(const WeakCallbackInfo<clsname>& data)\
	{\
		if (clsname* cpp_object = data.GetParameter())\
		{\
			delete cpp_object;\
		}\
	}\
	\
	void clsname##FunctionInvocationCallback(const FunctionCallbackInfo<v8::Value>& args)\
	{\
		if (!args.IsConstructCall())\
			return args.GetReturnValue().Set(Undefined(args.GetIsolate()));\
		clsname* cpp_object = New##clsname##Function(args);\
		if (!cpp_object)\
			return;\
		Handle<Object> object = args.This();\
		Local<External>  ee = External::New(args.GetIsolate(), cpp_object);\
		object->SetInternalField(0, ee);\
		Persistent<External> ret = Persistent<External>(args.GetIsolate(), ee);\
		ret.SetWeak(cpp_object, clsname##WeakExternalReferenceCallback, WeakCallbackType::kParameter);\
		args.GetReturnValue().Set(Undefined(args.GetIsolate()));\
	}\
	\
	void Setup##clsname##ObjectToJs(Isolate* isolate, Handle<ObjectTemplate> global)\
	{\
		Handle<FunctionTemplate> obj_templ = FunctionTemplate::New(isolate, & clsname##FunctionInvocationCallback);\
		obj_templ->SetClassName(String::NewFromUtf8(\
			isolate,\
			#clsname,\
			NewStringType::kNormal).ToLocalChecked());\
		obj_templ->InstanceTemplate()->SetInternalFieldCount(1);\
		Handle<ObjectTemplate> obj_proto = obj_templ->PrototypeTemplate();\
		global->Set(String::NewFromUtf8(\
			isolate,\
			#clsname,\
			NewStringType::kNormal).ToLocalChecked(), obj_templ);\
	}