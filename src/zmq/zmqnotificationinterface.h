// Copyright (c) 2015-2017 The Machinecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MACHINECOIN_ZMQ_ZMQNOTIFICATIONINTERFACE_H
#define MACHINECOIN_ZMQ_ZMQNOTIFICATIONINTERFACE_H

#include <validationinterface.h>
#include <string>
#include <map>
#include <list>

class CBlockIndex;
class CZMQAbstractNotifier;

class CZMQNotificationInterface final : public CValidationInterface
{
public:
    virtual ~CZMQNotificationInterface();

    static CZMQNotificationInterface* Create();

protected:
    bool Initialize();
    void Shutdown();

    // CValidationInterface
    void TransactionAddedToMempool(const CTransactionRef& tx, int64_t nAcceptTime) override;
    void BlockConnected(const std::shared_ptr<const CBlock>& pblock, const CBlockIndex* pindexConnected, const std::vector<CTransactionRef>& vtxConflicted) override;
    void BlockDisconnected(const std::shared_ptr<const CBlock>& pblock, const CBlockIndex *pindexDisconnected) override;
    void UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload) override;
    void NotifyChainLock(const CBlockIndex *pindex, const llmq::CChainLockSig& clsig) override;
    void NotifyGovernanceVote(const CGovernanceVote& vote) override;
    void NotifyGovernanceObject(const CGovernanceObject& object) override;


private:
    CZMQNotificationInterface();

    void *pcontext;
    std::list<CZMQAbstractNotifier*> notifiers;
};

#endif // MACHINECOIN_ZMQ_ZMQNOTIFICATIONINTERFACE_H
