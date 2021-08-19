//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __HOMATRANSPORT_WORKLOADSYNTHESIZER_H_
#define __HOMATRANSPORT_WORKLOADSYNTHESIZER_H_

#include <omnetpp.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include "networklayer/contract/IPvXAddress.h"
#include "common/Minimal.h"
#include "application/MsgSizeDistributions.h"
#include "application/AppMessage_m.h"
#include "transport/HomaConfigDepot.h"

/**
 * Mocks message generating behaviour of an application. Given a messge size
 * distribution and a interarrival time distribution, this module generates
 * message sizes of that distribution and sends them in interarrival times
 * sampled from the interarrival distribution.
 */
class MimicSynthesizer : public cSimpleModule
{
  PUBLIC:
    MimicSynthesizer();
    ~MimicSynthesizer();

  PROTECTED:
    enum SelfMsgKinds { START = 1, SEND, STOP };

    // generates samples from the a given random distribution
    // MsgSizeDistributions *msgSizeGenerator;

    /*******************************
     *          parameters         *
     *******************************/
    // contains destination addresses as specified in config.xml for each host.
    IPvXAddress destAddress;
    simtime_t startTime;
    simtime_t stopTime;
    bool isSender;
    struct Command
    {
        simtime_t tSend;
        long numBytes;
        Command(simtime_t t, long n) {tSend=t; numBytes=n;}
    };
    typedef std::vector<Command> CommandVector;
    CommandVector commands;
    // int maxDataBytesPerPkt;
    // int maxDataBytesPerEthFrame;
    cXMLElement* xmlConfig;
    // uint32_t nicLinkSpeed; // in Gb/s
    // uint32_t fabricLinkSpeed;
    // double fabricLinkDelay;
    // double edgeLinkDelay;
    // double hostSwTurnAroundTime;
    // double hostNicSxThinkTime;
    // double switchFixDelay;
    // bool isFabricCutThrough;
    // bool isSingleSpeedFabric;
    int parentHostIdx;
    int commandIndex;

    // states
    cMessage* selfMsg;
    IPvXAddress srcAddress;
    //int sendMsgSize; // In bytes
    // int nextDestHostId; // -1 means the destination must be chosen
                        // randomely based on config.xml information.

    // statistics
    // int numSent;
    // int numReceived;

    static simsignal_t sentMsgSignal;
    static simsignal_t rcvdMsgSignal;

    // Signal for end to end to delay every received messages
    //static simsignal_t msgE2EDelaySignal;

    // keep pairs of upper bound message size range and its corresponding
    // signal id.
    //std::vector<uint64_t> msgSizeRangeUpperBounds;
    //std::vector<simsignal_t> msgBytesOnWireSignalVec;
    //std::vector<simsignal_t> msgE2ELatencySignalVec;
    //std::vector<simsignal_t> msgE2EStretchSignalVec;
    //std::vector<simsignal_t> msgQueueDelaySignalVec;
    //std::vector<simsignal_t> msgTransprotSchedDelaySignalVec;

    // This signal is emitted with a pointer to an object of type MesgStats.
    // The object contains all end-to-end stats for a message that is just
    // completed. The GlobalSignalListener is listener to this signal and
    // consumer of the object.
    //static simsignal_t mesgStatsSignal;

  PROTECTED:
    virtual void initialize();
    virtual void handleMessage(cMessage* msg);
    virtual void finish();

    void processStart();
    void processSend();
    void processStop();
    void processRcvdMsg(cPacket* msg);
    void sendMsg(long numBytes);
    void parseScript(const char *script);
    //void registerTemplatedStats(const char* msgSizeRanges);
    //double idealMsgEndToEndDelay(AppMessage* rcvdMsg);
};
/*
class MesgStats : public cObject, noncopyable
{
  PUBLIC:
    MesgStats(){}

  PUBLIC:
    uint64_t mesgSize;
    uint64_t mesgSizeOnWire;
    uint64_t mesgSizeBin;
    simtime_t latency;
    double stretch;
    double queuingDelay;
    simtime_t transportSchedDelay;
};
*/
#endif //__HOMATRANSPORT_WORKLOADSYNTHESIZER_H_
