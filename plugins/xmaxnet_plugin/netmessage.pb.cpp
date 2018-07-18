// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: netmessage.proto

#include "netmessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace google {
namespace protobuf {
class HelloMsgDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<HelloMsg>
      _instance;
} _HelloMsg_default_instance_;
}  // namespace protobuf
}  // namespace google
namespace protobuf_netmessage_2eproto {
void InitDefaultsHelloMsgImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::google::protobuf::_HelloMsg_default_instance_;
    new (ptr) ::google::protobuf::HelloMsg();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::google::protobuf::HelloMsg::InitAsDefaultInstance();
}

void InitDefaultsHelloMsg() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsHelloMsgImpl);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::google::protobuf::HelloMsg, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::google::protobuf::HelloMsg, msg_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::google::protobuf::HelloMsg)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::google::protobuf::_HelloMsg_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "netmessage.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\020netmessage.proto\022\017google.protobuf\"\027\n\010H"
      "elloMsg\022\013\n\003msg\030\001 \001(\tb\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 68);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "netmessage.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_netmessage_2eproto
namespace google {
namespace protobuf {

// ===================================================================

void HelloMsg::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int HelloMsg::kMsgFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

HelloMsg::HelloMsg()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_netmessage_2eproto::InitDefaultsHelloMsg();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:google.protobuf.HelloMsg)
}
HelloMsg::HelloMsg(const HelloMsg& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  msg_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.msg().size() > 0) {
    msg_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.msg_);
  }
  // @@protoc_insertion_point(copy_constructor:google.protobuf.HelloMsg)
}

void HelloMsg::SharedCtor() {
  msg_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _cached_size_ = 0;
}

HelloMsg::~HelloMsg() {
  // @@protoc_insertion_point(destructor:google.protobuf.HelloMsg)
  SharedDtor();
}

void HelloMsg::SharedDtor() {
  msg_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void HelloMsg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* HelloMsg::descriptor() {
  ::protobuf_netmessage_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_netmessage_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const HelloMsg& HelloMsg::default_instance() {
  ::protobuf_netmessage_2eproto::InitDefaultsHelloMsg();
  return *internal_default_instance();
}

HelloMsg* HelloMsg::New(::google::protobuf::Arena* arena) const {
  HelloMsg* n = new HelloMsg;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void HelloMsg::Clear() {
// @@protoc_insertion_point(message_clear_start:google.protobuf.HelloMsg)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  msg_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  _internal_metadata_.Clear();
}

bool HelloMsg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:google.protobuf.HelloMsg)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // string msg = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_msg()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->msg().data(), static_cast<int>(this->msg().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "google.protobuf.HelloMsg.msg"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:google.protobuf.HelloMsg)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:google.protobuf.HelloMsg)
  return false;
#undef DO_
}

void HelloMsg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:google.protobuf.HelloMsg)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string msg = 1;
  if (this->msg().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->msg().data(), static_cast<int>(this->msg().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "google.protobuf.HelloMsg.msg");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->msg(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:google.protobuf.HelloMsg)
}

::google::protobuf::uint8* HelloMsg::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:google.protobuf.HelloMsg)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string msg = 1;
  if (this->msg().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->msg().data(), static_cast<int>(this->msg().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "google.protobuf.HelloMsg.msg");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->msg(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:google.protobuf.HelloMsg)
  return target;
}

size_t HelloMsg::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:google.protobuf.HelloMsg)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // string msg = 1;
  if (this->msg().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->msg());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void HelloMsg::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:google.protobuf.HelloMsg)
  GOOGLE_DCHECK_NE(&from, this);
  const HelloMsg* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const HelloMsg>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:google.protobuf.HelloMsg)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:google.protobuf.HelloMsg)
    MergeFrom(*source);
  }
}

void HelloMsg::MergeFrom(const HelloMsg& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:google.protobuf.HelloMsg)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.msg().size() > 0) {

    msg_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.msg_);
  }
}

void HelloMsg::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:google.protobuf.HelloMsg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void HelloMsg::CopyFrom(const HelloMsg& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:google.protobuf.HelloMsg)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HelloMsg::IsInitialized() const {
  return true;
}

void HelloMsg::Swap(HelloMsg* other) {
  if (other == this) return;
  InternalSwap(other);
}
void HelloMsg::InternalSwap(HelloMsg* other) {
  using std::swap;
  msg_.Swap(&other->msg_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata HelloMsg::GetMetadata() const {
  protobuf_netmessage_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_netmessage_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)