#include "RouteSingleton.h"
#include "View.h"

void RouteSingleton::addRoute(
    std::string routeName,
    ContainerCreator func
) {
    routeMap[routeName] = func;
}

std::optional<ContainerCreator> RouteSingleton::getRoute(
    std::string s
) {
    if (routeMap.find(s) != routeMap.end()) {
        // Some
        return routeMap[s];
    }
    // None
    return {};
}
