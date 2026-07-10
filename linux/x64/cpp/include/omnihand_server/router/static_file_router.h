#ifndef __STATIC_FILE_ROUTER_H
#define __STATIC_FILE_ROUTER_H
#include <filesystem>
#include "crow.h"
#include "crow/middlewares/cors.h"

namespace agilink {
namespace omnihand {
namespace server {
namespace transport {
void RegisterStaticFiles(crow::App<crow::CORSHandler>& app, const std::filesystem::path& local_path);
}  // namespace transport
}  // namespace server
}  // namespace omnihand
}  // namespace agilink
#endif