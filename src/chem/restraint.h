//
// (C) 2004 Christian E. Schafmeister
//


/*
 *	restraint.h
 *
 *	Maintain a Restraint object.
 *
 *
 */

#ifndef RESTRAINT_H
#define	RESTRAINT_H

#include <iostream>
#include <string>
#include <vector>
#include "core/common.h"
#include "addon/vector3.h"
#include "core/sequence.h"
#include "core/holder.h"


#include "chemPackage.h"


namespace chem {


SMART(Atom);
SMART(Matter);
SMART(Aggregate);
SMART(Restraint);

SMART(Restraint );
class Restraint_O : public core::T_O
{
    LISP_BASE1(core::T_O);
    LISP_CLASS(chem,ChemPkg,Restraint_O,"Restraint");
public:
//    void	archiveBase(core::ArchiveP node);
//    void	serialize(serialize::SNode node);

private:
public:

	virtual Restraint_sp	copyDontRedirectAtoms() {_OF(); SUBCLASS_MUST_IMPLEMENT();};
	virtual void redirectAtoms() {_OF(); SUBCLASS_MUST_IMPLEMENT();};

	virtual void invertStereochemistryOfRestraint() {_OF();SUBCLASS_MUST_IMPLEMENT();};




	DEFAULT_CTOR_DTOR(Restraint_O);
};



SMART(RestraintAnchor);
class RestraintAnchor_O : public Restraint_O
{
    LISP_BASE1(Restraint_O);
    LISP_CLASS(chem,ChemPkg,RestraintAnchor_O,"RestraintAnchor");
#if INIT_TO_FACTORIES
 public:
    static RestraintAnchor_sp make();
#else
    DECLARE_INIT();
#endif

public:
//	void	archiveBase(core::ArchiveP node);
//	void	serialize(serialize::SNode node);

private:
	Atom_wp		_A;
	Vector3		_AnchorPos;
	double		_Weight;
public:

	void setWeight(double d) { this->_Weight = d; };
	double getWeight() { return this->_Weight; };

	void	setAnchorPos(const Vector3& pos )
	{
	    this->_AnchorPos = pos;
	};
	Atom_sp	getAtom() {_OF();ASSERTNOTNULL(this->_A);return this->_A.lock();};
	void	setAtom(Atom_sp a) {this->_A=a;};

	Vector3 getAnchorPos() { return this->_AnchorPos; };

	Restraint_sp	copyDontRedirectAtoms();
	void redirectAtoms();

    	void invertStereochemistryOfRestraint();


	RestraintAnchor_O(const RestraintAnchor_O& old);
	DEFAULT_CTOR_DTOR(RestraintAnchor_O);
};



SMART(RestraintChiral);
class RestraintChiral_O : public Restraint_O
{
    LISP_BASE1(Restraint_O);
    LISP_CLASS(chem,ChemPkg,RestraintChiral_O,"RestraintChiral");

public:
//    void	archiveBase(core::ArchiveP node);
//    void	serialize(serialize::SNode node);

private:
	Atom_wp		_A;
	int		_Chirality;	// S=1,R=-1

public:

	void	setParameters(int c)
	{
	    this->_Chirality = c;
	};
	Atom_sp	getAtomA() {_OF();ASSERTNOTNULL(this->_A);return this->_A.lock();};
	void	setAtomA(Atom_sp a) {this->_A=a;};

	int	getChirality() { return this->_Chirality; };
	void	setChirality(int c) { this->_Chirality = c; };

	Restraint_sp	copyDontRedirectAtoms();
	void redirectAtoms();

    	void invertStereochemistryOfRestraint();

	RestraintChiral_O(const RestraintChiral_O& old);
	DEFAULT_CTOR_DTOR(RestraintChiral_O);
};





SMART(RestraintDistance);
class RestraintDistance_O : public Restraint_O
{
    LISP_BASE1(Restraint_O);
    LISP_CLASS(chem,ChemPkg,RestraintDistance_O,"RestraintDistance");


public:
//    void	archiveBase(core::ArchiveP node);
//    void	serialize(serialize::SNode node);

private:
	Atom_wp		_A;
	Atom_wp		_B;
	double		_Min;
	double		_Max;
	double		_Weight;

public:


