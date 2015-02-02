// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: storage.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "storage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* Storage_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Storage_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_storage_2eproto() {
  protobuf_AddDesc_storage_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "storage.proto");
  GOOGLE_CHECK(file != NULL);
  Storage_descriptor_ = file->message_type(0);
  static const int Storage_offsets_[9] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, version_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, node_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, mnemonic_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, passphrase_protection_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, pin_failed_attempts_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, pin_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, language_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, label_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, imported_),
  };
  Storage_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Storage_descriptor_,
      Storage::default_instance_,
      Storage_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Storage, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Storage));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_storage_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Storage_descriptor_, &Storage::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_storage_2eproto() {
  delete Storage::default_instance_;
  delete Storage_reflection_;
}

void protobuf_AddDesc_storage_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf_AddDesc_types_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rstorage.proto\032\013types.proto\"\303\001\n\007Storage"
    "\022\017\n\007version\030\001 \002(\r\022\031\n\004node\030\002 \001(\0132\013.HDNode"
    "Type\022\020\n\010mnemonic\030\003 \001(\t\022\035\n\025passphrase_pro"
    "tection\030\004 \001(\010\022\033\n\023pin_failed_attempts\030\005 \001"
    "(\r\022\013\n\003pin\030\006 \001(\t\022\020\n\010language\030\007 \001(\t\022\r\n\005lab"
    "el\030\010 \001(\t\022\020\n\010imported\030\t \001(\010B0\n\037com.satosh"
    "ilabs.trezor.protobufB\rTrezorStorage", 276);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "storage.proto", &protobuf_RegisterTypes);
  Storage::default_instance_ = new Storage();
  Storage::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_storage_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_storage_2eproto {
  StaticDescriptorInitializer_storage_2eproto() {
    protobuf_AddDesc_storage_2eproto();
  }
} static_descriptor_initializer_storage_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Storage::kVersionFieldNumber;
const int Storage::kNodeFieldNumber;
const int Storage::kMnemonicFieldNumber;
const int Storage::kPassphraseProtectionFieldNumber;
const int Storage::kPinFailedAttemptsFieldNumber;
const int Storage::kPinFieldNumber;
const int Storage::kLanguageFieldNumber;
const int Storage::kLabelFieldNumber;
const int Storage::kImportedFieldNumber;
#endif  // !_MSC_VER

Storage::Storage()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Storage::InitAsDefaultInstance() {
  node_ = const_cast< ::HDNodeType*>(&::HDNodeType::default_instance());
}

Storage::Storage(const Storage& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Storage::SharedCtor() {
  _cached_size_ = 0;
  version_ = 0u;
  node_ = NULL;
  mnemonic_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  passphrase_protection_ = false;
  pin_failed_attempts_ = 0u;
  pin_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  language_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  label_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  imported_ = false;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Storage::~Storage() {
  SharedDtor();
}

void Storage::SharedDtor() {
  if (mnemonic_ != &::google::protobuf::internal::kEmptyString) {
    delete mnemonic_;
  }
  if (pin_ != &::google::protobuf::internal::kEmptyString) {
    delete pin_;
  }
  if (language_ != &::google::protobuf::internal::kEmptyString) {
    delete language_;
  }
  if (label_ != &::google::protobuf::internal::kEmptyString) {
    delete label_;
  }
  if (this != default_instance_) {
    delete node_;
  }
}

void Storage::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Storage::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Storage_descriptor_;
}

const Storage& Storage::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_storage_2eproto();
  return *default_instance_;
}

Storage* Storage::default_instance_ = NULL;

Storage* Storage::New() const {
  return new Storage;
}

