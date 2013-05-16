//
//  URIParser.h
//  URIParserLib
//
//  Created by Daniel Martín on 4/24/13.
//  Copyright (c) 2013 Daniel Martín. All rights reserved.
//

#ifndef __URIParserLib__URIParser__
#define __URIParserLib__URIParser__

#include <string>
#include "URI.h"

using namespace std;

typedef enum {
    URI_PARSE_STATE_SCHEME,
    URI_PARSE_STATE_HIERARCHICAL_URL_ID,
    URI_PARSE_STATE_AUTHORITY,
    URI_PARSE_STATE_PATH,
    URI_PARSE_STATE_QUERY_STRING,
    URI_PARSE_STATE_FRAGMENT_ID
} URIParseState;

typedef enum {
    AUTHORITY_PARSE_STATE_CREDENTIALS,
    AUTHORITY_PARSE_STATE_DESTINATION
} AuthorityParseState;

typedef enum {
    CREDENTIALS_PARSE_STATE_USER,
    CREDENTIALS_PARSE_STATE_PASSWORD
} CredentialsParseState;

typedef enum {
    DESTINATION_PARSE_STATE_HOST,
    DESTINATION_PARSE_STATE_PORT
} DestinationParseState;

class URIParser
{
public:
    URIParser();
    URIParser(const string& parseURI) : m_parseURI(parseURI) {}
    
    const URI *parse();
    
private:
    string m_parseURI;
    URIParseState m_parseState; // Global URI parse state
    AuthorityParseState m_authorityState;
    CredentialsParseState m_credentialsState;
    DestinationParseState m_destinationState;
    
    bool parseGlobal(const string &anURIString, URI *anURI, string &anAuthority, const URIParseState &initialState);
    void parseAuthority(const string &anAuthority, URI *anURI, string &aCredentials, string &aDestination);
    void parseCredentials(const string &aCredentials, URI *anURI, string &aUser, string &aPassword);
    void parseDestination(const string &aDestination, URI *anURI, string &aHost, string &aPort);
};

#endif /* defined(__URIParserLib__URIParser__) */
