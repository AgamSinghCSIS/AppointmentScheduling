#ifndef PROFILE_H
#define PROFILE_H

#include <string> 

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>

enum UserRole {
    ADMIN,
    FACULTY,
    STUDENT
};

class Profile;
using AuthInfo = Wt::Auth::Dbo::AuthInfo<Profile>;
    
class Profile {
private: 
  std::string firstName;
  std::string lastName;
  UserRole role;

public: 
  Profile();

  std::string getFirstName() const;
  void setFirstName(std::string);

  std::string getLastName() const;
  void setLastName(std::string);

  UserRole getRole() const;
  void setRole(UserRole);

  template<typename Action>
  void persist(Action&);
};

template<typename Action>
void Profile::persist(Action &a) {
  namespace dbo = Wt::Dbo;

  dbo::field(a, firstName, "firstName");
  dbo::field(a, lastName, "lastName");
  dbo::field(a, role, "role");
  
}

DBO_EXTERN_TEMPLATES(Profile)

#endif // PROFILE_H
