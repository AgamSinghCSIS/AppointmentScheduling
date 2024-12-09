#ifndef SETUP_ROUTE_H
#define SETUP_ROUTE_H

#include <functional>
#include <memory>
#include <utility>

#include "MainApp.h"
#include "RouteSingleton.h"

template<typename WidgetClass>
auto makeWidgetInstantiator() {
    return [](MainApp *a) {
        return std::make_unique<WidgetClass>(a);
    };
}

template<typename WidgetClass>
void addWidget(std::string route) {
    RouteSingleton::addRoute(
        route,
        makeWidgetInstantiator<WidgetClass>()
    );
}

#define SETUP_ROUTE(__ROUTE_, __CLASS_NAME_) \
    namespace { \
        static struct RouteAdder ## __CLASS_NAME_ { \
            RouteAdder ## __CLASS_NAME_ () { \
                addWidget<__CLASS_NAME_>(__ROUTE_); \
            } \
        } RouteAdder ## __CLASS_NAME_ ## Instance; \
    }

#endif // SETUP_ROUTE_H