	void	setParameters(double mn,double mx, double wt ) {
			this->_Min=mn;
			this->_Max=mx;
			this->_Weight=wt;
	};
	Atom_sp	getAtomA() {_OF();ASSERTNOTNULL(this->_A);return this->_A.lock();};
	void	setAtomA(Atom_sp a) {this->_A=a;};
	Atom_sp	getAtomB() {_OF();ASSERTNOTNULL(this->_B);return this->_B.lock();};
	void	setAtomB(Atom_sp a) {this->_B=a;};

	double	getMin() {return this->_Min;};
	double	getMax() {return this->_Max;};
	double	getWeight() {return this->_Weight;};

	Restraint_sp	copyDontRedirectAtoms();
	void redirectAtoms();

    	void invertStereochemistryOfRestraint() { /* Nothing */ };


	RestraintDistance_O(const RestraintDistance_O& old);
	DEFAULT_CTOR_DTOR(RestraintDistance_O);
};


SMART(RestraintAngle);
class RestraintAngle_O : public Restraint_O
{
    LISP_BASE1(Restraint_O);
    LISP_CLASS(chem,ChemPkg,RestraintAngle_O,"RestraintAngle");
public:
//    void	archiveBase(core::ArchiveP node);
//    void	serialize(serialize::SNode node);

private:
	Atom_wp		_A;
	Atom_wp		_B;
	Atom_wp		_C;
	double		_MinRad;
	double		_MaxRad;
	double		_Weight;

public:


	void	setParameters(double mn,double mx, double wt ) {
			this->_MinRad=mn;
			this->_MaxRad=mx;
			this->_Weight=wt;
	};
	Atom_sp	getAtomA() {_OF();ASSERTNOTNULL(this->_A);return this->_A.lock();};
	void	setAtomA(Atom_sp a) {this->_A=a;};
	Atom_sp	getAtomB() {_OF();ASSERTNOTNULL(this->_B);return this->_B.lock();};
	void	setAtomB(Atom_sp a) {this->_B=a;};
	Atom_sp	getAtomC() {_OF();ASSERTNOTNULL(this->_C);return this->_C.lock();};
	void	setAtomC(Atom_sp a) {this->_C=a;};

	double	getMin() {return this->_MinRad;};
	double	getMax() {return this->_MaxRad;};
	double	getWeight() {return this->_Weight;};

	Restraint_sp	copyDontRedirectAtoms();
	void redirectAtoms();

    	void invertStereochemistryOfRestraint() { /* Nothing */ };


	RestraintAngle_O(const RestraintAngle_O& old);
	DEFAULT_CTOR_DTOR(RestraintAngle_O);
};




SMART(RestraintDihedral);
class RestraintDihedral_O : public Restraint_O
{
    LISP_BASE1(Restraint_O);
    LISP_CLASS(chem,ChemPkg,RestraintDihedral_O,"RestraintDihedral");

public:
//    void	archiveBase(core::ArchiveP node);
//    void	serialize(serialize::SNode node);

protected:
	Atom_wp		_A;
	Atom_wp		_B;
	Atom_wp		_C;
	Atom_wp		_D;
		/*! I think these should be between 0 and 360
		 * But look at stereochemistry.cc line 211 transMin/transMax
		 * and cisMin/cisMax are between -180 and 180
		 */
	double		_MinDegrees;
		/*! I think these should be between 0 and 360
		 * But look at stereochemistry.cc line 211 transMin/transMax
		 * and cisMin/cisMax are between -180 and 180
		 */
	double		_MaxDegrees;
	double		_Weight;

public:


