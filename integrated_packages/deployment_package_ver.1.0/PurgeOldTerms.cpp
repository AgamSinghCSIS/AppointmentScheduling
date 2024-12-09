#include <chrono>
#include <memory>
#include <optional>

#include <Wt/WContainerWidget.h>
#include <Wt/WDateTime.h>
#include <Wt/WDialog.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>

#include "Appointment.h"
#include "AdvisorPairing.h"
#include "MainApp.h"
#include "PurgeOldTerms.h"
#include "SetupRoute.h"
#include "Session.h"
#include "Term.h"
#include "TermBuilder.h"
#include "TermQuery.h"
#include "UserRoleRedirectUtil.h"

namespace dbo = Wt::Dbo;

PurgeTerms::PurgeTerms(MainApp *app) : View(app), session(app->getSession()) {
    if (redirectFromAdmin(app)) {
        return;
    }

    addTemplate();
    addPurgeButton();
    renderTerms();
}

void PurgeTerms::addTemplate() {
    templ = addWidget(
        std::make_unique<Wt::WTemplate>(
            Wt::WString::tr(
                "purge-old-terms-template"
            )
        )
    );
    templ->setInternalPathEncoding(true);
}

void PurgeTerms::addPurgeButton() {
    Wt::WPushButton *purgeButton = templ->bindWidget(
        "purge-button",
        std::make_unique<Wt::WPushButton>("Purge Old Terms")
    );
    purgeButton->clicked().connect([=] {
        Wt::WDialog *dialog = addChild(
            std::make_unique<Wt::WDialog>("Confirm Purge")
        );

        Wt::WTemplate *modalTempl = dialog->contents()->addWidget(
            std::make_unique<Wt::WTemplate>(
                Wt::WString::tr(
                    "admin-purge-modal"
                )
            )
        );

        Wt::WPushButton *confirmButton = modalTempl->bindWidget(
            "confirm-button",
            std::make_unique<Wt::WPushButton>("Confirm")
        );

        confirmButton->clicked().connect([=] {
            dbo::Transaction trans(session);

            TermQuery termQuery(session);

            auto terms = termQuery.findOldTerms();

            for (auto &term : terms) {
                using Pairings = dbo::collection<dbo::ptr<AdvisorPairing>>;

                Pairings pairings = session
                    .find<AdvisorPairing>()
                    .where("termID = ?")
                    .bind(term.id())
                    ;

                for (auto &pairing : pairings) {
                    pairing.remove();
                }

                term.remove();
            }

            using TimePoint = std::chrono::time_point<
                std::chrono::system_clock
            >;

            TimePoint removeBefore = std::chrono::system_clock::now();

            std::optional<dbo::ptr<Term>> currTermOpt = termQuery.findByTime(
                std::chrono::system_clock::now()
            );

            if (currTermOpt) {
                auto currTerm = *currTermOpt;
                removeBefore = currTerm->getStartTime();    
            }

            using Appts = dbo::collection<dbo::ptr<Appointment>>;
            
            Appts appts = session
                .find<Appointment>()
                .where("endTime <= ?")
                .bind(removeBefore)
                ;

            for (auto &appt : appts) {
                appt.remove();
            }

            dialog->accept();

            renderTerms();
        });

        Wt::WPushButton *cancelButton = modalTempl->bindWidget(
            "cancel-button",
            std::make_unique<Wt::WPushButton>("Cancel")
        );

        cancelButton->clicked().connect([=] {
            dialog->accept();
        });

        dialog->show();
    });
}

void PurgeTerms::renderTerms() {
    dbo::Transaction trans(session);

    termContainer = templ->bindWidget(
        "term-container",
        std::make_unique<Wt::WContainerWidget>()
    );

    TermQuery termQuery(session); 
    auto termOpt = termQuery.findByTime(std::chrono::system_clock::now());
    if (termOpt) {
        auto term = *termOpt;
        Wt::WTemplate *currentTermTempl = termContainer->addWidget(
            std::make_unique<Wt::WTemplate>(
                Wt::WString::tr("admin-current-term-partial")
            )
        );
        currentTermTempl->bindWidget(
            "title",
            std::make_unique<Wt::WText>(
                term->getName()
            )
        );

        std::string format = "ddd MMM d yyyy hh:mm:ss";

        Wt::WDateTime startTime(term->getStartTime());
        currentTermTempl->bindWidget(
            "start-date",
            std::make_unique<Wt::WText>(
                startTime.toString(format)
            )
        );

        Wt::WDateTime endTime(term->getEndTime());
        currentTermTempl->bindWidget(
            "end-date",
            std::make_unique<Wt::WText>(
                endTime.toString(format)
            )
        );
    }

    auto terms = termQuery.findOldTerms();

    for (auto &term : terms) {
        Wt::WTemplate *currentTermTempl = termContainer->addWidget(
            std::make_unique<Wt::WTemplate>(
                Wt::WString::tr("admin-term-partial")
            )
        );
        currentTermTempl->bindWidget(
            "title",
            std::make_unique<Wt::WText>(
                term->getName()
            )
        );

        std::string format = "ddd MMM d yyyy hh:mm:ss";

        Wt::WDateTime startTime(term->getStartTime());
        currentTermTempl->bindWidget(
            "start-date",
            std::make_unique<Wt::WText>(
                startTime.toString(format)
            )
        );

        Wt::WDateTime endTime(term->getEndTime());
        currentTermTempl->bindWidget(
            "end-date",
            std::make_unique<Wt::WText>(
                endTime.toString(format)
            )
        );
    }
}

SETUP_ROUTE("/admin/purge_terms", PurgeTerms)

