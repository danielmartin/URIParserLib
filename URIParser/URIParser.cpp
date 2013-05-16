//
//  URIParser.cpp
//  URIParserLib
//
//  Created by Daniel Martín on 4/24/13.
//  Copyright (c) 2013 Daniel Martín. All rights reserved.
//

#include "URIParser.h"
#include "URI.h"
#include <iterator>
#include <iostream>

void URIParser::parseAuthority(const string &anAuthority, URI *anURI, string &aCredentials, string &aDestination)
{
    bool goodParse = true;
    m_authorityState = AUTHORITY_PARSE_STATE_CREDENTIALS;
    
    for (string::const_iterator anAuthorityIterator = anAuthority.begin(); anAuthorityIterator != anAuthority.end() && goodParse; anAuthorityIterator++) {
        switch (m_authorityState) {
            case AUTHORITY_PARSE_STATE_CREDENTIALS:
                if (*anAuthorityIterator == '@') {
                    m_authorityState = AUTHORITY_PARSE_STATE_DESTINATION;
                } else {
                    aCredentials.push_back(*anAuthorityIterator);
                }
                break;
            case AUTHORITY_PARSE_STATE_DESTINATION:
                aDestination.push_back(*anAuthorityIterator);
                break;
            default:
                break;
        }
    }
    
    if (goodParse) {
        if (!aCredentials.empty()) {
            if (m_authorityState == AUTHORITY_PARSE_STATE_CREDENTIALS) {
                aDestination = aCredentials;
                anURI->setDestination(aDestination);
                aCredentials.clear();
            } else {
                anURI->setCredentials(aCredentials);
            }
        }
        if (!aDestination.empty()) {
            anURI->setDestination(aDestination);
        }
    } else {
        delete anURI;
        anURI = NULL;
    }
}

void URIParser::parseCredentials(const string &aCredentials, URI *anURI, string &aUser, string &aPassword)
{
    bool goodParse = true;
    m_credentialsState = CREDENTIALS_PARSE_STATE_USER;

    for (string::const_iterator aCredentialsIterator = aCredentials.begin(); aCredentialsIterator != aCredentials.end() && goodParse; aCredentialsIterator++) {
        switch (m_credentialsState) {
            case CREDENTIALS_PARSE_STATE_USER:
                if (*aCredentialsIterator == ':') {
                    m_credentialsState = CREDENTIALS_PARSE_STATE_PASSWORD;
                } else {
                    aUser.push_back(*aCredentialsIterator);
                }
                break;
            case CREDENTIALS_PARSE_STATE_PASSWORD:
                aPassword.push_back(*aCredentialsIterator);
                break;
            default:
                break;
        }
    }
    
    if (goodParse) {
        if (!aUser.empty()) {
            anURI->setUser(aUser);
        }
        if (!aPassword.empty()) {
            anURI->setPassword(aPassword);
        }
    } else {
        delete anURI;
        anURI = NULL;
    }
}

void URIParser::parseDestination(const string &aDestination, URI *anURI, string &aHost, string &aPort)
{
    bool goodParse = true;
    m_destinationState = DESTINATION_PARSE_STATE_HOST;

    for (string::const_iterator aDestinationIterator = aDestination.begin(); aDestinationIterator != aDestination.end() && goodParse; aDestinationIterator++) {
        switch (m_destinationState) {
            case DESTINATION_PARSE_STATE_HOST:
                if (*aDestinationIterator == ':') {
                    m_destinationState = DESTINATION_PARSE_STATE_PORT;
                } else {
                    aHost.push_back(*aDestinationIterator);
                }
                break;
            case DESTINATION_PARSE_STATE_PORT:
                aPort.push_back(*aDestinationIterator);
                break;
            default:
                break;
        }
    }
    
    if (goodParse) {
        if (!aHost.empty()) {
            anURI->setHost(aHost);
        }
        if (!aPort.empty()) {
            anURI->setPort(aPort);
        }
    } else {
        delete anURI;
        anURI = NULL;
    }
}


