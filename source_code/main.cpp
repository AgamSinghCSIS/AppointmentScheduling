#include <csignal>
#include <memory>
#include <utility>

#include "AuthSeeder.h"
#include "DatabaseUtils.h"
#include "MainApp.h"
#include "RouteSingleton.h"
#include "TableCreatorSingleton.h"
#include "TermStub.h"
#include "Session.h"

int runMainApp(int argc, char **argv) {
    AASS::Session::configureAuth();
    TableCreatorSingleton::createTables([=] {
        return createDBBackend();
    });
    seedAuth();

    return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
        return std::make_unique<MainApp>(env);
    });
}

void kill_thread(int signal) {
    if (signal == SIGSEGV) {
        Wt::WApplication *app = Wt::WApplication::instance();
        if (app) {
            Wt::WApplication::instance()->quit();
        }
        else {
            std::terminate();
        }
    }
}

int main(int argc, char **argv) {
    std::signal(SIGSEGV, kill_thread);
    return runMainApp(argc, argv);
}
