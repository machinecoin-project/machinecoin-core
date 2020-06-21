// Copyright (c) 2015-2020 The Machinecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MACHINECOIN_DSNOTIFICATIONINTERFACE_H
#define MACHINECOIN_DSNOTIFICATIONINTERFACE_H

#include <validationinterface.h>

class CDSNotificationInterface : public CValidationInterface
{
public:
    CDSNotificationInterface(CConnman& connmanIn): connman(connmanIn) {}
    virtual ~CDSNotificationInterface() = default;

    // a small helper to initialize current block height in sub-modules on startup
    void InitializeCurrentBlockTip();

protected:
    // CValidationInterface
    void AcceptedBlockHeader(const CBlockIndex *pindexNew) override;
    void NotifyHeaderTip(const CBlockIndex *pindexNew, bool fInitialDownload) override;
    void UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload) override;

private:
    CConnman& connman;
};

#endif // MACHINECOIN_DSNOTIFICATIONINTERFACE_H
