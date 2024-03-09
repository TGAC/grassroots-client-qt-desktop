#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include "globus_auth.hpp"

#include <QObject>

 class Receiver : public QObject
{
	Q_OBJECT

public:
	Receiver (GlobusAuth *globus_p);
	virtual ~Receiver ();

public slots:
	void ReceiveToken (QString token);
};


#endif // RECEIVER_HPP
