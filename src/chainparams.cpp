// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Machinecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * 04ffff001d01044c4d4465722054616765737370696567656c2030362f4a616e2f323031342048656e7279204d61736b652c2066616d6f757320666967687465722c206973206e6f77203530207965617273206f6c64
 * algorithm: scrypt
 * merkle hash: 36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c
 * pszTimestamp: Der Tagesspiegel 06/Jan/2014 Henry Maske, famous fighter, is now 50 years old
 * pubkey: 040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9
 * time: 1389040865
 * bits: 0x1e0ffff0
 * nonce: 3716037
 * genesis hash: 6a1f879bcea5471cbfdee1fd0cb2ddcc4fed569a500e352d41de967703e83172
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Der Tagesspiegel 06/Jan/2014 Henry Maske, famous fighter, is now 50 years old";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 350000; // Machinecoin: number of blocks until a halving which is ~ 20 months
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 227931; // Machinecoin BIP34 height
        consensus.BIP34Hash = uint256S("0x5b77f70e30731bbf8c4b6c44cf62170cc559b6015aa9b7988d8d44a30c4a4614"); // Machinecoin BIP34Hash
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Machinecoin: powLimit
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // Machinecoin: 3 1/2 days
        consensus.nPowTargetSpacing = 2.5 * 60; // Machinecoin: 2 1/2 minutes
        consensus.nPowTargetTimespanV2 = 2.5 * 60; // Machinecoin: 2 1/2 minutes
        consensus.nPowTargetSpacingV2 = 2.5 * 60; // Machinecoin: 2 1/2 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // Machinecoin: 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // Machinecoin: nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141 and BIP143)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // Never / undefined

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfb; // Machinecoin: 0xfb
        pchMessageStart[1] = 0xc0; // Machinecoin: 0xc0
        pchMessageStart[2] = 0xb6; // Machinecoin: 0xb6
        pchMessageStart[3] = 0xdb; // Machinecoin: 0xdb
        nDefaultPort = 40333;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1389040865, 3716037, 0x1e0ffff0, 1, 50 * COIN); // Machinecoin: 1389040865, 3716037, 0x1e0ffff0
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x6a1f879bcea5471cbfdee1fd0cb2ddcc4fed569a500e352d41de967703e83172")); // Machinecoin: 0x6a1f879bcea5471cbfdee1fd0cb2ddcc4fed569a500e352d41de967703e83172
        assert(genesis.hashMerkleRoot == uint256S("0x36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c")); // Machinecoin: 0x36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c

        vSeeds.push_back(CDNSSeedData("dnsseed1.machinecoin.org", "dnsseed1.machinecoin.org", true)); // Machinecoin: dnsseed1
        vSeeds.push_back(CDNSSeedData("dnsseed2.machinecoin.org", "dnsseed2.machinecoin.org")); // Machinecoin: dnsseed2

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 50); // Machinecoin: M
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,  5); // Machinecoin: 3
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178); // Machinecoin: 2
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >(); // Machinecoin: xpub
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >(); // Machinecoin: xprv

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 23021, uint256S("0x0268f4e816aac0874c911c83e263353289854c94a21cf97675652419893e7d8f"))  // Machinecoin: checkpoint at 23021
            ( 53600, uint256S("0x327ec569aa2439c16542ed9402884f5ce691d08f49168d672f19f817ace7a06b"))  // Machinecoin: checkpoint at 53600
            (112715, uint256S("0x4fba0d1f891a35a7e0b7370d13b777e75fd2826423ef777ccca2f63d6acc70c5"))  // Machinecoin: checkpoint at 112715
            (130938, uint256S("0x3fc5ccce46b45775ea3cb9f0d10169227bbd019518ebb90e5d6b8a770bf85d1d"))  // Machinecoin: checkpoint at 130938
            (148401, uint256S("0x0c6b00515da19b9a95571e8cc61447442f6ade0f2e10e3b9ee6df133a76a809f"))  // Machinecoin: checkpoint at 148401
            (305275, uint256S("0x4222db35707d9af172ea5c23582338874d1f2956b1850f857c50a3e6f2c6f313"))  // Machinecoin: checkpoint at 305275
            (329528, uint256S("0x6fc60fe0c0133dfbb7fd7fae48e689c344e513a109cb4724913f768070f02a11")), // Machinecoin: checkpoint at 329528
            1473351035, // Machinecoin: UNIX timestamp of last checkpoint block
            364811,     // Machinecoin: total number of transactions between genesis and last checkpoint
                        // Machinecoin: (the tx=... number in the SetBestChain debug.log lines)
            500.0       // Machinecoin: estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 350000; // Machinecoin: number of blocks for testnet until a halving which is ~ 20 months
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 21111; // Machinecoin BIP34 height
        consensus.BIP34Hash = uint256S("0x78a10bf38a56678f906ee80c2bbeb88649d210de6b8a6751a0c2d4bd0603c642"); // Machinecoin BIP34Hash
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Machinecoin: powLimit
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // Machinecoin: 3 1/2 days
        consensus.nPowTargetSpacing = 2.5 * 60; // Machinecoin: 2 1/2 minutes
        consensus.nPowTargetTimespanV2 = 2.5 * 60; // Machinecoin: 2 1/2 minutes
        consensus.nPowTargetSpacingV2 = 2.5 * 60; // Machinecoin: 2 1/2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141 and BIP143)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800; // May 1st 2017

        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb1;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 50333;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1389040865, 3716037, 0x1e0ffff0, 1, 50 * COIN); // Machinecoin: 1389040865, 3716037, 0x1e0ffff0
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x6a1f879bcea5471cbfdee1fd0cb2ddcc4fed569a500e352d41de967703e83172")); // Machinecoin: 0x6a1f879bcea5471cbfdee1fd0cb2ddcc4fed569a500e352d41de967703e83172
        assert(genesis.hashMerkleRoot == uint256S("0x36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c")); // Machinecoin: 0x36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("testnetdnsseed1.machinecoin.org", "testnetdnsseed1.machinecoin.org", true)); // Machinecoin: testnetdnsseed1
        vSeeds.push_back(CDNSSeedData("testnetdnsseed2.machinecoin.org", "testnetdnsseed2.machinecoin.org")); // Machinecoin: testnetdnsseed2

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 299, uint256S("0x000000002a936ca763904c3c35fce2f3556c559c0214345d31b1bcebf76acb70")),
            1369685559,
            0,
            300.0
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 350000; // Machinecoin: number of blocks for testnet until a halving which is ~ 20 months
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Machinecoin: powLimit
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // Machinecoin: 3 1/2 days
        consensus.nPowTargetSpacing = 2.5 * 60; // Machinecoin: 2 1/2 minutes
        consensus.nPowTargetTimespanV2 = 2.5 * 60; // Machinecoin: 2 1/2 minutes
        consensus.nPowTargetSpacingV2 = 2.5 * 60; // Machinecoin: 2 1/2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 60333;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1389040865, 3716037, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x6a1f879bcea5471cbfdee1fd0cb2ddcc4fed569a500e352d41de967703e83172"));
        assert(genesis.hashMerkleRoot == uint256S("0x36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x8d9888baa78c14790d6beb64a9cc61ba7541b509953ba1e51b3cd6cbdc245904")),
            0,
            0,
            0
        };
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
 
