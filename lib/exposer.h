#pragma once

#include <atomic>
#include <cstdint>
#include <memory>

#include "CivetServer.h"
#include "registry.h"

namespace prometheus {

class MetricsHandler : public CivetHandler {
 public:
  MetricsHandler(const std::vector<std::weak_ptr<Collectable>>& collectables,
                 Registry& registry);

  bool handleGet(CivetServer* server, struct mg_connection* conn);

  const std::vector<std::weak_ptr<Collectable>>& collectables_;
  Family<Counter>* bytesTransferedFamily_;
  Counter* bytesTransfered_;
  Family<Counter>* numScrapesFamily_;
  Counter* numScrapes_;
};

class Exposer {
 public:
  Exposer(std::uint16_t port);
  void registerCollectable(const std::weak_ptr<Collectable>& collectable);

 private:
  CivetServer server_;
  std::vector<std::weak_ptr<Collectable>> collectables_;
  std::shared_ptr<Registry> exposerRegistry_;
  MetricsHandler metricsHandler_;
};
}