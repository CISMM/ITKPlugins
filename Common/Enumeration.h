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

  int GetNumberOfEnumerants() const {
    return static_cast< int >( m_Enumerants.size() );
  }

  const std::string & GetEnumerantName( int index ) const {
    return m_Enumerants[index].first;
  }

  int GetEnumerantValue( int index ) const {
    return m_Enumerants[index].second;
  }

  int GetEnumerantValueForName( const std::string & name ) const {
    for ( size_t i = 0; i < m_Enumerants.size(); ++i )
      {
      if ( m_Enumerants[i].first == name )
        return m_Enumerants[i].second;
      }

    // Not found
    return -1;
  }

  void PrintSelf( std::ostream & os );

protected:

private:
  std::string m_Name;

  std::vector< std::pair< std::string, int > > m_Enumerants;
};

#endif

