#include "includes/request.hpp"
#include <string>

Disposition::Disposition(std::string disposition)
{
    if (!disposition.empty())
    {
        // std::cerr << "HERE2" << std::endl;

        size_t typeStart = disposition.find("name=") + 6;
        size_t typeEnd = disposition.find("\"", typeStart);
        if (typeEnd != 0)
            typeEnd = disposition.find("\r", typeStart);
        if (typeStart != std::string::npos)
            type = disposition.substr(typeStart, typeEnd - typeStart);

        size_t nameStart = disposition.find("filename=") + 10;
        size_t nameEnd = disposition.find("\"", nameStart);\
        if (nameEnd != 0)
            nameEnd = disposition.find("\r", nameStart);
        if (nameStart != std::string::npos)
            filename = disposition.substr(nameStart, nameEnd - nameStart);

        size_t contentEnd = disposition.find(";", 0);
        if (contentEnd != std::string::npos)
            contentdisposition = disposition.substr(0, contentEnd);
    }
}

std::string Disposition::getType( void ) const
{
    return type;
}

std::string Disposition::getFilename( void ) const
{
    return filename;
}

std::string Disposition::getcontentdisposition( void ) const
{
    return contentdisposition;
}

Content::Content(std::string Content, std::string boundary)
{
    if (!Content.empty() && !boundary.empty())
    {
        // std::cerr << "HERE1" << std::endl;

        size_t dispositionStart = Content.find("Content-Disposition: ") + 20;
        size_t dispositionEnd = Content.find("\r", dispositionStart);
        if (dispositionStart != std::string::npos)
            filename = Disposition(Content.substr(dispositionStart, dispositionEnd - dispositionStart));

        size_t typeStart = Content.find("Content-Type: ") + 13;
        size_t typeEnd = Content.find("\r", typeStart + 1);
        if (typeStart != std::string::npos)
            content_type = Content.substr(typeStart, typeEnd - typeStart);

        size_t fileStart = Content.find("\r\n\r\n") + 4;
        size_t fileEnd = Content.find(boundary, fileStart);
        if (fileStart != std::string::npos)
            content = Content.substr(fileStart, fileEnd - fileStart);

    }
}

std::string Content::getContent( void ) const
{
    return content;
}

std::string Content::getContentType( void ) const
{
    return content_type;
}

std::string Request::request( void ) const
{
    return _request;
}

std::string Request::Get( void ) const
{
    return get;
}

std::string Request::Post( void ) const
{
    return post;
}

std::string Request::Host( void ) const
{
    return host;
}

std::string Request::Useragent( void ) const
{
    return useragent;
}

std::string Request::Accept( void ) const
{
    return accept;
}

std::string Request::Acceptencoding( void ) const
{
    return acceptencoding;
}

std::string Request::Acceptlanguage( void ) const
{
    return acceptlanguage;
}

std::string Request::Xrequestedwith( void ) const
{
    return xrequestedwith;
}

std::string Request::Connection( void ) const
{
    return connection;
}

std::string Request::Referer( void ) const
{
    return referer;
}

std::string Request::Upgradeinsecurerequests( void ) const
{
    return upgradeinsecurerequests;
}

std::string Request::Secfetchdest( void ) const
{
    return secfetchdest;
}

std::string Request::Secfetchmode( void ) const
{
    return secfetchmode;
}

std::string Request::Secfetchsite( void ) const
{
    return secfetchsite;
}

std::string Request::Secfetchuser( void ) const
{
    return secfetchuser;
}

std::string Request::Contenttype( void ) const
{
    return contenttype;
}

std::string Request::Contentlength( void ) const
{
    return contentlength;
}

std::string Request::Contentdisposition( void ) const
{
    return contentdisposition;
}

std::string Request::Origin( void ) const
{
    return origin;
}

std::string Request::Boundary( void ) const
{
    return boundary;
}

int Request::Eof( void )
{
    return eof;
}

