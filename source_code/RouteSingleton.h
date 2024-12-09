#ifndef ROUTE_SINGLETON_H
#define ROUTE_SINGLETON_H

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

#include "MainApp.h"
#include "View.h"

using ContainerCreator = std::function<std::unique_ptr<View>(MainApp*)>;

class RouteSingleton {
public:
    static void addRoute(
        std::string,
        ContainerCreator
    );
    static std::optional<ContainerCreator> getRoute(
        std::string
    );
private:
    RouteSingleton();
    inline static std::unordered_map<
        std::string,
        ContainerCreator
    > routeMap;
};

#endif // ROUTE_SINGLETON_H
