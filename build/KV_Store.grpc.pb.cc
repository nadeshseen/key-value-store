// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: KV_Store.proto

#include "KV_Store.pb.h"
#include "KV_Store.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace kv_store {

static const char* KeyValueService_method_names[] = {
  "/kv_store.KeyValueService/GENERAL_KV",
  "/kv_store.KeyValueService/Server_Join",
};

std::unique_ptr< KeyValueService::Stub> KeyValueService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< KeyValueService::Stub> stub(new KeyValueService::Stub(channel, options));
  return stub;
}

KeyValueService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GENERAL_KV_(KeyValueService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Server_Join_(KeyValueService_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status KeyValueService::Stub::GENERAL_KV(::grpc::ClientContext* context, const ::kv_store::KV_Request& request, ::kv_store::KV_Reply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::kv_store::KV_Request, ::kv_store::KV_Reply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GENERAL_KV_, context, request, response);
}

void KeyValueService::Stub::async::GENERAL_KV(::grpc::ClientContext* context, const ::kv_store::KV_Request* request, ::kv_store::KV_Reply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::kv_store::KV_Request, ::kv_store::KV_Reply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GENERAL_KV_, context, request, response, std::move(f));
}

void KeyValueService::Stub::async::GENERAL_KV(::grpc::ClientContext* context, const ::kv_store::KV_Request* request, ::kv_store::KV_Reply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GENERAL_KV_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::kv_store::KV_Reply>* KeyValueService::Stub::PrepareAsyncGENERAL_KVRaw(::grpc::ClientContext* context, const ::kv_store::KV_Request& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::kv_store::KV_Reply, ::kv_store::KV_Request, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GENERAL_KV_, context, request);
}

::grpc::ClientAsyncResponseReader< ::kv_store::KV_Reply>* KeyValueService::Stub::AsyncGENERAL_KVRaw(::grpc::ClientContext* context, const ::kv_store::KV_Request& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGENERAL_KVRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status KeyValueService::Stub::Server_Join(::grpc::ClientContext* context, const ::kv_store::Join_Request& request, ::kv_store::KV_Reply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::kv_store::Join_Request, ::kv_store::KV_Reply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Server_Join_, context, request, response);
}

void KeyValueService::Stub::async::Server_Join(::grpc::ClientContext* context, const ::kv_store::Join_Request* request, ::kv_store::KV_Reply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::kv_store::Join_Request, ::kv_store::KV_Reply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Server_Join_, context, request, response, std::move(f));
}

void KeyValueService::Stub::async::Server_Join(::grpc::ClientContext* context, const ::kv_store::Join_Request* request, ::kv_store::KV_Reply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Server_Join_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::kv_store::KV_Reply>* KeyValueService::Stub::PrepareAsyncServer_JoinRaw(::grpc::ClientContext* context, const ::kv_store::Join_Request& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::kv_store::KV_Reply, ::kv_store::Join_Request, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Server_Join_, context, request);
}

::grpc::ClientAsyncResponseReader< ::kv_store::KV_Reply>* KeyValueService::Stub::AsyncServer_JoinRaw(::grpc::ClientContext* context, const ::kv_store::Join_Request& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncServer_JoinRaw(context, request, cq);
  result->StartCall();
  return result;
}

KeyValueService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      KeyValueService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< KeyValueService::Service, ::kv_store::KV_Request, ::kv_store::KV_Reply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](KeyValueService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::kv_store::KV_Request* req,
             ::kv_store::KV_Reply* resp) {
               return service->GENERAL_KV(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      KeyValueService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< KeyValueService::Service, ::kv_store::Join_Request, ::kv_store::KV_Reply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](KeyValueService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::kv_store::Join_Request* req,
             ::kv_store::KV_Reply* resp) {
               return service->Server_Join(ctx, req, resp);
             }, this)));
}

KeyValueService::Service::~Service() {
}

::grpc::Status KeyValueService::Service::GENERAL_KV(::grpc::ServerContext* context, const ::kv_store::KV_Request* request, ::kv_store::KV_Reply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status KeyValueService::Service::Server_Join(::grpc::ServerContext* context, const ::kv_store::Join_Request* request, ::kv_store::KV_Reply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace kv_store

