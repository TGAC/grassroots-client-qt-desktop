#ifndef UNSIGNED_INT_VALIIDATOR_H
#define UNSIGNED_INT_VALIIDATOR_H

#include "QValidator"

class UnsignedIntValidator : public QValidator
{
public:
	UnsignedIntValidator (QWidget *parent_p = nullptr);
	virtual ~UnsignedIntValidator ();
};

#endif // UNSIGNED_INT_VALIIDATOR_H
