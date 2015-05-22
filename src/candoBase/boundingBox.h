#ifndef	BoundingBox_H //[
#define BoundingBox_H



#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include "core/foundation.h"
#include "core/object.h"
#include "candoBasePackage.fwd.h"
#include "matrix.h"
#include "vector3.h"

namespace candoBase {

    FORWARD(BoundingBox);
    class BoundingBox_O : public core::T_O
    {
	LISP_BASE1(core::T_O);
	LISP_CLASS(candoBase,CandoBasePkg,BoundingBox_O,"BoundingBox");
    public:
	void	initialize();
//	void	archiveBase(core::ArchiveP node);
    private:
	// instance variables
	bool	_Defined;
	Vector3	_MinCorner;
	Vector3	_MaxCorner;
    public:
    public:
	bool isDefined() { return this->_Defined;};
	Vector3& minCorner() { return this->_MinCorner; };
	Vector3& maxCorner() { return this->_MaxCorner; };

	double getMinX() const;
	double getMaxX() const;
	double getMinY() const;
	double getMaxY() const;
	double getMinZ() const;
	double getMaxZ() const;

	Vector3 getCenter() const;
	double getExtentX() const;
	double getExtentY() const;
	double getExtentZ() const;

	bool encompasses(const Vector3& point );
	void expandToEncompassPoint(const Vector3& point );
	void expandToEncompassPoint(const Matrix& transform, const Vector3& point );
	void pad(double add);

	BoundingBox_O( const BoundingBox_O& ss ); //!< Copy constructor

	DEFAULT_CTOR_DTOR(BoundingBox_O);
    };


};
TRANSLATE(candoBase::BoundingBox_O);
#endif //]
