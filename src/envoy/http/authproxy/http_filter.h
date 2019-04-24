#pragma once

#include <string>
#include "envoy/config/filter/http/authproxy/v2alpha1/config.pb.h"
#include "envoy/server/filter_config.h"
#include "common/common/logger.h"

namespace Envoy {
namespace Http {

class AuthProxyFilterConfig {
public:
  AuthProxyFilterConfig(const istio::envoy::config::filter::http::authproxy::v2alpha1::FilterConfig& filter_config);

  const std::string& key() const { return key_; }
  const std::string& val() const { return val_; }

private:
  const std::string key_;
  const std::string val_;
};

typedef std::shared_ptr<AuthProxyFilterConfig> AuthProxyFilterConfigSharedPtr;

class AuthProxyFilter : public StreamDecoderFilter,
                        public Logger::Loggable<Logger::Id::filter> {
public:
  AuthProxyFilter(AuthProxyFilterConfigSharedPtr);
  ~AuthProxyFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(HeaderMap&, bool) override;
  FilterDataStatus decodeData(Buffer::Instance&, bool) override;
  FilterTrailersStatus decodeTrailers(HeaderMap&) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks&) override;

private:
  const AuthProxyFilterConfigSharedPtr config_;
  StreamDecoderFilterCallbacks* decoder_callbacks_;

  const LowerCaseString headerKey() const;
  const std::string headerValue() const;
};

} // namespace Http
} // namespace Envoy