Request::Request( char *buffer )
{
    if (!buffer)
        return;
    std::string req(buffer);
    this->_request = req;

    size_t getStart = _request.find("GET ") + 4;
    if (getStart != std::string::npos)
    {
        // std::cerr << "1" << std::endl;
        
        size_t getEnd = _request.find(" ", getStart);
        size_t getEnd2 = _request.find("?", getStart);
        if (getEnd2 < getEnd)
            getEnd = getEnd2;
        this->get = _request.substr(getStart, getEnd - getStart);
    }

    size_t postStart = _request.find("POST ") + 5;
    if (postStart != std::string::npos)
    {
        // std::cerr << "2" << std::endl;
        size_t postEnd = _request.find(" ", postStart);
        size_t postEnd2 = _request.find("?", postStart);
        if (postEnd2 < postEnd)
            postEnd = postEnd2;
        this->post = _request.substr(postStart, postEnd - postStart);
    }

    size_t hostStart = _request.find("Host: ") + 6;
    if (hostStart != std::string::npos)
    {
        // std::cerr << "3" << std::endl;
        size_t hostEnd = _request.find("\r", hostStart);
        this->host = _request.substr(hostStart, hostEnd - hostStart);
    }
    
    size_t agentStart = _request.find("User-Agent: ") + 12;
    if (agentStart != std::string::npos)
    {
        // std::cerr << "4" << std::endl;
        size_t agentEnd = _request.find("\r", agentStart);
        this->useragent = _request.substr(agentStart, agentEnd - agentStart);
    }

    size_t acceptStart = _request.find("Accept: ") + 8;
    if (acceptStart != std::string::npos)
    {
        // std::cerr << "5" << std::endl;
        size_t acceptEnd = _request.find("\r", acceptStart);
        this->accept = _request.substr(acceptStart, acceptEnd - acceptStart);
    }

    size_t languageStart = _request.find("Accept-Language: ") + 17;
    if (languageStart != std::string::npos)
    {
        // std::cerr << "6" << std::endl;
        size_t languageEnd = _request.find("\r", languageStart);
        this->acceptlanguage = _request.substr(languageStart, languageEnd - languageStart);
    }

    size_t encodingStart = _request.find("Accept-Encoding: ") + 17;
    if (encodingStart != std::string::npos)
    {
        // std::cerr << "7" << std::endl;
        size_t encodingEnd = _request.find("\r", encodingStart);
        this->acceptencoding = _request.substr(encodingStart, encodingEnd - encodingStart);
    }

    size_t xrequestStart = _request.find("X-Requested-With: ") + 18;
    if (xrequestStart != std::string::npos)
    {
        // std::cerr << "8" << std::endl;
        size_t xrequestEnd = _request.find("\r", xrequestStart);
        this->xrequestedwith = _request.substr(xrequestStart, xrequestEnd - xrequestStart);
    }

    size_t connectionStart = _request.find("Connection: ") + 12;
    if (connectionStart != std::string::npos)
    {
        // std::cerr << "9" << std::endl;
        size_t connectionEnd = _request.find("\r", connectionStart);
        this->connection = _request.substr(connectionStart, connectionEnd - connectionStart);
    }

    size_t refererStart = _request.find("Referer: ") + 9;
    if (refererStart != std::string::npos)
    {
        // std::cerr << "10" << std::endl;
        size_t refererend = _request.find(":", refererStart);
        size_t refererend2 = _request.find(":", refererend + 1);
        size_t refererend3 = _request.find("/", refererend2);
        size_t refererend4 = _request.find("\r", refererend3);
        this->referer = _request.substr(refererend3, refererend4 - refererend3);
    }

    size_t upStart = _request.find("Upgrade-Insecure-Requests: ") + 30;
    if (upStart != std::string::npos)
    {
        // std::cerr << "11" << std::endl;
        size_t upEnd = _request.find("\r", upStart);
        this->upgradeinsecurerequests = _request.substr(upStart, upEnd - upStart);
    }

    size_t usrStart = _request.find("Sec-Fetch-User: ") + 16;
    if (usrStart != std::string::npos)
    {
        // std::cerr << "12" << std::endl;
        size_t usrEnd = _request.find("\r", usrStart);
        this->secfetchuser = _request.substr(usrStart, usrEnd - usrStart);
    }

    size_t destStart = _request.find("Sec-Fetch-Dest: ") + 16;
    if (destStart != std::string::npos)
    {
        // std::cerr << "13" << std::endl;
        size_t destEnd = _request.find("\r", destStart);
        this->secfetchdest = _request.substr(destStart, destEnd - destStart);
    }

    size_t modeStart = _request.find("Sec-Fetch-Mode: ") + 16;
    if (modeStart != std::string::npos)
    {
        // std::cerr << "14" << std::endl;
        size_t modeEnd = _request.find("\r", modeStart);
        this->secfetchmode = _request.substr(modeStart, modeEnd - modeStart);
    }

    size_t siteStart = _request.find("Sec-Fetch-Site: ") + 16;
    if (siteStart != std::string::npos)
    {
        // std::cerr << "15" << std::endl;
        size_t siteEnd = _request.find("\r", siteStart);
        this->secfetchsite = _request.substr(siteStart, siteEnd - siteStart);
    }

    size_t cont_typeStart = _request.find("Content-Type: ") + 14;
    if (cont_typeStart != std::string::npos)
    {
        // std::cerr << "16" << std::endl;
        size_t cont_typeEnd = _request.find(";", cont_typeStart);
        size_t cont_typeEnd2 = _request.find("\r", cont_typeStart);
        if (cont_typeEnd2 < cont_typeEnd)
            cont_typeEnd = cont_typeEnd2;
        this->contenttype = _request.substr(cont_typeStart, cont_typeEnd - cont_typeStart);
    }

    size_t boundaryStart = _request.find("boundary=") + 9;
    if (boundaryStart != std::string::npos)
    {
        // std::cerr << "17" << std::endl;
        size_t boundaryEnd = _request.find("\r", boundaryStart);
        this->boundary = _request.substr(boundaryStart, boundaryEnd - boundaryStart);
    }

    size_t cont_lengthStart = _request.find("Content-Length: ") + 16;
    if (cont_lengthStart != std::string::npos)
    {
        // std::cerr << "18" << std::endl;
        size_t cont_lengthEnd = _request.find("\r", cont_lengthStart);
        this->contentlength = _request.substr(cont_lengthStart, cont_lengthEnd - cont_lengthStart);
    }

    size_t originStart = _request.find("Origin: ") + 8;
    if (originStart != std::string::npos)
    {
        // std::cerr << "19" << std::endl;
        size_t originEnd = _request.find("\r", originStart);
        this->origin = _request.substr(originStart, originEnd - originStart);
    }

    size_t dispositionStart = _request.find("\r\n" + boundary) + boundary.size() + 2;
    if (dispositionStart != std::string::npos)
    {
        // std::cerr << "20" << std::endl;
        size_t dispositionEnd = _request.find(boundary + "--", dispositionStart);
        this->contentdisposition = _request.substr(dispositionStart, dispositionEnd - dispositionStart);
    }

        // std::cerr << "21" << std::endl;

    if (!get.empty())
    {
        size_t eofgetStart = _request.find("\r\n\r\n");
        if (eofgetStart != std::string::npos)
            eof = 1;
        else
            eof = 0;
    }
    else
    {
        size_t eofpostStart = _request.find(boundary + "--");
        if (eofpostStart != std::string::npos)
            eof = 1;
        else
            eof = 0;
    }
    if (!contentdisposition.empty() && !boundary.empty())
        this->content = Content(contentdisposition, boundary);
}

