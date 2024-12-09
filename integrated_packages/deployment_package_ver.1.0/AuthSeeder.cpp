#include <memory>
#include <utility>

#include <Wt/Auth/HashFunction.h>
#include <Wt/Auth/Identity.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/WDate.h>
#include <Wt/WDateTime.h>
#include <Wt/WTime.h>

#include "Appointment.h"
#include "AppointmentBuilder.h"
#include "AdvisorPairing.h"
#include "AuthSeeder.h"
#include "DatabaseUtils.h"
#include "Profile.h"
#include "ProfileBuilder.h"
#include "Session.h"
#include "Term.h"
#include "TermBuilder.h"
#include "TermQueryDummy.h"
#include "TimeUtils.h"

namespace dbo = Wt::Dbo;

void seedAdminUsers(AASS::Session &session) {
    auto withUserDB = [&](std::string email) {
        dbo::Transaction trans(session);        

        auto &users = dynamic_cast<UserDatabase&>(session.getUsers());

        Wt::Auth::User userLookup = users.findWithIdentity(
            Wt::Auth::Identity::LoginName,
            email
        );

        if (userLookup.isValid()) {
            std::cerr << "Admin user " << email << " already exists!\n";
            return;
        }

        Wt::Auth::User user = users.registerNew();
        user.addIdentity(Wt::Auth::Identity::LoginName, email);
        user.setEmail(email);
        session.getPasswordAuthService().updatePassword(
            user,
            "password"
        );

        dbo::ptr<AuthInfo> authInfo = users.find(user);

        dbo::ptr<Profile> profile = session.add(
            ProfileBuilder()
                .withFirstName("Admin")
                .withLastName("User")
                .withRole(UserRole::ADMIN)
                .buildUnique()
        );
        authInfo.modify()->setUser(profile);
    };
    withUserDB("admin@una.edu");
}

void seedFacultyUsers(AASS::Session &session) {
    auto withUserDB = [&](std::string email) {
        dbo::Transaction trans(session);

        auto &users = dynamic_cast<UserDatabase&>(session.getUsers());

        Wt::Auth::User userLookup = users.findWithIdentity(
            Wt::Auth::Identity::LoginName,
            email
        );

        if (userLookup.isValid()) {
            std::cerr << "Faculty user " << email << " already exists!\n";
            return;
        }

        Wt::Auth::User user = users.registerNew();
        user.addIdentity(Wt::Auth::Identity::LoginName, email);
        user.setEmail(email);
        session.getPasswordAuthService().updatePassword(
            user,
            "password"
        );

        dbo::ptr<AuthInfo> authInfo = users.find(user);

        dbo::ptr<Profile> profile = session.add(
            ProfileBuilder()
                .withFirstName("Faculty")
                .withLastName("User")
                .withRole(UserRole::FACULTY)
                .buildUnique()
        );
        authInfo.modify()->setUser(profile);
    };
    withUserDB("faculty@una.edu");
    withUserDB("faculty1@una.edu");
    withUserDB("faculty2@una.edu");
}

void seedStudentUsers(AASS::Session &session) {
    auto withUserDB = [&](std::string email) {
        dbo::Transaction trans(session);

        auto &users = dynamic_cast<UserDatabase&>(session.getUsers());

        Wt::Auth::User userLookup = users.findWithIdentity(
            Wt::Auth::Identity::LoginName,
            email
        );

        if (userLookup.isValid()) {
            std::cerr << "Student user " << email << " already exists!\n";
            return;
        }

        Wt::Auth::User user = users.registerNew();
        user.addIdentity(Wt::Auth::Identity::LoginName, email);
        user.setEmail(email);
        session.getPasswordAuthService().updatePassword(
            user,
            "password"
        );

        dbo::ptr<AuthInfo> authInfo = users.find(user);

        dbo::ptr<Profile> profile = session.add(
            ProfileBuilder()
                .withFirstName("Student")
                .withLastName("User")
                .withRole(UserRole::STUDENT)
                .buildUnique()
        );
        authInfo.modify()->setUser(profile);
    };
    withUserDB("student@una.edu");
    withUserDB("student1@una.edu");
    withUserDB("student2@una.edu");
}

