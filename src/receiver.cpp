#include "receiver.hpp"

#include <QDebug>


Receiver ::	Receiver (GlobusAuth *globus_p)
{
	connect (globus_p, &GlobusAuth :: GotToken, this, &Receiver :: ReceiveToken);
}

Receiver :: ~Receiver ()
{

}


void Receiver :: ReceiveToken (QString token)
{
	qDebug () << "token: " << token << Qt :: endl;
}
