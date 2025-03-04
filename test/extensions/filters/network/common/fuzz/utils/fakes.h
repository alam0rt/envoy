#pragma once

#include "test/mocks/server/factory_context.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class FakeListenerInfo : public Network::ListenerInfo {
public:
  explicit FakeListenerInfo(const envoy::config::listener::v3::Listener& config)
      : metadata_(config.metadata()), direction_(config.traffic_direction()),
        is_quic_(config.udp_listener_config().has_quic_options()),
        bypass_overload_manager_(config.bypass_overload_manager()) {}
  FakeListenerInfo() = default;
  const envoy::config::core::v3::Metadata& metadata() const override {
    return metadata_.proto_metadata_;
  }
  const Envoy::Config::TypedMetadata& typedMetadata() const override {
    return metadata_.typed_metadata_;
  }
  envoy::config::core::v3::TrafficDirection direction() const override { return direction_; }
  bool isQuic() const override { return is_quic_; }
  bool shouldBypassOverloadManager() const override { return bypass_overload_manager_; }

private:
  Envoy::Config::MetadataPack<Envoy::Network::ListenerTypedMetadataFactory> metadata_;
  envoy::config::core::v3::TrafficDirection direction_ = envoy::config::core::v3::UNSPECIFIED;
  const bool is_quic_ = false;
  const bool bypass_overload_manager_ = false;
};

class FakeFactoryContext : public MockFactoryContext {
public:
  void prepareSimulatedSystemTime() {
    api_ = Api::createApiForTest(time_system_);
    dispatcher_ = api_->allocateDispatcher("test_thread");
  }
  AccessLog::AccessLogManager& accessLogManager() override { return access_log_manager_; }
  Upstream::ClusterManager& clusterManager() override { return cluster_manager_; }
  Event::Dispatcher& mainThreadDispatcher() override { return *dispatcher_; }
  const Network::DrainDecision& drainDecision() override { return drain_manager_; }
  Init::Manager& initManager() override { return init_manager_; }
  ServerLifecycleNotifier& lifecycleNotifier() override { return lifecycle_notifier_; }
  const LocalInfo::LocalInfo& localInfo() const override { return local_info_; }
  Envoy::Runtime::Loader& runtime() override { return runtime_loader_; }
  Stats::Scope& scope() override { return scope_; }
  Singleton::Manager& singletonManager() override { return *singleton_manager_; }
  ThreadLocal::Instance& threadLocal() override { return thread_local_; }
  Server::Admin& admin() override { return admin_; }
  Stats::Scope& listenerScope() override { return listener_scope_; }
  Api::Api& api() override { return *api_; }
  TimeSource& timeSource() override { return time_system_; }
  OverloadManager& overloadManager() override { return overload_manager_; }
  ProtobufMessage::ValidationContext& messageValidationContext() override {
    return validation_context_;
  }
  ProtobufMessage::ValidationVisitor& messageValidationVisitor() override {
    return ProtobufMessage::getStrictValidationVisitor();
  }
  Event::SimulatedTimeSystem& simulatedTimeSystem() {
    return dynamic_cast<Event::SimulatedTimeSystem&>(time_system_);
  }
  Event::TestTimeSystem& timeSystem() { return time_system_; }
  Grpc::Context& grpcContext() override { return grpc_context_; }
  Http::Context& httpContext() override { return http_context_; }

  Event::DispatcherPtr dispatcher_;
  Event::SimulatedTimeSystem time_system_;
  Api::ApiPtr api_;
};

} // namespace Configuration
} // namespace Server
} // namespace Envoy
