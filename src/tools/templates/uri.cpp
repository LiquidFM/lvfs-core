/**
 * This file is part of lvfs-core.
 *
 * Copyright (C) 2011-2014 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * lvfs-core is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lvfs-core is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lvfs-core. If not, see <http://www.gnu.org/licenses/>.
 */

#include "uri.h"
#include "statemachine.h"

#include <stdlib.h>
#include <string.h>


namespace LVFS {
namespace Tools {
namespace Templates {

Uri::Uri(const char *uri) :
    m_valid(false),
    m_buffer(strdup(uri)),
    m_schema(""),
    m_userName(""),
    m_password(""),
    m_port(0),
    m_domain("")
{
    enum State
    {
        Shema,
        Colon,
        Slash,
        UserName,
        Password,
        Domain,
        Port,
        Path,
        Stoped
    };

    /**
     * XXX: Colon (:) is a valid character in UNIX files names
     * but we ignore it here.
     */
    typedef RULE(Shema,    '/', Path,      Uri, path)                        Rule1;
    typedef RULE(Shema,      0, Stoped,    Uri, path)                        Rule2;
    typedef RULE(Shema,    ':', Colon,     Uri, schema)                      Rule3;
    typedef RULE(Colon,    '/', Slash,     Uri, doNothing)                   Rule4;
    typedef RULE(Slash,    '/', UserName,  Uri, doNothing)                   Rule5;

    typedef RULE(UserName, '/', Path,      Uri, domain)                      Rule6;
    typedef RULE(UserName,   0, Stoped,    Uri, domain)                      Rule7;

    typedef RULE(UserName, ':', Password,  Uri, userName)                    Rule8;
    typedef RULE(UserName, '@', Domain,    Uri, userName)                    Rule9;
    typedef RULE(Password, '@', Domain,    Uri, password)                    Rule10;
    typedef RULE(Password, '/', Path,      Uri, userNameIsDomain_thisIsPort) Rule11;
    typedef RULE(Password,   0, Stoped,    Uri, userNameIsDomain_thisIsPort) Rule12;

    typedef RULE(Domain,   ':', Port,      Uri, domain)                      Rule13;
    typedef RULE(Domain,   '/', Path,      Uri, domain)                      Rule14;
    typedef RULE(Domain,     0, Stoped,    Uri, domain)                      Rule15;

    typedef RULE(Port,     '/', Path,      Uri, port)                        Rule16;
    typedef RULE(Port,       0, Stoped,    Uri, port)                        Rule17;

    typedef RULE(Path,     '/', Path,      Uri, path)                        Rule18;
    typedef RULE(Path,       0, Stoped,    Uri, path)                        Rule19;

    typedef LOKI_TYPELIST_19(Rule1,  Rule2,  Rule3,  Rule4,  Rule5,
                             Rule6,  Rule7,  Rule8,  Rule9,  Rule10,
                             Rule11, Rule12, Rule13, Rule14, Rule15,
                             Rule16, Rule17, Rule18, Rule19) Rules;

    m_valid = StateMachine<Rules>(this).process(m_buffer) == Stoped;

//  Uri parser1("http://UserName:Password@site.com:3128");
//
//  if (parser1.m_valid)
//      qDebug() << "VALID" << "http://UserName:Password@site.com:3128";
//  else
//      qDebug() << "INVALID" << "http://UserName:Password@site.com:3128";
//
//  Uri parser2("UserName:Password@site.com:3128");
//
//  if (parser2.m_valid)
//      qDebug() << "VALID" << "UserName:Password@site.com:3128";
//  else
//      qDebug() << "INVALID" << "UserName:Password@site.com:3128";
//
//  Uri parser3("http://UserName@site.com:3128");
//
//  if (parser3.m_valid)
//      qDebug() << "VALID" << "http://UserName@site.com:3128";
//  else
//      qDebug() << "INVALID" << "http://UserName@site.com:3128";
//
//  Uri parser4("UserName@site.com:3128");
//
//  if (parser4.m_valid)
//      qDebug() << "VALID" << "UserName@site.com:3128";
//  else
//      qDebug() << "INVALID" << "UserName@site.com:3128";
//
//  Uri parser5("http://site.com:3128");
//
//  if (parser5.m_valid)
//      qDebug() << "VALID" << "http://site.com:3128";
//  else
//      qDebug() << "INVALID" << "http://site.com:3128";
//
//  Uri parser6("site.com:3128");
//
//  if (parser6.m_valid)
//      qDebug() << "VALID" << "site.com:3128";
//  else
//      qDebug() << "INVALID" << "site.com:3128";
//
//  Uri parser7("http://site.com");
//
//  if (parser7.m_valid)
//      qDebug() << "VALID" << "http://site.com";
//  else
//      qDebug() << "INVALID" << "http://site.com";
//
//  Uri parser8("site.com");
//
//  if (parser8.m_valid)
//      qDebug() << "VALID" << "site.com";
//  else
//      qDebug() << "INVALID" << "site.com";
//
//  Uri parser9("site.com/home/dav");
//
//  if (parser9.m_valid)
//      qDebug() << "VALID" << "site.com/home/dav";
//  else
//      qDebug() << "INVALID" << "site.com/home/dav";
//
//  Uri parser10("/home/dav");
//
//  if (parser10.m_valid)
//      qDebug() << "VALID" << "/home/dav";
//  else
//      qDebug() << "INVALID" << "/home/dav";
}

Uri::~Uri()
{
    free(m_buffer);
}

void Uri::doNothing(char *string, int size)
{}

void Uri::schema(char *string, int size)
{
    m_schema = string;
    string[size] = 0;
}

void Uri::userName(char *string, int size)
{
    m_userName = string;
    string[size] = 0;
}

void Uri::password(char *string, int size)
{
    m_password = string;
    string[size] = 0;
}

void Uri::domain(char *string, int size)
{
#if PLATFORM_OS(UNIX)
    if (size == 0)
        m_path.push_back("/");
    else
#endif
    {
        m_domain = string;
        string[size] = 0;
    }
}

void Uri::port(char *string, int size)
{
    string[size] = 0;
    m_port = strtol(string, NULL, 10);
}

void Uri::path(char *string, int size)
{
    if (size > 0)
    {
        m_path.push_back(string);
        string[size] = 0;
    }
#if PLATFORM_OS(UNIX)
    else
        if (m_path.empty())
            m_path.push_back("/");
#endif
}

void Uri::userNameIsDomain_thisIsPort(char *string, int size)
{
    m_domain = m_userName;
    m_userName = "";
    port(string, size);
}

}}}
