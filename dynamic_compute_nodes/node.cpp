//created by Andrew, Oct 2021

#include "node.h"

void Idle::onRun()
{
    //first time through startup initialization
    if(!startup)
    {
        nodeConnection* temp;
        for (int i = 0; i < CONNECTION_QUANTITY; i++){
            temp = new nodeConnection(this->context_->IPS[i], this->context_->PORTS[i]);
            this->context_->connections.push_back(temp);
        }
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
            std::cout << "Transition to Operate State" << std::endl;
            
            //std::cout << recievedCrap << std::endl;
            this->context_->job = new Job(recievedCrap.substr(1, recievedCrap.length() - 1));
            this->context_->job->startRun();


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
            
            this->context_->terminalConnection->setMessage("Connected to terminal");
            this->context_->terminalConnection->sendMessage();

            std::cout << "Transition to Operate State" << std::endl;
            this->context_->job = new Job();
            this->context_->job->startRun();
	        this->context_->TransitionTo(new Operate);
            return;
        }
    }

    //if 10 seconds elapsed
    if ((double)(clock() - start) * 1000.0 / CLOCKS_PER_SEC > 10000) {
        printf("10 seconds elapsed - ");
        std::cout << "Transition to Operate State" << std::endl;
        this->context_->job = new Job();
        this->context_->job->startRun();
	    this->context_->TransitionTo(new Operate);
        return;
    }
}

void Operate::onRun()
{

#ifdef __arm__
    if (digitalRead(25) == 0)
      {
          std::cout << "Transition to DataSend State" << std::endl;
          this->context_->TransitionTo(new DataSend);
          return;
      }
#endif

    char buf[sizeof(int) * 3 + 2];
    for(int i = 0; i < this->context_->connections.size(); i++)
    {
        if(this->context_->connections.at(i)->getConnection()[0] - 48 == DATASEND_)
        {
            std::string sendTemp = this->context_->connections.at(i)->getConnection();
	    if (sendTemp.length() > 1)
                this->context_->job->recievedData(sendTemp.substr(1, sendTemp.length() - 1));
        }

        //if other conneciton is seeking to create connection then send job information TODO, and signal accepted connection
        if (this->context_->connections.at(i)->getConnection()[0] - 48 == SEEKINGCONNECTION_) {
            //signals to other connection that it has been recognized
            snprintf(buf, sizeof buf, "%d", ACCEPTEDCONNECTION_);
            std::string sendTemp = std::to_string(ACCEPTEDCONNECTION_) + context_->job->send();
            this->context_->connections.at(i)->setMessage(sendTemp.c_str());
            this->context_->connections.at(i)->sendMessage();
        }
    }
}

void DataSend::onRun()
{
    
	
	for(int i = 0; i < CONNECTION_QUANTITY; i++)
    {
        
	if (context_->job == nullptr) {
		std::cout << "laid off";
		continue;
	}
	std::string sendTemp = std::to_string(DATASEND_) + context_->job->send();
        this->context_->connections.at(i)->setMessage(sendTemp.c_str());
        this->context_->connections.at(i)->sendMessage();
    }
    this->context_->TransitionTo(new Stop);
}

void Stop::onRun()
{
#ifdef __arm__
    if (digitalRead(25) == 1)
    {
        std::cout << "Power Levels Suitiable" << std::endl << "Transition back to Operate State" << std::endl;
        this->context_->TransitionTo(new Operate);
        return;
    }
#else
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