void Storage::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    version_ = 0u;
    if (has_node()) {
      if (node_ != NULL) node_->::HDNodeType::Clear();
    }
    if (has_mnemonic()) {
      if (mnemonic_ != &::google::protobuf::internal::kEmptyString) {
        mnemonic_->clear();
      }
    }
    passphrase_protection_ = false;
    pin_failed_attempts_ = 0u;
    if (has_pin()) {
      if (pin_ != &::google::protobuf::internal::kEmptyString) {
        pin_->clear();
      }
    }
    if (has_language()) {
      if (language_ != &::google::protobuf::internal::kEmptyString) {
        language_->clear();
      }
    }
    if (has_label()) {
      if (label_ != &::google::protobuf::internal::kEmptyString) {
        label_->clear();
      }
    }
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    imported_ = false;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Storage::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 version = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &version_)));
          set_has_version();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_node;
        break;
      }

      // optional .HDNodeType node = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_node:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_node()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_mnemonic;
        break;
      }

      // optional string mnemonic = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_mnemonic:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_mnemonic()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->mnemonic().data(), this->mnemonic().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_passphrase_protection;
        break;
      }

      // optional bool passphrase_protection = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_passphrase_protection:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &passphrase_protection_)));
          set_has_passphrase_protection();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_pin_failed_attempts;
        break;
      }

      // optional uint32 pin_failed_attempts = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_pin_failed_attempts:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &pin_failed_attempts_)));
          set_has_pin_failed_attempts();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(50)) goto parse_pin;
        break;
      }

      // optional string pin = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_pin:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_pin()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->pin().data(), this->pin().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_language;
        break;
      }

      // optional string language = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_language:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_language()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->language().data(), this->language().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(66)) goto parse_label;
        break;
      }

      // optional string label = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_label:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_label()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->label().data(), this->label().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(72)) goto parse_imported;
        break;
      }

      // optional bool imported = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_imported:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &imported_)));
          set_has_imported();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void Storage::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 version = 1;
  if (has_version()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->version(), output);
  }

  // optional .HDNodeType node = 2;
  if (has_node()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->node(), output);
  }

  // optional string mnemonic = 3;
  if (has_mnemonic()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->mnemonic().data(), this->mnemonic().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->mnemonic(), output);
  }

  // optional bool passphrase_protection = 4;
  if (has_passphrase_protection()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(4, this->passphrase_protection(), output);
  }

  // optional uint32 pin_failed_attempts = 5;
  if (has_pin_failed_attempts()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(5, this->pin_failed_attempts(), output);
  }

  // optional string pin = 6;
  if (has_pin()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->pin().data(), this->pin().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      6, this->pin(), output);
  }

  // optional string language = 7;
  if (has_language()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->language().data(), this->language().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      7, this->language(), output);
  }

  // optional string label = 8;
  if (has_label()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->label().data(), this->label().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      8, this->label(), output);
  }

  // optional bool imported = 9;
  if (has_imported()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(9, this->imported(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Storage::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required uint32 version = 1;
  if (has_version()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->version(), target);
  }

  // optional .HDNodeType node = 2;
  if (has_node()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->node(), target);
  }

  // optional string mnemonic = 3;
  if (has_mnemonic()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->mnemonic().data(), this->mnemonic().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->mnemonic(), target);
  }

  // optional bool passphrase_protection = 4;
  if (has_passphrase_protection()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(4, this->passphrase_protection(), target);
  }

  // optional uint32 pin_failed_attempts = 5;
  if (has_pin_failed_attempts()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(5, this->pin_failed_attempts(), target);
  }

  // optional string pin = 6;
  if (has_pin()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->pin().data(), this->pin().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        6, this->pin(), target);
  }

  // optional string language = 7;
  if (has_language()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->language().data(), this->language().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        7, this->language(), target);
  }

  // optional string label = 8;
  if (has_label()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->label().data(), this->label().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        8, this->label(), target);
  }

  // optional bool imported = 9;
  if (has_imported()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(9, this->imported(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Storage::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 version = 1;
    if (has_version()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->version());
    }

    // optional .HDNodeType node = 2;
    if (has_node()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->node());
    }

    // optional string mnemonic = 3;
    if (has_mnemonic()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->mnemonic());
    }

    // optional bool passphrase_protection = 4;
    if (has_passphrase_protection()) {
      total_size += 1 + 1;
    }

    // optional uint32 pin_failed_attempts = 5;
    if (has_pin_failed_attempts()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->pin_failed_attempts());
    }

    // optional string pin = 6;
    if (has_pin()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->pin());
    }

    // optional string language = 7;
    if (has_language()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->language());
    }

    // optional string label = 8;
    if (has_label()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->label());
    }

  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // optional bool imported = 9;
    if (has_imported()) {
      total_size += 1 + 1;
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Storage::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Storage* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Storage*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Storage::MergeFrom(const Storage& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_version()) {
      set_version(from.version());
    }
    if (from.has_node()) {
      mutable_node()->::HDNodeType::MergeFrom(from.node());
    }
    if (from.has_mnemonic()) {
      set_mnemonic(from.mnemonic());
    }
    if (from.has_passphrase_protection()) {
      set_passphrase_protection(from.passphrase_protection());
    }
    if (from.has_pin_failed_attempts()) {
      set_pin_failed_attempts(from.pin_failed_attempts());
    }
    if (from.has_pin()) {
      set_pin(from.pin());
    }
    if (from.has_language()) {
      set_language(from.language());
    }
    if (from.has_label()) {
      set_label(from.label());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from.has_imported()) {
      set_imported(from.imported());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Storage::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Storage::CopyFrom(const Storage& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Storage::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  if (has_node()) {
    if (!this->node().IsInitialized()) return false;
  }
  return true;
}

void Storage::Swap(Storage* other) {
  if (other != this) {
    std::swap(version_, other->version_);
    std::swap(node_, other->node_);
    std::swap(mnemonic_, other->mnemonic_);
    std::swap(passphrase_protection_, other->passphrase_protection_);
    std::swap(pin_failed_attempts_, other->pin_failed_attempts_);
    std::swap(pin_, other->pin_);
    std::swap(language_, other->language_);
    std::swap(label_, other->label_);
    std::swap(imported_, other->imported_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Storage::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Storage_descriptor_;
  metadata.reflection = Storage_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)