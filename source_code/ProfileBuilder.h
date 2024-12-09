//userRole not done

#ifndef PROFILEBUILDER_H
#define PROFILEBUILDER_H

#include <chrono>
#include <memory>
#include <utility>

#include "Profile.h"

class ProfileBuilder {
private:
    std::string firstName;
    std::string lastName;
    UserRole role;

    void reset();
public:
    ProfileBuilder();

    ProfileBuilder& withFirstName(std::string firstName);
    ProfileBuilder& withLastName(std::string lastName);
    ProfileBuilder& withRole(UserRole role);

    Profile build();
    std::unique_ptr<Profile> buildUnique();
};

#endif // PROFILEBUILDER_H

