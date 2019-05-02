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

  ENVOY_LOG(warn, "Headers size before:\n{}", headers.size());
  ENVOY_LOG(warn, "Headers before:\n{}", headers);
  //add a header
  headers.addCopy(headerKey(), headerValue());
  ENVOY_LOG(warn, "Headers size after:\n{}", headers.size());
  ENVOY_LOG(warn, "Headers after:\n{}", headers);

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
