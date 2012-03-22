#ifndef __Enumeration_h_
#define __Enumeration_h_

#include <string>
#include <utility>
#include <vector>


class Enumeration {
public:
  Enumeration();
  virtual ~Enumeration();

  void SetName( const std::string & name ) {
    m_Name = name;
  }
  const std::string & GetName() const {
    return m_Name;
  }

  void AddEnumerant( const std::string & enumerantName, int value ) {
    m_Enumerants.push_back( std::make_pair< std::string, int >( enumerantName, value ) );
  }

  void PrintSelf( std::ostream & os );

protected:

private:
  std::string m_Name;

  std::vector< std::pair< std::string, int > > m_Enumerants;
};

#endif

