#ifndef __MemberDescription_h_
#define __MemberDescription_h_

#include <string>
#include <iostream>

class MemberDescription {
public:
  MemberDescription();
  virtual ~MemberDescription();

  void SetMemberName( const std::string & memberName ) {
    m_MemberName = memberName;
  }
  const std::string & GetMemberName() const {
    return m_MemberName;
  }

  void PrintSelf( std::ostream & os );

protected:

private:
  std::string m_MemberName;
};

#endif
