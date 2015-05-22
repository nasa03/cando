#define	DEBUG_LEVEL_FULL

#include "core/common.h"
#include "frameRecognizer.h"
#include "core/str.h"
#include "core/environment.h"
#include "core/wrappers.h"

namespace chem {


void	FrameRecognizer_O::initialize()
{_OF();
    this->_Name = _Nil<core::Symbol_O>();
    this->_GroupName = _Nil<core::Symbol_O>();
}

void	FrameRecognizer_O::compileSmarts(const string& osm)
{_G();
    this->_Smarts = osm;
    this->_ChemInfo = chem::ChemInfo_O::create();
    this->_ChemInfo ->compileSmarts(this->_Smarts);
    if ( !this->_ChemInfo->compileSucceeded() )
    {
	SIMPLE_ERROR(BF("Error compiling ChemInfo: %s") % this->_ChemInfo);
    }
    LOG(BF("Successfully compiled smarts code for atom O") );
//    LOG(BF("ChemInfo code = %s") % this->_ChemInfo->asXmlString());
}


#ifdef XML_ARCHIVE
    void	FrameRecognizer_O::archive(core::ArchiveP node)
{_OF();
#if PRODUCTION_CODE   // FIXME use "name" only and remove the test for "_key"
    node->attribute("name",this->_Name);
#else
    if ( node->loading() )
    {
	if ( node->hasAttribute("name") )
	{
	    node->attribute("name",this->_Name);
	} else
	{
	    node->attribute("_key",this->_Name);
	}
    } else
    {
	node->attribute("name",this->_Name);
    }
#endif
    node->attribute("smarts",this->_Smarts);
    node->attributeIfNotNil("groupName",this->_GroupName);
    	// Compile the smarts strings into ChemInfo code
    if ( node->loading() )
    {
        TRY()
	{
            this->compileSmarts(this->_Smarts);
	} catch ( core::Condition& err )
	{
	    SIMPLE_ERROR(BF("Could not parse Smarts code for %s %s")% this->_Smarts % err.message() );
	}
    }
};
#endif



bool	FrameRecognizer_O::recognizes( Atom_sp o )
{_G();
    ASSERTNOTNULL(this->_ChemInfo);
    ASSERT(this->_ChemInfo->compileSucceeded());
    return this->_ChemInfo->matches(o);
}


ChemInfoMatch_sp FrameRecognizer_O::getMatch()
{_G();
    return this->_ChemInfo->getMatch();
}



    void FrameRecognizer_O::setRecognizerName(core::Symbol_sp fn)
{
    this->_Name = fn;
}

uint FrameRecognizer_O::depth()
{_G();
    ASSERTNOTNULL(this->_ChemInfo);
    return this->_ChemInfo->depth();
}


    core::Symbol_sp FrameRecognizer_O::getRecognizerName()
{
    return this->_Name;
}

void FrameRecognizer_O::setGroupName(core::Symbol_sp fn)
{
    this->_GroupName = fn;
}

core::Symbol_sp FrameRecognizer_O::getGroupName()
{
    if ( this->_GroupName.nilp() )
	return this->_Name;
    return this->_GroupName;
}



string FrameRecognizer_O::description() const
{
    stringstream ss;
    ss << "( " << this->className() << " ";
    if ( this->_Name.pointerp() )
    {
	ss << " :name " << this->_Name->__repr__();
    } else 
    {
	ss << " :name -UNDEFINED-";
    }
    ss << " :groupName \"" << this->_GroupName->__repr__() << "\"";
    ss << " :smarts " << "\"" << this->_Smarts << "\" )";
    return ss.str();
}


#if INIT_TO_FACTORIES

#define ARGS_FrameRecognizer_O_make "(name smarts group_name)"
#define DECL_FrameRecognizer_O_make ""
#define DOCS_FrameRecognizer_O_make "make FrameRecognizer"
  FrameRecognizer_sp FrameRecognizer_O::make(core::Symbol_sp name, const string& smarts, core::Symbol_sp groupName)
  {_G();
      GC_ALLOCATE(FrameRecognizer_O, me );
    me->_Name = name;
    me->_Smarts = smarts;
    me->_GroupName = groupName;
    me->_ChemInfo = ChemInfo_O::create();
    me->_ChemInfo->compileSmarts(me->_Smarts);
    if ( !me->_ChemInfo->compileSucceeded() )
    {
	SIMPLE_ERROR(BF("%s") % me->_ChemInfo->compilerMessage());
    }
    return me;
  };

#else