	void	setParameters(double mn,double mx, double wt ) {
			this->_MinDegrees=mn;
			this->_MaxDegrees=mx;
			this->_Weight=wt;
	};
	Atom_sp	getAtomA() {_OF();ASSERTNOTNULL(this->_A);return this->_A.lock();};
	Atom_sp	getAtomB() {_OF();ASSERTNOTNULL(this->_B);return this->_B.lock();};
	Atom_sp	getAtomC() {_OF();ASSERTNOTNULL(this->_C);return this->_C.lock();};
	Atom_sp	getAtomD() {_OF();ASSERTNOTNULL(this->_D);return this->_D.lock();};
	void	setAtomA(Atom_sp a) {this->_A=a;};
	void	setAtomB(Atom_sp a) {this->_B=a;};
	void	setAtomC(Atom_sp a) {this->_C=a;};
	void	setAtomD(Atom_sp a) {this->_D=a;};

	double	getMinDegrees() {return this->_MinDegrees;};
	double	getMaxDegrees() {return this->_MaxDegrees;};
	double	getWeight() {return this->_Weight;};

	Restraint_sp	copyDontRedirectAtoms();
	void redirectAtoms();

    	void invertStereochemistryOfRestraint();


	RestraintDihedral_O(const RestraintDihedral_O& old);
	DEFAULT_CTOR_DTOR(RestraintDihedral_O);
};



SMART(RestraintFixedNonbond);
class RestraintFixedNonbond_O : public Restraint_O
{
    LISP_BASE1(Restraint_O);
    LISP_CLASS(chem,ChemPkg,RestraintFixedNonbond_O,"RestraintFixedNonbond");

public:
	void	initialize();
//	void	archiveBase(core::ArchiveP node);
//	void	serialize(serialize::SNode node);
protected:
	Matter_sp	_Matter;
public:
    static RestraintFixedNonbond_sp create(core::Lisp_sp e,Matter_sp matter);
public:
	void setMatter(Matter_sp matter) { this->_Matter = matter;};
	Matter_sp getMatter() { return this->_Matter;};

	RestraintFixedNonbond_O(const RestraintFixedNonbond_O& old);
	DEFAULT_CTOR_DTOR(RestraintFixedNonbond_O);
};






SMART(RestraintList);
class RestraintList_O : public core::Vector_O
{
    LISP_BASE1(core::Vector_O);
    LISP_CLASS(chem,ChemPkg,RestraintList_O,"RestraintList");

public:
//    void	archiveBase(core::ArchiveP node);
//    void	serialize(serialize::SNode node);

private:
    gctools::Vec0<Restraint_sp>	_Restraints;
public:
    typedef gctools::Vec0<Restraint_sp>::iterator	iterator;
    typedef gctools::Vec0<Restraint_sp>::const_iterator	const_iterator;
public:
    gctools::Vec0<Restraint_sp>::iterator	begin() { return this->_Restraints.begin(); };
    gctools::Vec0<Restraint_sp>::iterator	end() { return this->_Restraints.end(); };
    gctools::Vec0<Restraint_sp>::const_iterator	begin() const { return this->_Restraints.begin(); };
    gctools::Vec0<Restraint_sp>::const_iterator	end() const { return this->_Restraints.end(); };

	void		clear()	{this->_Restraints.clear(); };
	void		addRestraint( Restraint_sp r) { this->_Restraints.push_back(r); };
	int		size()	{return this->_Restraints.size(); };

	Restraint_sp	getRestraintIndex(int i) {return this->_Restraints[i]; };

	RestraintList_sp	copyDontRedirectAtoms();
	void	redirectAtoms();

	void	merge(RestraintList_sp rl);

//	RestraintList_O(const RestraintList_O& rl );




    DEFAULT_CTOR_DTOR(RestraintList_O);
};





};
TRANSLATE(chem::Restraint_O);
TRANSLATE(chem::RestraintAnchor_O);
TRANSLATE(chem::RestraintChiral_O);
TRANSLATE(chem::RestraintDistance_O);
TRANSLATE(chem::RestraintAngle_O);
TRANSLATE(chem::RestraintDihedral_O);
TRANSLATE(chem::RestraintFixedNonbond_O);
TRANSLATE(chem::RestraintList_O);
#endif
