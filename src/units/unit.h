#ifndef	_units_Unit_H
#define _units_Unit_H

#include "core/common.h"
#include "unitsPackage.h"
#include "dimension.h"
#include "dimension.fwd.h"
namespace units
{

FORWARD(Unit);
class Unit_O : public core::T_O
{
    friend class NamedUnit_O;
    friend class Quantity_O;
    LISP_BASE1(core::T_O);
    LISP_CLASS(units,UnitsPkg,Unit_O,"Unit");
//    DECLARE_ARCHIVE();
//    DECLARE_SERIALIZE();
    DEFAULT_CTOR_DTOR(Unit_O);
public:
	void initialize();

public:
    static Unit_sp make(core::Cons_sp args);
public:
    // instance variables here
	/*! Store the inverse scale factor for this Unit */
	double			_Amount;
	/*! Store the powers into the SIUnits  */
	int			_Powers[NumBaseDimensions];

public:
	static Unit_sp create( Unit_sp orig, int power, core::Lisp_sp lisp);
	static Unit_sp createSquareRoot( Unit_sp orig, core::Lisp_sp lisp);

public:
	/*! Parse a list of units with optional powers into an amountScale and powers */
	static void parseUnitList(double& amountScale, int powers[], core::Cons_sp unitList);
	void incorporateUnit(Unit_sp unit, double amountScale, int power);

	void adjustPowersAndAmountScale(int power, int powers[], double& amountScale) const;

public: // Functions here

	string __repr__() const;

	/*! For testing only! - set amount by hand to check conversions from one unit to another */
	void test_set_amount(double amount);

	Unit_sp copyWithoutName() const;

	/*! Return true if units are compatible (same powers) */
	bool is_compatible(Unit_sp other, int power=1) const;

	/*! Return the amount to scale myself to be in the units other.
	  The optional power value is a convenience to quickly change
	   convert units into reciprocal or other units
	    */
	double conversion_factor_to(Unit_sp other, int power=1) const;

	/*! Return a string representing the unts only */
	string unitsOnlyAsString() const;

	/*! Construct a new Unit by multiplying this by other */
	core::T_sp operator*(core::T_sp other) const;

	/*! Construct a new Unit by dividing this by other */
	core::T_sp operator/(core::T_sp other) const;

	/*! Construct a new Unit by taking the power of this by an integer */
	Unit_sp power(int pow) const;

	/*! Construct a new Unit by taking the sqrt of this,
	 Throw if the Powers aren't multiples of 2 */
	Unit_sp sqrt() const;

#if 0	

	/*! Return if this Unit has dimensions that match the other Unit */
	bool matches(Unit_sp other) const;

#endif


//	void adjustPowersAndAmountScale(int power, int powers[],double& amountScale) const;


};

}; /* units */

TRANSLATE(units::Unit_O);

#endif /* _units_Unit_H */


