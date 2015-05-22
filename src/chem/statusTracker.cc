       
#define	DEBUG_LEVEL_NONE

#include "statusTracker.h"
//#include "core/archiveNode.h"
#include "core/wrappers.h"



namespace chem {

    REGISTER_CLASS(chem,StatusTracker_O);



//
// Constructor
//

void	StatusTracker_O::initialize()
{
    this->Base::initialize();
    this->_HasError = false;
    this->_ErrorStream.str("");
    this->_MessageStream.str("");
}




    StatusTracker_O::StatusTracker_O(const StatusTracker_O& ss ) : core::T_O(ss)
{
    this->_HasError = ss._HasError;
    this->_MessageStream.str(ss._MessageStream.str());
    this->_ErrorStream.str(ss._ErrorStream.str());
}




//
// Destructor
//





#ifdef XML_ARCHIVE
    void	StatusTracker_O::archive(core::ArchiveP node)
{_G();
    node->attribute("hasError", this->_HasError );
    node->archiveStringStreamIfDefined("ErrorMessages",this->_ErrorStream);
    node->archiveStringStreamIfDefined("Messages",this->_MessageStream);
}
#endif





void	StatusTracker_O::addMessage(const string& msg)
{_G();
    LOG(BF("Message: %s") % msg.c_str()  );
    this->_MessageStream << msg << std::endl;
}





void	StatusTracker_O::addError(const string& msg)
{_G();
    LOG(BF("Message: %s") % msg.c_str()  );
    this->_HasError = true;
    this->_ErrorStream << msg << std::endl;
}






void	StatusTracker_O::reset()
{_G();
    this->_HasError = false;
    this->_ErrorStream.str("");
    this->_MessageStream.str("");
}

string	StatusTracker_O::getStatus()
{_G();
stringstream	stuff;
    stuff.str("");
    stuff << this->_MessageStream.str();
    if ( this->_HasError )
    {
        stuff << this->_ErrorStream.str();
    }
    LOG(BF("Status = |%s|") % stuff.str().c_str()  );
    return stuff.str();
}


StatusTracker_sp	StatusTracker_O::copy()
{_G();
    GC_COPY(StatusTracker_O, stnew , *this); // = RP_Copy<StatusTracker_O>(this);
    return stnew;
}






};