    core::T_sp	FrameRecognizer_O::__init__(core::Function_sp exec, core::Cons_sp args, core::Environment_sp env, core::Lisp_sp lisp)
{_OF();
    this->_Name = translate::from_object<core::Symbol_O>::convert(env->lookup(Pkg(),"name"));
    this->_Smarts = translate::from_object<core::Str_O>::convert(env->lookup(Pkg(),"smarts"))->get();
    this->_GroupName = translate::from_object<core::Symbol_O>::convert(env->lookup(Pkg(),"groupName"));
    this->_ChemInfo = ChemInfo_O::create();
    this->_ChemInfo->compileSmarts(this->_Smarts);
    if ( !this->_ChemInfo->compileSucceeded() )
    {
	SIMPLE_ERROR(BF(this->_ChemInfo->compilerMessage()));
    }
    return _Nil<core::T_O>();
}

#endif

void FrameRecognizer_O::exposeCando(core::Lisp_sp lisp)
{
    core::class_<FrameRecognizer_O>()
//	.def_raw("core:__init__",&FrameRecognizer_O::__init__,"(self &key name (smarts \"\") groupName)")
	.def("recognizes",&FrameRecognizer_O::recognizes)
//	.def("boundFrame",&FrameRecognizer_O::boundFrame)
	.def("getMatch",&FrameRecognizer_O::getMatch)
	.def("getSmarts",&FrameRecognizer_O::getSmarts)
//	.def("getMatchedAtomWithTag",&FrameRecognizer_O::getMatchedAtomWithTag)
//	.def("getTemporaryOAtomMatch",&FrameRecognizer_O::getTemporaryOAtomMatch)
//	.def("getTemporaryPAtomMatch",&FrameRecognizer_O::getTemporaryPAtomMatch)
//	.def("getTemporaryQAtomMatch",&FrameRecognizer_O::getTemporaryQAtomMatch)
	.def("getRecognizerName",&FrameRecognizer_O::getRecognizerName)
	.def("getGroupName",&FrameRecognizer_O::getGroupName)
	.def("compileSmarts",&FrameRecognizer_O::compileSmarts)
	.def("depth",&FrameRecognizer_O::depth)
	;
}

    void FrameRecognizer_O::exposePython(core::Lisp_sp lisp)
    {_G();
#ifdef	USEBOOSTPYTHON //[
#ifdef USEBOOSTPYTHON
	PYTHON_CLASS(ChemPkg,FrameRecognizer,"","",_lisp)
	.def("recognizes",&FrameRecognizer_O::recognizes)
//	.def("boundFrame",&FrameRecognizer_O::boundFrame)
	.def("getMatch",&FrameRecognizer_O::getMatch)
	.def("getSmarts",&FrameRecognizer_O::getSmarts)
//	.def("getMatchedAtomWithTag",&FrameRecognizer_O::getMatchedAtomWithTag)
//	.def("getTemporaryOAtomMatch",&FrameRecognizer_O::getTemporaryOAtomMatch)
//	.def("getTemporaryPAtomMatch",&FrameRecognizer_O::getTemporaryPAtomMatch)
//	.def("getTemporaryQAtomMatch",&FrameRecognizer_O::getTemporaryQAtomMatch)
	.def("getRecognizerName",&FrameRecognizer_O::getRecognizerName)
	.def("compileSmarts",&FrameRecognizer_O::compileSmarts)
	;
#endif
//    boost::python::def("create_FrameRecognizer", &FrameRecognizer_O::create );
#endif
}


    EXPOSE_CLASS(chem,FrameRecognizer_O);
};




