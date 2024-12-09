#ifndef VIEW_H
#define VIEW_H

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>

#include "MainApp.h"

class View: public Wt::WContainerWidget {
public:
    View(MainApp*);
protected:
    MainApp *parent;
};

#endif

