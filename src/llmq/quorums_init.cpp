// Copyright (c) 2018 The Machinecoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <llmq/quorums_init.h>

#include <llmq/quorums_blockprocessor.h>
#include <llmq/quorums_commitment.h>
#include <llmq/quorums_dummydkg.h>

namespace llmq
{

void InitLLMQSystem(CEvoDB& evoDb)
{
    quorumBlockProcessor = new CQuorumBlockProcessor(evoDb);
    quorumDummyDKG = new CDummyDKG();
}

void DestroyLLMQSystem()
{
    delete quorumDummyDKG;
    quorumDummyDKG = nullptr;
    delete quorumBlockProcessor;
    quorumBlockProcessor = nullptr;
}

}
