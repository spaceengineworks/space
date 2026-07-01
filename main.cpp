#include <Optikos.hpp>
#include <SERuntime.hpp>
#include <iostream>

#include "VulkanConfig.hpp"

int main() {
  Logger::add_logger();
  Optikos::Optikos app("space", 800, 480);
  SE::SERuntine runtime(SE::SeRender::Vulkan);
  if (app.getRenderer()->setUpConfig(runtime.SeAskConfig()))
    std::cout << "TRUE" << std::endl;
  else
    std::cout << "FALSE" << std::endl;

  auto data = (SharedVulkanConfig *)runtime.SeAskConfig();

  auto btn = std::make_unique<Optikos::Button>(120, 30, Optikos::Vec2(20, 20),
                                               "Click me");
  btn->setEvent([]() { std::cout << "Clicked!\n"; });
  app.addWidget(1, std::move(btn));

  while (!app.should_close()) {
    app.begin();

    std::cout << data->swapChainExtent->height << std::endl;

    app.end();
  }

  return 0;
}