Request::Request( const Request &other )
{
    if (this != &other)
        *this = other;  
}

Request &Request::operator=( const Request &other)
{
    this->_request = other._request;
    this->get = other.get;
    this->post = other.post;
    this->host = other.host;
    this->useragent = other.useragent;
    this->accept = other.accept;
    this->acceptencoding = other.acceptencoding;
    this->acceptlanguage = other.acceptlanguage;
    this->xrequestedwith = other.xrequestedwith;
    this->connection = other.connection;
    this->referer = other.referer;
    this->upgradeinsecurerequests = other.upgradeinsecurerequests;
    this->secfetchdest = other.secfetchdest;
    this->secfetchmode = other.secfetchmode;
    this->secfetchsite = other.secfetchsite;
    this->secfetchuser = other.secfetchuser;
    this->contenttype = other.contenttype;
    this->contentlength = other.contentlength;
    this->contentdisposition = other.contentdisposition;
    this->origin = other.origin;
    this->boundary = other.boundary;
    this->content = other.content;
    this->eof = other.eof;
    return *this;
}

Content::Content(const Content &other)
{
    if (this != &other)
        *this = other; 
}

Content &Content::operator=(const Content &other)
{
    this->content_type = other.content_type;
    this->content = other.content;
    this->filename = other.filename;
    return *this;
}

Disposition::Disposition(const Disposition &other)
{
    if (this != &other)
        *this = other; 
}

Disposition &Disposition::operator=(const Disposition &other)
{
    this->type = other.type;
    this->filename = other.filename;
    this->contentdisposition = other.contentdisposition;
    return *this;
}

Disposition::~Disposition()
{
}
Content::~Content()
{
}
Request::~Request()
{
}

Disposition::Disposition()
{
}
Content::Content()
{
}
Request::Request() : eof(0)
{
}

//#textmate (mate);
//snippets