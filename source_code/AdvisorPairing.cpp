#include "AdvisorPairing.h"
#include "DatabaseUtils.h"

AdvisorPairing::AdvisorPairing() :
	studentID(-1),
	advisorID(-1),
	termID(-1) {}

int64_t AdvisorPairing::getStudentID() const {
    return studentID;
}

void AdvisorPairing::setStudentID(int64_t sID) {
    studentID = sID;
}

int64_t AdvisorPairing::getAdvisorID() const {
    return advisorID;
}

void AdvisorPairing::setAdvisorID(int64_t aID) {
    advisorID = aID;
}

int64_t AdvisorPairing::getTermID() const {
    return termID;
}

void AdvisorPairing::setTermID(int64_t tID) {
    termID = tID;
}

CREATE_TABLE(AdvisorPairing, advisor_pairing)