void seedTerms(AASS::Session &session) {
    [&]{
        dbo::Transaction trans(session);

        dbo::collection<dbo::ptr<Term>> terms = session
            .find<Term>()
            .where("id=?")
            .bind(1)
            ;

        if (terms.size() > 0) {
            std::cerr << "Term 1 already created!\n";
            return;
        }

        TermQueryDummy termQuery(session);

        auto termQueryPtr = *(termQuery.findById(1));

        dbo::ptr<Term> ptr = session.add(
            termQueryPtr
        );
    }();
    [&]{
        dbo::Transaction trans(session);

        dbo::collection<dbo::ptr<Term>> terms = session
            .find<Term>()
            .where("id = ?")
            .bind(2)
            ;
        if (terms.size() > 0) {
            std::cerr << "Term 2 already created!\n";
            return;
        }

        dbo::ptr<Term> ptr = session.add(
            TermBuilder()
                .withName("Fall 2023")
                .withStartTime(
                    Wt::WDateTime(
                        Wt::WDate(2023, 8, 16),
                        Wt::WTime(0, 0, 0)
                    ).toTimePoint()
                )
                .withEndTime(
                    Wt::WDateTime(
                        Wt::WDate(2023, 12, 11),
                        Wt::WTime(0, 0, 0)
                    ).toTimePoint()
                )
                .buildUnique()
        );
    }();
    [&]{
        dbo::Transaction trans(session);

        dbo::collection<dbo::ptr<Term>> terms = session
            .find<Term>()
            .where("id = ?")
            .bind(3)
            ;
        if (terms.size() > 0) {
            std::cerr << "Term 3 already created!\n";
            return;
        }

        dbo::ptr<Term> ptr = session.add(
            TermBuilder()
                .withName("Spring 2023")
                .withStartTime(
                    Wt::WDateTime(
                        Wt::WDate(2023, 1, 11),
                        Wt::WTime(0, 0, 0)
                    ).toTimePoint()
                )
                .withEndTime(
                    Wt::WDateTime(
                        Wt::WDate(2023, 5, 12),
                        Wt::WTime(0, 0, 0)
                    ).toTimePoint()
                )
                .buildUnique()
        );
    }();
    [&]{
        dbo::Transaction trans(session);

        dbo::collection<dbo::ptr<Term>> terms = session
            .find<Term>()
            .where("id = ?")
            .bind(4)
            ;
        if (terms.size() > 0) {
            std::cerr << "Term 4 already created!\n";
            return;
        }

        dbo::ptr<Term> ptr = session.add(
            TermBuilder()
                .withName("Fall 2024")
                .withStartTime(
                    Wt::WDateTime(
                        Wt::WDate(2024, 8, 15),
                        Wt::WTime(0, 0, 0)
                    ).toTimePoint()
                )
                .withEndTime(
                    Wt::WDateTime(
                        Wt::WDate(2024, 12, 16),
                        Wt::WTime(0, 0, 0)
                    ).toTimePoint()
                )
                .buildUnique()
        );
    }();
}

void seedAdvisorPairings(AASS::Session &session) {
    auto fromEmails = [&](std::string sEmail, std::string fEmail) {
        dbo::Transaction trans(session);

        UserDatabase &users = session.getUserDB();

        auto studentUser = users.findWithIdentity(
            Wt::Auth::Identity::LoginName,
            sEmail
            );
        if (!studentUser.isValid()) {
            std::cerr << "Student " << sEmail << " does not exist!\n";
            return;
        }
        int64_t studentID = users.find(studentUser).id();

        auto facultyUser = users.findWithIdentity(
            Wt::Auth::Identity::LoginName,
            fEmail
        );
        if (!facultyUser.isValid()) {
            std::cerr << "Faculty " << fEmail << " does not exist!\n";
            return;
        }
        int64_t facultyID = users.find(facultyUser).id();

        auto ap = std::make_unique<AdvisorPairing>();
        ap->setStudentID(studentID);
        ap->setAdvisorID(facultyID);
        ap->setTermID(1);
        
        dbo::ptr<AdvisorPairing> ptr = session.add(std::move(ap));
    };

    fromEmails("student1@una.edu", "faculty@una.edu");
    fromEmails("student2@una.edu", "faculty@una.edu");
    fromEmails("student1@una.edu", "faculty1@una.edu");
    fromEmails("student2@una.edu", "faculty1@una.edu");
    fromEmails("student1@una.edu", "faculty2@una.edu");
}

void seedAppointments(AASS::Session &session) {
    auto fromEmails = [&](std::string sEmail, std::string fEmail) {
        dbo::Transaction trans(session);

        UserDatabase &users = session.getUserDB();

        auto studentUser = users.findWithIdentity(
                Wt::Auth::Identity::LoginName,
            sEmail
            );
        if (!studentUser.isValid()) {
            std::cerr << "Student " << sEmail << " does not exist!\n";
            return;
        }
        int64_t studentID = users.find(studentUser).id();

        auto facultyUser = users.findWithIdentity(
            Wt::Auth::Identity::LoginName,
            fEmail
        );
        if (!facultyUser.isValid()) {
            std::cerr << "Faculty " << fEmail << " does not exist!\n";
            return;
        }
        int64_t facultyID = users.find(facultyUser).id();

        using Appts = dbo::collection<dbo::ptr<Appointment>>;

        Appts appts = session
            .find<Appointment>()
            .where("studentID = ?")
            .bind(studentID)
            .where("facultyID = ?")
            .bind(facultyID)
            ;

        if (appts.size() > 0) {
            std::cerr << "Appointment for " << sEmail << " and " << fEmail
                      << " already exists!\n"
                      ;
        }

        auto a = std::make_unique<Appointment>();
        a->setStudentID(studentID);
        a->setFacultyID(facultyID);
        a->setStartTime(timePointFromStr("2024-01-15 08:00:00"));
        a->setEndTime(timePointFromStr("2024-01-15 09:00:00"));
        a->setTitle("Course scheduling.");
        
        dbo::ptr<Appointment> ptr = session.add(std::move(a));
    };

    fromEmails("student1@una.edu", "faculty@una.edu");
    fromEmails("student2@una.edu", "faculty@una.edu");
    fromEmails("student1@una.edu", "faculty1@una.edu");
    fromEmails("student2@una.edu", "faculty1@una.edu");
    fromEmails("student1@una.edu", "faculty2@una.edu");
}

void seedAuth() {
    AASS::Session session(createDBBackend());
    seedAdminUsers(session);
    seedFacultyUsers(session);
    seedStudentUsers(session);
    seedTerms(session);
    seedAdvisorPairings(session);
    seedAppointments(session);
}

