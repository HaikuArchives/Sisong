
/*
	IComm interface for component replicants
	(to be placed into an editor)
*/

#include <stdio.h>

#include <Application.h>
#include <View.h>
#include <Window.h>
#include <Message.h>
#include <Shelf.h>

#include "IComm.h"
#include "ICommClient.h"


// instantiate the IComm interface.
ICommClient::ICommClient(BMessenger serverLink)
{
	fServer = serverLink;
	
	printf("ICC Uplink initiated\n");
	fflush(stdout);
}

ICommClient::~ICommClient()
{
}


/*
void c------------------------------() {}
*/

int ICommClient::GetCurrentLine()
{
	return SendMessageGetInt(MIC_GET_CURRENT_LINE);
}

int ICommClient::GetCurrentCol()
{
	return SendMessageGetInt(MIC_GET_CURRENT_COL);
}



/*
void c------------------------------() {}
*/

void ICommClient::SendMessageCommand(uint command, BMessage *reply)
{
	BMessage msg(command);
	fServer.SendMessage(&msg, reply, MESSAGE_TIMEOUT, MESSAGE_TIMEOUT);
}

int ICommClient::SendMessageGetInt(uint command)
{
BMessage replymsg;
int32 reply;

	SendMessageCommand(command, &replymsg);
	
	if (replymsg.FindInt32("reply", &reply) == B_OK)
		return reply;
	else
		return -1;
}










