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
        Event(double time, event_type type);
        virtual ~Event();
        event_type type;

        virtual void processEvent() const;
        
        double time;

        virtual void printEvent() const;
};

// Custom comparator for priority_queue
struct EventComparator {
    bool operator()(const Event* event1, const Event* event2) const;
};


class MineBlock : public Event {
    public:
    int miner_id;
    Block new_block;

    MineBlock(double time, event_type type, int miner_id, Block new_block);
    virtual ~MineBlock();


    void processEvent() const override;

    void printEvent() const override;

    void transmitBlock(Block block) const;
};


class ReceiveBlock : public Event {
    public:
    int receiver_id;
    Block incoming_block;
    int sender_id;

    ReceiveBlock(double time, event_type type, int receiver_id, int sender_id, Block incoming_block);

    virtual ~ReceiveBlock();

    void processEvent() const override;
    void printEvent() const override;

    private:
        bool verifyBlock(vector<double> &balances) const;
        void updateTransactionPool() const;
        void receiveBlock() const;

};


class ReceiveTransaction : public Event
{
public:
    Node* receiver;
    Node* sender;
    const Transaction txn;

    ReceiveTransaction(
        Node* receiver, Node* sender, Transaction txn, event_type type, double time
    );

    virtual ~ReceiveTransaction();
    
    //
    void processEvent() const override;
    void printEvent() const override;

    // ----------------------------- This funtiom receives transation and updates Node's local data --------------------------
    int receive() const;

    // ------------------------------------------------------------------------------------------------
    // This function will check all the neighbours of the transmitter,(exclude the sender)
    // compute latency delay for each,
    // and push "ReceiveTransaction" event in the event queue 
    // ------------------------------------------------------------------------------------------------
    void transmit() const;

};


class GenerateTransaction : public Event
{

public:
    Node *generator;

    GenerateTransaction(Node *generator, event_type type, double time);
    virtual ~GenerateTransaction();

    //
    void processEvent() const override;
    void printEvent() const override;
    
    // ------------------------------------------------------------------------------------------------ 
    //this function creates transaction object and add it to generator node's transaction pool 
    //-------------------------------------------------------------------------------------------------
    Transaction generate() const;


    // ------------------------------------------------------------------------------------------------
    // This function will check all the neighbours of the transmitter,
    // compute latency delay for each 
    // and push "ReceiveTransaction" event in the event queue 
    // ------------------------------------------------------------------------------------------------
    void transmit(Transaction txn) const;

    // -------------------- Generate next transaction after some delay ---------------------------------- 
    void initializeNextTransaction() const;
};

#endif