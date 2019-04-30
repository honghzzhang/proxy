#include <string>

#include "src/envoy/http/authproxy/http_filter.h"
#include "envoy/config/filter/http/authproxy/v2alpha1/config.pb.h"
#include "envoy/server/filter_config.h"

using istio::envoy::config::filter::http::authproxy::v2alpha1::FilterConfig;

namespace Envoy {
namespace Http {

AuthProxyFilterConfig::AuthProxyFilterConfig(
    const FilterConfig& filter_config)
    : key_(filter_config.key()), val_(filter_config.val()) {}

AuthProxyFilter::AuthProxyFilter(AuthProxyFilterConfigSharedPtr config)
    : config_(config) {}

AuthProxyFilter::~AuthProxyFilter() {}

void AuthProxyFilter::onDestroy() {
  ENVOY_LOG(warn, "Called AuthProxyFilter : {}", __func__);
}

const LowerCaseString AuthProxyFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string AuthProxyFilter::headerValue() const {
  return config_->val();
}

FilterHeadersStatus AuthProxyFilter::decodeHeaders(HeaderMap& headers, bool) {
  ENVOY_LOG(warn, "AuthProxyFilter::decodeHeaders with config key: {}, val: {}\n",
            config_->key(), config_->val());

  //ENVOY_LOG(warn, "Headers:\n{}", headers);
  //add a header
  headers.addCopy(headerKey(), headerValue());
  //ENVOY_LOG(warn, "Headers after:\n{}", headers);

  /*
  headers.iterate(
      [](const Http::HeaderEntry& header,
         void* context) -> Http::HeaderMap::Iterate {
        Context* ctx = static_cast<Context*>(context);
        auto key = std::string(header.key().getStringView());
        auto value = std::string(header.value().getStringView());
        if (headerKey() == key && headerValue() == value) {
             ENVOY_LOG(warn, "Found matched key/value\n{}{}", key, value);
        }
        return Http::HeaderMap::Iterate::Continue;
      },
      &ctx);
  */
  return FilterHeadersStatus::Continue;
}

FilterDataStatus AuthProxyFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

FilterTrailersStatus AuthProxyFilter::decodeTrailers(HeaderMap&) {
  return FilterTrailersStatus::Continue;
}

void AuthProxyFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy
