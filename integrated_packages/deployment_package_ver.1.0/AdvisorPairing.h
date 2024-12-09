#ifndef ADVISORPAIRING_H
#define ADVISORPAIRING_H

#include <string>

#include <Wt/Dbo/Dbo.h>

class AdvisorPairing {
private:
    int64_t studentID;
    int64_t advisorID;
    int64_t termID;

public:
    AdvisorPairing();
    int64_t getStudentID() const;
    void setStudentID(int64_t);
    int64_t getAdvisorID() const;
    void setAdvisorID(int64_t);
    int64_t getTermID() const;
    void setTermID(int64_t);

    template<typename Action>
    void persist(Action&);
};

template<typename Action>
void AdvisorPairing::persist(Action &a) {
    namespace dbo = Wt::Dbo;

    dbo::field(a, studentID, "studentID");
    dbo::field(a, advisorID, "advisorID");
    dbo::field(a, termID, "termID");
}

#endif //ADVISORPAIRING_H
