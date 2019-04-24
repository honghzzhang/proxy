#include <string>

#include "src/envoy/http/authproxy/http_filter.h"
#include "envoy/config/filter/http/authproxy/v2alpha1/config.pb.h"
#include "common/config/json_utility.h"
#include "envoy/registry/registry.h"

using istio::envoy::config::filter::http::authproxy::v2alpha1::FilterConfig;

namespace Envoy {
namespace Server {
namespace Configuration {

class AuthProxyFilterConfig : public NamedHttpFilterConfigFactory,
                              public Logger::Loggable<Logger::Id::filter> {
public:
  Http::FilterFactoryCb createFilterFactory(const Json::Object& json_config, 
                                            const std::string&,
                                            FactoryContext& context) override {
    ENVOY_LOG(debug, "Called AuthProxyFilterConfig : {}", __func__);
    FilterConfig filter_config;
    translateAuthProxyFilter(json_config, filter_config);
    return createFilter(filter_config, context);
  }

  Http::FilterFactoryCb createFilterFactoryFromProto(
      const Protobuf::Message& filter_config, const std::string&,
      FactoryContext& context) override {
    return createFilter(
        Envoy::MessageUtil::downcastAndValidate<const FilterConfig&>(filter_config), context);
  }

  /**
   *  Return the Protobuf Message that represents your config incase you have config proto
   */
  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new FilterConfig()};
  }

  std::string name() override { 
      return "AuthProxy"; 
  }

private:
  Http::FilterFactoryCb createFilter(const FilterConfig& filter_config, FactoryContext&) {
    ENVOY_LOG(debug, "Called AuthProxyFilterConfig : {}", __func__);
    Http::AuthProxyFilterConfigSharedPtr config =
        std::make_shared<Http::AuthProxyFilterConfig>(
            Http::AuthProxyFilterConfig(filter_config));

    return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      auto filter = new Http::AuthProxyFilter(config);
      callbacks.addStreamDecoderFilter(Http::StreamDecoderFilterSharedPtr{filter});
    };
  }

  void translateAuthProxyFilter(const Json::Object& json_config,
                                FilterConfig& filter_config) {
    // normally we want to validate the json_config againts a defined json-schema here.
    JSON_UTIL_SET_STRING(json_config, filter_config, key);
    JSON_UTIL_SET_STRING(json_config, filter_config, val);
  }
};

/**
 * Static registration for the AuthProxy filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<AuthProxyFilterConfig, NamedHttpFilterConfigFactory>
    register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
