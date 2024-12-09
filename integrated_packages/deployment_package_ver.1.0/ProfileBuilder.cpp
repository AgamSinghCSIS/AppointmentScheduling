#include <chrono>
#include <utility>
#include <memory>

#include "ProfileBuilder.h"
#include "Profile.h"

void ProfileBuilder::reset() {
    firstName = "";
    lastName = "";
    role = UserRole::STUDENT;
}

ProfileBuilder::ProfileBuilder() {
    reset();
}
ProfileBuilder& ProfileBuilder::withFirstName(std::string f) {
    firstName = std::move(f);
    return *this;
}

ProfileBuilder& ProfileBuilder::withLastName(std::string l) {
    lastName = std::move(l);
    return *this;
}

ProfileBuilder& ProfileBuilder::withRole(UserRole r) {
    role = std::move(r);
    return *this;
}

Profile ProfileBuilder::build() {
    Profile a;
    a.setFirstName(std::move(firstName));
    a.setLastName(std::move(lastName));
    a.setRole(role);
    reset();
    return a;
}

std::unique_ptr<Profile> ProfileBuilder::buildUnique() {
    auto a = std::make_unique<Profile>();
    a->setFirstName(std::move(firstName));
    a->setLastName(std::move(lastName));
    a->setRole(role);
    reset();
    return a;
}

