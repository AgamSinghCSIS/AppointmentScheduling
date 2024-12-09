#include <filesystem>
#include <memory>
#include <utility>

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WBootstrap5Theme.h>
#include <Wt/Dbo/backend/Sqlite3.h>

#include "HomePageView.h"
#include "MainApp.h"
#include "RouteSingleton.h"

MainApp::MainApp(const Wt::WEnvironment &env) :
    Wt::WApplication(env),
    session(std::make_unique<Wt::Dbo::backend::Sqlite3>("db.sqlite3"))
{
    setTitle("AASS");
    setTheme(std::make_unique<Wt::WBootstrap5Theme>());

    addMessageResourceBundles();

    internalPathChanged().connect(this, &MainApp::onInternalPathChange);

    content = root()->addWidget(std::make_unique<Wt::WContainerWidget>());
    showHomeScreen();
}

void MainApp::onInternalPathChange() {
    auto widgetCreator = RouteSingleton::getRoute(internalPath());
    content->clear();
    if (widgetCreator) {
        content->addWidget((*widgetCreator)(this));
    }
    else {
        setInternalPath("/", true);
    }
}

void MainApp::addMessageResourceBundles() {
    namespace fs = std::filesystem;

    fs::path templatePath = fs::path(appRoot()) / "templates";

    for (const auto &entry : fs::directory_iterator(templatePath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            messageResourceBundle().use(
                (templatePath / entry.path().stem()).string()
            );
        }
    }
}

void MainApp::showHomeScreen() {
    content->addWidget(
        (*RouteSingleton::getRoute("/"))(this)
    );
}

AASS::Session& MainApp::getSession() {
    return session;
}

