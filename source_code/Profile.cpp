#include <string>

#include <Wt/Auth/Dbo/AuthInfo.h>
#include <Wt/Dbo/Impl.h>
#include <Wt/Auth/Dbo/AuthInfo.h>

#include "DatabaseUtils.h"
#include "Profile.h"

Profile::Profile() : 
  firstName("fname"),
  lastName("lname"),
  role(UserRole::STUDENT) {}

std::string Profile::getFirstName() const {
  return firstName;
}

void Profile::setFirstName(std::string s) {
  firstName = std::move(s); 
}

std::string Profile::getLastName() const {
  return lastName;
}

void Profile::setLastName(std::string s) {
  lastName = std::move(s);
} 

UserRole Profile::getRole() const {
    return role;
}

void Profile::setRole(UserRole r) {
    role = r;
}

DBO_INSTANTIATE_TEMPLATES(Profile)

CREATE_TABLE(Profile, profile)

