#ifndef FACULTY_UPLOAD_CSV_PAGE_H
#define FACULTY_UPLOAD_CSV_PAGE_H

#include <Wt/WFileUpload.h>
#include <Wt/WTemplate.h>

#include "Session.h"
#include "View.h"

class FacultyUploadCSVPage: public View {
public:
	explicit FacultyUploadCSVPage(MainApp*);
private:
    AASS::Session &session;
	Wt::WTemplate *templ;
	Wt::WFileUpload *uploadStudentsButton;
	Wt::WFileUpload *uploadScheduleButton;

	void addTemplate();
	void addUploadStudentsButton();
	void addUploadScheduleButton();

    void sendErrorToast(std::string);
};

#endif // FACULTY_UPLOAD_CSV_PAGE_H
