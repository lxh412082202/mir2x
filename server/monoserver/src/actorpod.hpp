/*
 * =====================================================================================
 *
 *       Filename: actorpod.hpp
 *        Created: 04/20/2016 21:49:14
 *  Last Modified: 05/02/2016 00:16:29
 *
 *    Description: why I made actor as a plug, because I want it to be a one to zero/one
 *                 mapping as ServerObject -> Actor
 *
 *                 Then a server object can  plug to the actor system slot for concurrent
 *                 operation, but also some server objects don't need this functionality
 *
 *                 And what's more, if an object is both ServerObject and Actor, we need
 *                 MI, but I really don't want to use MI
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */
#pragma once

#include <functional>
#include <unordered_map>
#include <Theron/Theron.h>

#include "monoserver.hpp"
#include "messagepack.hpp"

class ActorPod: public Theron::Actor
{
    private:
        using MessagePackOperation
            = std::function<void(const MessagePack&, const Theron::Address &)>;
        // no need to keep the message pack itself
        // since when registering response operation, we always have the message pack avaliable
        typedef struct _RespondMessageRecord {
            // MessagePack RespondMessagePack;
            MessagePackOperation    RespondOperation;

            // _RespondMessageRecord(const MessagePack & rstMPK,
            //         const MessagePackOperation &rstOperation)
            //     : RespondMessagePack(rstMPK)
            //     , RespondOperation(rstOperation)
            // {}
            _RespondMessageRecord(const MessagePackOperation &rstOperation)
                : RespondOperation(rstOperation)
            {}
        } RespondMessageRecord;
    protected:
        size_t m_ValidID;
        MessagePackOperation m_Operate;
        std::unordered_map<uint32_t, RespondMessageRecord> m_RespondMessageRecordM;

    public:
        explicit ActorPod(Theron::Framework *pFramework,
                const std::function<void(const MessagePack &, const Theron::Address &)> &fnOperate)
            : Theron::Actor(*pFramework)
            , m_ValidID(0)
            , m_Operate(fnOperate)
        {
            RegisterHandler(this, &ActorPod::InnHandler);
        }

        virtual ~ActorPod() = default;

    protected:
        using Theron::Actor::Send;

    protected:
        void InnHandler(const MessagePack &rstMPK, Theron::Address stFromAddr)
        {
            if(rstMPK.Respond()){
                auto pRecord = m_RespondMessageRecordM.find(rstMPK.Respond());
                if(pRecord != m_RespondMessageRecordM.end()){
                    // we do have an record for this message
                    if(pRecord->second.RespondOperation){
                        try{
                            pRecord->second.RespondOperation(rstMPK, stFromAddr);
                        }catch(...){
                            extern MonoServer *g_MonoServer;
                            g_MonoServer->AddLog(LOGTYPE_WARNING,
                                    "caught exception in operating response message");
                        }
                    }else{
                        extern MonoServer *g_MonoServer;
                        g_MonoServer->AddLog(LOGTYPE_WARNING,
                                "registered response operation is not callable");
                    }
                    m_RespondMessageRecordM.erase(pRecord);
                }else{
                    extern MonoServer *g_MonoServer;
                    g_MonoServer->AddLog(LOGTYPE_WARNING,
                            "no registered operation for response message found");
                }
                // TODO & TBD
                // we ignore it or send it to m_Operate??? currently just dropped
                return;
            }

            // now message are handling  not on purpose of response only
            if(m_Operate){
                // in theron address is recommanded to copy rather than ref, but
                // here we use const ref when passing to m_Operate, because when
                // calling m_Operate(), address already has a copy when executaion
                // goes inside InnHandler(), so always there is a valid copy of
                // address for m_Operate()
                //
                // if m_Operate() has any other async\ed operation upon the address
                // inside, it should handler by itself.
                //
                // so there is only one copy of InnHandler() when callback invoked, and
                // don't worry, Theron::Address is actually only a pointer so copying
                // is really cheap
                //
                // for MessagePack copying, since Theron itself using const ref, so I
                // use const ref here
                try{
                    m_Operate(rstMPK, stFromAddr);
                }catch(...){
                    // 1. assume monoserver is ready when invoking callback
                    // 2. AddLog() is well defined in multithread environment
                    extern MonoServer *g_MonoServer;
                    g_MonoServer->AddLog(LOGTYPE_WARNING, "caught exception in ActorPod");
                }
            }else{
                // TODO & TBD
                // this message will show up many and many if not valid handler found
                // extern MonoServer *g_MonoServer;
                // g_MonoServer->AddLog(LOGTYPE_WARNING,
                //         "registered operation for message is not callable");
            }

        }

    public:
        // TODO may be I need to comment it out
        // send without exptecting a reply, ID() in rstMSG will be ignored
        bool Send(MessagePack stMSG, const Theron::Address &rstFromAddress)
        {
            stMSG.ID(0);
            return Theron::Actor::Send(stMSG, rstFromAddress);
        }

        // send with exptection of replying
        // external code has no idea of what ID we'll assign for it
        // so we need to make a copy and assign ID for it
        bool Send(MessagePack stMSG, const Theron::Address &rstFromAddress,
                const std::function<void(const MessagePack&, const Theron::Address &)> &fnOPR)
        {
            m_ValidID = (m_RespondMessageRecordM.empty() ? 1 : m_ValidID + 1);
            auto pRecord = m_RespondMessageRecordM.find(m_ValidID);
            if(pRecord != m_RespondMessageRecordM.end()){
                extern MonoServer *g_MonoServer;
                g_MonoServer->AddLog(LOGTYPE_WARNING, "response requested message overflows");
                // TODO
                // this function won't return;
                g_MonoServer->Restart();
            }

            stMSG.ID(m_ValidID);

            // 2. send it
            bool bRet = ActorPod::Send(stMSG, rstFromAddress);

            // 3. if send succeed, then register the handler of responding message
            //    here we won't exam fnOPR's callability
            if(bRet){
                m_RespondMessageRecordM.emplace(std::make_pair(m_ValidID, fnOPR));
            }
            // 4. return whether we succeed
            return bRet;
        }
};