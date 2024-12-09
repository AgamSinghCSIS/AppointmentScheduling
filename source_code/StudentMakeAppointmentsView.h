#ifndef STUDENT_MAKE_APPOINTMENTS_VIEW
#define STUDENT_MAKE_APPOINTMENTS_VIEW

#include <chrono>
#include <map>

#include <Wt/WCalendar.h>
#include <Wt/WComboBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WTime.h>
#include <Wt/WTimeEdit.h>

#include "MainApp.h"
#include "View.h"

class StudentMakeAppointmentsView: public View{
    public:
        explicit StudentMakeAppointmentsView(MainApp*);

    private:

        using TimePoint = std::chrono::time_point<
            std::chrono::system_clock>;
        Wt::WTemplate   *templ;
        Wt::WComboBox   *advisor;
        Wt::WCalendar   *calendar;
        Wt::WText       *availability;
        Wt::WTimeEdit   *startApptTime;
        Wt::WTimeEdit   *endApptTime; 
        Wt::WLineEdit   *apptTitle;
        Wt::WLineEdit   *apptDescription;
        Wt::WText       *apptDetails;
        Wt::WPushButton *bookButton;
        Wt::WPushButton *backButton;
        int64_t          stuID;
        std::map<std::string,
            int64_t>     advisorMap;

        void addTemplate();
        void getStuID();
        void addAdvisorSelection();
        void addCalendar();
        void addAvailability();
        void addStartApptTime();
        void addEndApptTime();
        void addApptTitle();
        void addApptDescription();
        void addApptDetails();
        void addBookButton();
        void addBackButton();
        void updateAvailability();
        void updateApptDetails();
        bool isApptValid();
        void createAppointment();
};

#endif // STUDENT_MAKE_APPOINTMENTS_VIEW
