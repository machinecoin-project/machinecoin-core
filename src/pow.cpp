// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Machinecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"
#include "validation.h"

// Machinecoin: Select retargeting
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    if (pindexLast->nHeight+1 < 76000)
    {
        return GetNextWorkRequired_V1(pindexLast, pblock, params); // Machinecoin: Standard retargeting (V1)
    }
    else if (pindexLast->nHeight+1 < 329529)
    {
        return GetNextWorkRequired_V2(pindexLast, pblock, params); // Machinecoin: Digishield retargeting (V2)
    }
    else if (pindexLast->nHeight+1 < 330000)
    {
        return UintToArith256(params.powLimit).GetCompact();       // Machinecoin: Retargeting to support the PoW change phase (V3)
    }
    else if (pindexLast->nHeight+1 < 468000)
    {
        return GetNextWorkRequired_V2(pindexLast, pblock, params); // Machinecoin: Digishield retargeting (V2)
    }
    else
    {
        return GetNextWorkRequired_V3(pindexLast, pblock, params); // Machinecoin: Octopus retargeting (V3)
    }
}

// Standard retargeting
unsigned int GetNextWorkRequired_V1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Only change once per interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 2.5 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Machinecoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval())
        blockstogoback = params.DifficultyAdjustmentInterval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

// DigiShield retargeting
unsigned int GetNextWorkRequired_V2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Only change once per interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentIntervalV2() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* nTargetSpacing minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacingV2*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentIntervalV2() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Machinecoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentIntervalV2()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentIntervalV2())
        blockstogoback = params.DifficultyAdjustmentIntervalV2();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

// Retargeting to support the PoW change phase (V3)
unsigned int GetNextWorkRequired_V3(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{

    if (pblock->GetBlockTime() - pindexLast->GetBlockTime() >= 300 && pblock->nBits != GetNextWorkRequired_V2(pindexLast, pblock, params)){
        arith_uint256 bnNew;
        bnNew.SetCompact(GetNextWorkRequired_V2(pindexLast, pblock, params));
        bool fShift = false;

        for (int i=0; i <= (pblock->GetBlockTime() -  pindexLast->GetBlockTime()) / 150; i++)
        {
            if (i == 0) continue;
            if (bnNew.bits() > 235)
                fShift = true; bnNew >>= 1;
            bnNew *= 190 + i * 3;
            bnNew /= 173;
            if (fShift)
                fShift = false; bnNew <<= 1;
        }

        const arith_uint256 nPowLimit  = UintToArith256(params.powLimit);

        if (bnNew > nPowLimit)
            bnNew = nPowLimit;

        return bnNew.GetCompact();
    }
    else return GetNextWorkRequired_V2(pindexLast, pblock, params);
}

// Machinecoin: Select retargeting
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (pindexLast->nHeight+1 < 76000) // Machinecoin: Standard retargeting (V1)
    {
        return CalculateNextWorkRequired_V1(pindexLast, nFirstBlockTime, params);
    }
    else if (pindexLast->nHeight+1 < 329529) // Machinecoin: Digishield retargeting (V2)
    {
        return CalculateNextWorkRequired_V2(pindexLast, nFirstBlockTime, params);
    }
    else if (pindexLast->nHeight+1 < 330000) // Machinecoin: Retargeting to support the PoW change phase (V3)
    {
        return UintToArith256(params.powLimit).GetCompact();
    }
    else if (pindexLast->nHeight+1 < 468000) // Machinecoin: Digishield retargeting (V2)
    {
        return CalculateNextWorkRequired_V2(pindexLast, nFirstBlockTime, params);
    }
    else
    {
        return CalculateNextWorkRequired_V2(pindexLast, nFirstBlockTime, params); // Machinecoin: Digishield retargeting (V2)
    }
}

// Standard retargeting
unsigned int CalculateNextWorkRequired_V1(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    //LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    arith_uint256 bnNew;
		  arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
		  bnOld = bnNew;
    // Machinecoin: intermediate uint256 can overflow by 1 bit
    bool fShift = bnNew.bits() > 235;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;
    if (fShift)
        bnNew <<= 1;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

// DigiShield retargeting
unsigned int CalculateNextWorkRequired_V2(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    //LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
    if (nActualTimespan < (params.nPowTargetTimespanV2 - (params.nPowTargetTimespanV2/4)))
        nActualTimespan = (params.nPowTargetTimespanV2 - (params.nPowTargetTimespanV2/4));
    if (nActualTimespan > (params.nPowTargetTimespanV2 + (params.nPowTargetTimespanV2/2)))
        nActualTimespan = (params.nPowTargetTimespanV2 + (params.nPowTargetTimespanV2/2));

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    // Machinecoin: intermediate uint256 can overflow by 1 bit
    bool fShift = bnNew.bits() > 235;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespanV2;
    if (fShift)
        bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

// Retargeting to support the PoW change phase (V3)
unsigned int CalculateNextWorkRequired_V3(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int64_t nBlockTimeO, int64_t nBlockTime, const Consensus::Params& params)
 {
   if (nBlockTime - nBlockTimeO >= 300)
   {
        arith_uint256 bnNew;
        bnNew.SetCompact(GetNextWorkRequired_V2(pindexLast, pblock, params));
        bool fShift = false;
        for (int i=0; i <= (nBlockTime - nBlockTimeO) / 150 ; i++)
        {
            if (i == 0) continue;
            if (bnNew.bits() > 235)
                fShift = true; bnNew >>= 1;
            bnNew *= 190 + i * 3;
            bnNew /= 173;
            if (fShift)
                fShift = false; bnNew <<= 1;
        }

        const arith_uint256 nPowLimit = UintToArith256(params.powLimit);

        if (bnNew > nPowLimit)
            bnNew = nPowLimit;

        return bnNew.GetCompact();
    }
    else return pblock->nBits;
 }

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