bool URIParser::parseGlobal(const string &anURIString, URI *anURI, string &anAuthority, const URIParseState &initialState)
{
    bool goodParse = false;
    bool keepIterating = true;
    bool isRelativeURL = true;
    m_parseState = initialState;
    
    string anSchemeName, aPath, aQueryString, aFragmentID;
    int numOfSlashes = 0; // Counter of consecutive forward slashes
    
    for (string::const_iterator anURIIterator = anURIString.begin(); anURIIterator != anURIString.end() && keepIterating; anURIIterator++) {
        switch (m_parseState) {
            case URI_PARSE_STATE_SCHEME:
                goodParse = true; // Relative URLs does not have an scheme
                if (*anURIIterator == ':') {
                    if (anURIIterator+1 != anURIString.end()) {
                        if (*(anURIIterator+1) == '/') {
                            // Absolute URL
                            m_parseState = URI_PARSE_STATE_HIERARCHICAL_URL_ID;
                            goodParse = false; // At this point, the URL is invalid
                        } else {
                            // Relative URL. Restart parsing in URI_PARSE_STATE_AUTHORITY state
                            anURIIterator = anURIString.begin()-1;
                            m_parseState = URI_PARSE_STATE_AUTHORITY;
                        }
                    }
                } else {
                    anSchemeName.push_back(*anURIIterator);
                }
                break;
            case URI_PARSE_STATE_HIERARCHICAL_URL_ID:
                if (*anURIIterator == '/') {
                    numOfSlashes++;
                    if (numOfSlashes == 2) {
                        m_parseState = URI_PARSE_STATE_AUTHORITY;
                        isRelativeURL = false; // This is not a relative URL anymore
                        goodParse = true; // Switch parse status to 'good' again
                    }
                } else {
                    numOfSlashes = 0;
                    m_parseState = URI_PARSE_STATE_AUTHORITY;
                }
                break;
            case URI_PARSE_STATE_AUTHORITY:
                if (*anURIIterator == '/' || *anURIIterator == '?' || *anURIIterator == '#') {
                    if (*anURIIterator == '/') {
                        m_parseState = URI_PARSE_STATE_PATH;
                    } else if (*anURIIterator == '?') {
                        m_parseState = URI_PARSE_STATE_QUERY_STRING;
                    } else if (*anURIIterator == '#') {
                        m_parseState = URI_PARSE_STATE_FRAGMENT_ID;
                    }
                } else {
                    anAuthority.push_back(*anURIIterator);
                }
                break;
            case URI_PARSE_STATE_PATH:
                // There is a potential path. Parse it
                if (*anURIIterator == '?' || *anURIIterator == '#') {
                    if (*anURIIterator == '?') {
                        m_parseState = URI_PARSE_STATE_QUERY_STRING;
                    } else if (*anURIIterator == '#') {
                        m_parseState = URI_PARSE_STATE_FRAGMENT_ID;
                    }
                } else {
                    aPath.push_back(*anURIIterator);
                }
                break;
            case URI_PARSE_STATE_QUERY_STRING:
                if (*anURIIterator == '#') {
                    m_parseState = URI_PARSE_STATE_FRAGMENT_ID;
                } else {
                    aQueryString.push_back(*anURIIterator);
                }
                break;
            case URI_PARSE_STATE_FRAGMENT_ID:
                if (*anURIIterator == '\0') {
                    
                } else {
                    aFragmentID.push_back(*anURIIterator);
                }
                break;
            default:
                break;
        }
    }
    if (!anSchemeName.empty() && anAuthority.empty()) {
        // Restart
        if (parseGlobal(anURIString, anURI, anAuthority, URI_PARSE_STATE_AUTHORITY)) {
            // If we have successfully parsed an authority section, further parse it into subsections
            if (!anAuthority.empty()) {
                string aCredentials, aDestination;
                parseAuthority(anAuthority, anURI, aCredentials, aDestination);
                
                // If we have successfully parsed a credentials section, further parse it into username and password
                if (!aCredentials.empty()) {
                    string aUser, aPassword;
                    parseCredentials(aCredentials, anURI, aUser, aPassword);
                }
                
                // If we have successfully parsed a destination section, further parse it into host and port
                if (!aDestination.empty()) {
                    string aHost, aPort;
                    parseDestination(aDestination, anURI, aHost, aPort);
                }
            }
        } else {
            delete anURI;
            anURI = NULL;
        }
    } else if (true) {
        if (!anSchemeName.empty() && !isRelativeURL) {
            anURI->setSchemeName(anSchemeName);
        }
        if (!anAuthority.empty()) {
            anURI->setAuthority(anAuthority);
        }
        if (!aPath.empty()) {
            anURI->setPath(aPath);
        }
        if (!aQueryString.empty()) {
            anURI->setQueryString(aQueryString);
        }
        if (!aFragmentID.empty()) {
            anURI->setFragmentIdentifier(aFragmentID);
        }
    } else {
        return false;
    }
    
    return true;
}

/*
 * The URI structure is the following:
 *
 *            user  pass  host   port
 *            credentials destination
 * scheme ://       authority         {/,?,#}    path    {?,#}    query string  # fragment ID
 * --------------------------------------------------------------------------------------------
 *
 * Optional components are: credentials, port, query string, and fragment ID
 *
 */
const URI *URIParser::parse()
{
    if (m_parseURI.empty())
        return NULL;
    
    // Create a new URI object
    URI *anURI = new URI();
    
    // Parse the URL globally
    string anAuthority;
    if (parseGlobal(m_parseURI, anURI, anAuthority, URI_PARSE_STATE_SCHEME)) {
        // If we have successfully parsed an authority section, further parse it into subsections
        if (!anAuthority.empty()) {
            string aCredentials, aDestination;
            parseAuthority(anAuthority, anURI, aCredentials, aDestination);
            
            // If we have successfully parsed a credentials section, further parse it into username and password
            if (!aCredentials.empty()) {
                string aUser, aPassword;
                parseCredentials(aCredentials, anURI, aUser, aPassword);
            }
            
            // If we have successfully parsed a destination section, further parse it into host and port
            if (!aDestination.empty()) {
                string aHost, aPort;
                parseDestination(aDestination, anURI, aHost, aPort);
            }
        }
    } else {
        delete anURI;
        anURI = NULL;
    }

    return anURI;
}






