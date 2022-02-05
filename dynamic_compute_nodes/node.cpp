//created by ANdrew

#include "node.h"

void Idle::onRun()
{
    if(!startup)
    {
        nodeConnection* temp;
        for (int i = 0; i < CONNECTION_QUANTITY; i++){
            temp = new nodeConnection(this->context_->IPS[i], this->context_->PORTS[i]);
            this->context_->connections.push_back(temp);
        }
        startup = true;
        start = clock();
    }

    char buf[sizeof(int)*3+2];
    for(int i = 0; i < CONNECTION_QUANTITY; i++){
        snprintf(buf, sizeof buf, "%d",SEEKINGCONNECTION_);
        this->context_->connections.at(i)->setMessage(buf);
        this->context_->connections.at(i)->sendMessage();

        //if other connection was previously running, then new job created with startup information TODO
        if(this->context_->connections.at(i)->getConnection()[0] - 48 == ACCEPTEDCONNECTION_){
            //signal to connecitng node that connection was accepted
            snprintf(buf, sizeof buf, "%d", ACCEPTEDCONNECTION_);
            this->context_->connections.at(i)->setMessage(buf);
            this->context_->connections.at(i)->sendMessage();

            std::cout << "Transition to Operate State" << std::endl;
            this->context_->job = new Job();
            this->context_->job->startRun();
	    this->context_->TransitionTo(new Operate);
            return;
        }

        //if other connection was also on startup seeking conneciton, then new job created
        if (this->context_->connections.at(i)->getConnection()[0] - 48 == SEEKINGCONNECTION_) {
            snprintf(buf, sizeof buf, "%d", SEEKINGCONNECTION_);
            this->context_->connections.at(i)->setMessage(buf);
            this->context_->connections.at(i)->sendMessage();

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
    
    //std::cout << "In Operate Phase" << std::endl;

#ifdef __arm__
    if (digitalRead(8) == 1)
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
           // this->context_->job->recievedData((std::string) this->context_->connections.at(i)->getConnection());
        }

        //if other conneciton is seeking to create connection then send job information TODO, and signal accepted connection
        if (this->context_->connections.at(i)->getConnection()[0] - 48 == SEEKINGCONNECTION_) {
            //signals to other connection that it has been recognized
            snprintf(buf, sizeof buf, "%d", ACCEPTEDCONNECTION_);
            this->context_->connections.at(i)->setMessage(buf);
            this->context_->connections.at(i)->sendMessage();
        }
    }
}

void DataSend::onRun()
{
    for(int i = 0; i < CONNECTION_QUANTITY; i++)
    {
        this->context_->connections.at(i)->setMessage(context_->job->send().c_str());
        this->context_->connections.at(i)->sendMessage();
        this->context_->TransitionTo(new Stop);
    }
}

void Stop::onRun()
{
    std::cout << "Program Exiting" << std::endl;
    exit(1);
}

int main(int argc, char const *argv[]){
#ifdef __arm__
    wiringPiSetup();
    pinMode(8, INPUT);
#endif
    Node_Context *nc = new Node_Context(new Idle, argv[1]);
    while (true)
        nc->start();
}
