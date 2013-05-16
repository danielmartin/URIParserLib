//
//  URI.h
//
//  This class represents a URI
//
//  URIParserLib
//
//  Created by Daniel Martín on 4/24/13.
//  Copyright (c) 2013 Daniel Martín. All rights reserved.
//

#ifndef __URIParserLib__URI__
#define __URIParserLib__URI__

#include <string>

using namespace std;

class URI
{
public:
    URI() : m_SchemeName(""),
            m_Authority(""),
            m_User(""),
            m_Password(""),
            m_Host(""),
            m_Port(""),
            m_Path(""),
            m_QueryString(""),
            m_FragmentIdentifier("")
    {}
    
    void setSchemeName(const string& schemeName) { m_SchemeName = string(schemeName); }
    string getSchemeName() const { return m_SchemeName; }
    
    void setAuthority(const string& authority) { m_Authority = string(authority); }
    string getAuthority() const { return m_Authority; }
    
    void setCredentials(const string& credentials) { m_Credentials = string(credentials); }
    string getCredentials() const { return m_Credentials; }
    
    void setDestination(const string& destination) { m_Destination = string(destination); }
    string getDestination() const { return m_Destination; }
    
    void setUser(const string& user) { m_User = string(user); }
    string getUser() const { return m_User; }
    
    void setPassword(const string& password) { m_Password = string(password); }
    string getPassword() const { return m_Password; }
    
    void setHost(const string& host) { m_Host = string(host); }
    string getHost() const { return m_Host; }
    
    void setPort(const string& port) { m_Port = string(port); }
    string getPort() const { return m_Port; }
    
    void setPath(const string& path) { m_Path = string(path); }
    string getPath() const { return m_Path; }
    
    void setQueryString(const string& queryString) { m_QueryString = string(queryString); }
    string getQueryString() const { return m_QueryString; }
    
    void setFragmentIdentifier(const string& fragmentIdentifier) { m_FragmentIdentifier = string(fragmentIdentifier); }
    string getFragmentIdentifier() const { return m_FragmentIdentifier; }
    
private:
    /*
     * URI scheme name
     */
    string m_SchemeName;
    /*
     * Authority section
     */
    string m_Authority;
    /*
     * Credentials section
     */
    string m_Credentials;
    /*
     * Destination section
     */
    string m_Destination;
    
    string m_User;
    string m_Password;
    string m_Host;
    string m_Port;
    
    string m_Path;
    string m_QueryString;
    string m_FragmentIdentifier;
};

#endif /* defined(__URIParserLib__URI__) */
