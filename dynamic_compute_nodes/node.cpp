/*
    node.cpp
    Andrew Beachemin
    android@vt.edu
    October 2021
*/

#include "node.h"

void Idle::onRun()
{
    //first time through startup initialization
    if(!startup)
    {
        //connect to adjacent nodes
        nodeConnection* temp;
        for (int i = 0; i < CONNECTION_QUANTITY; i++){
            temp = new nodeConnection(this->context_->IPS[i], this->context_->PORTS[i]);
            this->context_->connections.push_back(temp);
        }
	    std::cout << this->context_->TERMINAL_IP << std::endl;

        //terminal connection
	    this->context_->terminalConnection = new nodeConnection(this->context_->TERMINAL_IP, 8080);

        startup = true;
        start = clock();
    }

    //checks all connections for open connections
    for(int i = 0; i < CONNECTION_QUANTITY; i++){
        std::string recievedCrap = this->context_->connections.at(i)->getConnection();

        this->context_->connections.at(i)->setMessage(std::to_string(SEEKINGCONNECTION_).c_str());
        this->context_->connections.at(i)->sendMessage();

        //if other connection was previously running, then new job created with startup information
        if(recievedCrap[0] - 48 == ACCEPTEDCONNECTION_)
        {
            std::cout << "Transition to Operate State with data " << recievedCrap.substr(1, recievedCrap.length() - 1) << std::endl;

            this->context_->job = new Job(recievedCrap.substr(1, recievedCrap.length() - 1));
            this->context_->job->startRun();

            this->context_->terminalConnection->setMessage((this->context_->username + ": Operate State Transition" + recievedCrap.substr(1, recievedCrap.length() - 1)).c_str());
            this->context_->terminalConnection->sendMessage();


            //signal to connecitng node that connection was accepted
            std::string sendTemp = std::to_string(ACCEPTEDCONNECTION_) + context_->job->send();
            this->context_->connections.at(i)->setMessage(sendTemp.c_str());
            this->context_->connections.at(i)->sendMessage();

	        this->context_->TransitionTo(new Operate);
            return;
        }

        //if other connection was also on startup seeking conneciton, then new job created
        else if (recievedCrap[0] - 48 == SEEKINGCONNECTION_) 
        {
            this->context_->connections.at(i)->setMessage(std::to_string(SEEKINGCONNECTION_).c_str());
            this->context_->connections.at(i)->sendMessage();
            
            this->context_->terminalConnection->setMessage((this->context_->username + ": Operate State Transition - default").c_str());
            this->context_->terminalConnection->sendMessage();

            std::cout << "Transition to Operate State" << std::endl;
            this->context_->job = new Job();
            this->context_->job->startRun();
	        this->context_->TransitionTo(new Operate);
            return;
        }
    }

    //if 30 seconds elapsed transition to operate state anyway
    if ((double)(clock() - start) * 1000.0 / CLOCKS_PER_SEC > 30000) {
        printf("30 seconds elapsed - ");

        this->context_->terminalConnection->setMessage((this->context_->username + ": Operate State Transition - timeout").c_str());
        this->context_->terminalConnection->sendMessage();

        std::cout << "Transition to Operate State" << std::endl;
        this->context_->job = new Job();
        this->context_->job->startRun();
	    this->context_->TransitionTo(new Operate);
        return;
    }
}

void Operate::onRun()
{
    if (!startup) {
        start = clock();
        startup = true;
    }
#ifdef __arm__
    //transitions to data send if read low
    if (digitalRead(25) == 0)
      {
          this->context_->terminalConnection->setMessage((this->context_->username + ": DataSend State Transition - low power").c_str());
          this->context_->terminalConnection->sendMessage();
        
          std::cout << "Transition to DataSend State" << std::endl;
          this->context_->TransitionTo(new DataSend);
          return;
      }
#endif

    //inter nodal connection information network
    char buf[sizeof(int) * 3 + 2];
    for(int i = 0; i < this->context_->connections.size(); i++)
    {
        std::string sendTemp = this->context_->connections.at(i)->getConnection();

        //If other node is sending data
        if(sendTemp[0] - 48 == DATASEND_)
        {
            std::cout << "Recieved - " << sendTemp << std::endl;
	        if (sendTemp.length() > 1)
                this->context_->job->recievedData(sendTemp.substr(1, sendTemp.length() - 1));
        }

        //if other conneciton is seeking to create connection then send job information and signal accepted connection
        if (this->context_->connections.at(i)->getConnection()[0] - 48 == SEEKINGCONNECTION_) {
            //signals to other connection that it has been recognized
            snprintf(buf, sizeof buf, "%d", ACCEPTEDCONNECTION_);
            std::string sendTemp = std::to_string(ACCEPTEDCONNECTION_) + context_->job->send();
            this->context_->connections.at(i)->setMessage(sendTemp.c_str());
            this->context_->connections.at(i)->sendMessage();
        }
    }

    //if 10 seconds elapsed terminal checkin
    if ((double)(clock() - start) * 1000.0 / CLOCKS_PER_SEC > 10000) {
        this->context_->terminalConnection->setMessage((this->context_->username + ": Operate State Checkin - " + std::to_string(this->context_->job->getVal())).c_str());
        this->context_->terminalConnection->sendMessage(); 
        start = clock();
    }
}

void DataSend::onRun()
{
	//sends data to all nodes whether on or not
	for(int i = 0; i < CONNECTION_QUANTITY; i++)
    {
        
	    if (context_->job == nullptr) {
		    std::cout << "laid off";
		    continue;
	    }
	    std::string sendTemp = std::to_string(DATASEND_) + context_->job->send();
        std::cout << "Sent - " << sendTemp << std::endl;
        this->context_->terminalConnection->setMessage((this->context_->username + ": Stop State Transition - " + context_->job->send()).c_str());
        this->context_->terminalConnection->sendMessage();

        this->context_->connections.at(i)->setMessage(sendTemp.c_str());
        this->context_->connections.at(i)->sendMessage();
    }
    this->context_->TransitionTo(new Stop);
}

void Stop::onRun()
{
#ifdef __arm__
    //transition back to operate state when power turned back on
    if (digitalRead(25) == 1)
    {
        this->context_->terminalConnection->setMessage((this->context_->username + ": Operate State Transition - regular power").c_str());
        this->context_->terminalConnection->sendMessage();
        
        std::cout << "Power Levels Suitiable" << std::endl << "Transition back to Operate State" << std::endl;
        this->context_->TransitionTo(new Operate);
        return;
    }
#else
    //exti program if no pin to read
    std::cout << "Program Exiting" << std::endl;
    exit(1);
#endif
}

int main(int argc, char const *argv[]){
#ifdef __arm__
    wiringPiSetup();
    pinMode(25, INPUT);
#endif
    //char const 
    Node_Context *nc = new Node_Context(new Idle, argv);
    while (true)
        nc->start();
}
