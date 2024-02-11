#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <queue>

#include "../headers/Node.h"
#include "../constants/EventConstants.h"
#include "Utility.h"
#include "../constants/SimulationParameters.h"

using namespace std;


class Event{
    public:
    typedef priority_queue<Event> EventQueue;
    Event();
    Event(vector<Node*> nodes, double time, event_type type, EventQueue event_queue) : nodes(nodes), time(time), type(type), event_queue(event_queue) {}

    vector<Node*> nodes;
    event_type type;
    priority_queue<Event> event_queue;

    double time;
};


class EventComparator {
public:
    bool operator()(Event& a, Event& b)
    {
        if (a.time < b.time) {
            return true;
        }
        return false;
    }
};

class MineBlock : public Event {
    public:
    int miner_id;
    int prev_block_id;

    MineBlock(vector<Node*> &nodes, double time, event_type type, int miner_id, int mine_on_block_id) : Event(nodes, time, type), miner_id(miner_id), prev_block_id(prev_block_id){}

    void processEvent();

};


class ReceiveBlock : public Event {
    public:
    int receiver_id;
    Block incoming_block;
    int sender_id;

    ReceiveBlock();
    ReceiveBlock(vector<Node*> &nodes, double time, event_type type, int receiver_id, int sender_id, Block incoming_block) : Event(nodes, time, type), receiver_id(receiver_id), incoming_block(incoming_block) {}

    void processEvent();
    

    private:
        bool verifyBlock();
        void updateTransactionPool();
        void receiveBlock();

};


class ReceiveTransaction : public Event
{
public:
    Node* receiver;
    Node* sender;
    Transaction txn;

    ReceiveTransaction(Node* receiver, Node* sender, Transaction txn, vector<Node*> &nodes, event_type type, double time) : Event(nodes, time, type), receiver(receiver), sender(sender), txn(txn) {}
    
    //
    void processEvent();
    

    // ----------------------------- This funtiom receives transation and updates Node's local data --------------------------
    int receive();

    // ------------------------------------------------------------------------------------------------
    // This function will check all the neighbours of the transmitter,(exclude the sender)
    // compute latency delay for each,
    // and push "ReceiveTransaction" event in the event queue 
    // ------------------------------------------------------------------------------------------------
    void transmit();

};


class GenerateTransaction : public Event
{

public:
    Node *generator;

    GenerateTransaction(Node *generator, vector<Node*> &nodes, event_type type, double time) : Event (nodes, time, type), generator(generator) {}

    //
    void processEvent();
    
    // ------------------------------------------------------------------------------------------------ 
    //this function creates transaction object and add it to generator node's transaction pool 
    //-------------------------------------------------------------------------------------------------
    Transaction generate();


    // ------------------------------------------------------------------------------------------------
    // This function will check all the neighbours of the transmitter,
    // compute latency delay for each 
    // and push "ReceiveTransaction" event in the event queue 
    // ------------------------------------------------------------------------------------------------
    void transmit(Transaction txn);

    // -------------------- Generate next transaction after some delay ---------------------------------- 
    void initializeNextTransaction();
};

#endif