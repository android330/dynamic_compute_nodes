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
    }

    char buf[sizeof(int)*3+2];
    for(int i = 0; i < CONNECTION_QUANTITY; i++){
        snprintf(buf, sizeof buf, "%d",ACCEPTEDCONNECTION_);
        this->context_->connections.at(i)->setMessage(buf);
        this->context_->connections.at(i)->sendMessage();
        if(this->context_->connections.at(i)->getConnection()[0] - 48 == ACCEPTEDCONNECTION_){
            std::cout << "Transition to Operate State" << std::endl;
            this->context_->TransitionTo(new Operate);
            this->context_->job = Job();
            this->context_->job.startRun();
            return;;
        }
    }

}

void Operate::onRun()
{
    sleep(5);
    std::cout << "In Data Send Phase" << std::endl;
    if (digitalRead(8) == 1)
      {
          std::cout << "Transition to DataSend State" << std::endl;
          this->context_->TransitionTo(new DataSend);
          return;
      }


    for(int i = 0; i < this->context_->connections.size(); i++)
    {
        if(this->context_->connections.at(i)->getConnection()[0] - 48 == DATASEND_)
        {
            this->context_->job.recievedData((std::string) this->context_->connections.at(i)->getConnection());
        }
    }
}

void DataSend::onRun()
{
    for(int i = 0; i < CONNECTION_QUANTITY; i++)
    {
        this->context_->connections.at(i)->setMessage(this->context_->job.send());
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
    wiringPiSetup();
    pinMode(8, INPUT);
    Node_Context *nc = new Node_Context(new Idle, argv[1]);
    while (true)
        nc->start();
}
