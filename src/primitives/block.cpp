// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Machinecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <utilstrencodings.h>
#include <crypto/common.h>
#include <crypto/scrypt.h> // Machinecoin Scrypt
#include <crypto/hashblock.h> // Machinecoin TimeTravel

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetPoWHash() const
{
		if(GetBlockTime() >= 1473444000) // Machinecoin PoW Hardfork, Friday, 09-Sep-16 18:00:00 UTC
		{
				return HashTimeTravel(BEGIN(nVersion), END(nNonce), GetBlockTime()); // Machinecoin TimeTravel
		}
		else
		{
				uint256 thash;
				scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(thash)); // Machinecoin Scrypt
				return thash;
		}
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
