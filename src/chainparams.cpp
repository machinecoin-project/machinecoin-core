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
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
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
        consensus.nSubsidyHalvingInterval = 350000; // Machinecoin number of blocks until halving on 1st Nov. 2016 09:17:52 PM
        consensus.BIP34Height = 227931; // Machinecoin BIP34 height
        consensus.BIP34Hash = uint256S("5b77f70e30731bbf8c4b6c44cf62170cc559b6015aa9b7988d8d44a30c4a4614"); // Machinecoin BIP34 Hash
        consensus.BIP65Height = 416969; // c49bfa33dd4d76a6a05f93c1eb4310993ce00c7a8a9ee23c76164ddc2eecbdef
        consensus.BIP66Height = 415969; // 83703951207fcf9e22516f97603f8621299de483e85d85e8a86f7b0fd32717dc
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Machinecoin PoW Limit
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // Machinecoin 3.5 days
        consensus.nPowTargetSpacing = 2.5 * 60; // Machinecoin 2.5 minutes
				consensus.nPowTargetTimespanV2 = 2.5 * 60; // Machinecoin 2.5 minutes
				consensus.nPowTargetSpacingV2 = 2.5 * 60; // Machinecoin 2.5 minutes
				// consensus.nPowTargetTimespanV3
				// consensus.nPowTargetSpacingV3
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // Machinecoin 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // Machinecoin nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000039fa7a2354aa8");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x80316ccd85323c6ed19be11a420c53a5b42b6bc37df14eb56763e5bec37990c5"); //416569

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xdb;
        nDefaultPort = 40333;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1389040865, 3716037, 0x1e0ffff0, 1, 50 * COIN); // Machinecoin 1389040865, 3716037
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x6a1f879bcea5471cbfdee1fd0cb2ddcc4fed569a500e352d41de967703e83172"));
        assert(genesis.hashMerkleRoot == uint256S("0x36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("dnsseed1.machinecoin.org", "dnsseed1.machinecoin.org", true));
        vSeeds.push_back(CDNSSeedData("dnsseed2.machinecoin.org", "dnsseed2.machinecoin.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50); // Machinecoin M
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5); // Machinecoin 3
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178); // Machinecoin 2
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 23021, uint256S("0x0268f4e816aac0874c911c83e263353289854c94a21cf97675652419893e7d8f"))  // Machinecoin: checkpoint at 23021
            ( 53600, uint256S("0x327ec569aa2439c16542ed9402884f5ce691d08f49168d672f19f817ace7a06b"))  // Machinecoin: checkpoint at 53600
            (112715, uint256S("0x4fba0d1f891a35a7e0b7370d13b777e75fd2826423ef777ccca2f63d6acc70c5"))  // Machinecoin: checkpoint at 112715
            (130938, uint256S("0x3fc5ccce46b45775ea3cb9f0d10169227bbd019518ebb90e5d6b8a770bf85d1d"))  // Machinecoin: checkpoint at 130938
            (148401, uint256S("0x0c6b00515da19b9a95571e8cc61447442f6ade0f2e10e3b9ee6df133a76a809f"))  // Machinecoin: checkpoint at 148401
            (305275, uint256S("0x4222db35707d9af172ea5c23582338874d1f2956b1850f857c50a3e6f2c6f313"))  // Machinecoin: checkpoint at 305275
            (329528, uint256S("0x6fc60fe0c0133dfbb7fd7fae48e689c344e513a109cb4724913f768070f02a11"))  // Machinecoin: checkpoint at 329528
						(410476, uint256S("0x8da1c7f79018fac8acac69a57b2f8b5d2743af67976a4525fdedc8c85a3a1418"))  // Machinecoin: checkpoint at 410476
        };

        chainTxData = ChainTxData{
            // Data as of block 0x8da1c7f79018fac8acac69a57b2f8b5d2743af67976a4525fdedc8c85a3a1418 (height 410476).
            1490495580, // * UNIX timestamp of last known number of transactions
            449775,  // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500.0         // * estimated number of transactions per second after that timestamp
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
        consensus.nSubsidyHalvingInterval = 350000; // Machinecoin number of blocks until halving on 1st Nov. 2016 09:17:52 PM
        consensus.BIP34Height = 0; // Machinecoin BIP34 height
        consensus.BIP34Hash = uint256S("72059c481cc49a2941cc36bd0f070abfe1ccc6e329534602dbdef555547e895f"); // Machinecoin BIP34 Hash
        consensus.BIP65Height = 0; // c49bfa33dd4d76a6a05f93c1eb4310993ce00c7a8a9ee23c76164ddc2eecbdef
        consensus.BIP66Height = 0; // 83703951207fcf9e22516f97603f8621299de483e85d85e8a86f7b0fd32717dc
        consensus.powLimit = uint256S("72059c481cc49a2941cc36bd0f070abfe1ccc6e329534602dbdef555547e895f"); // Machinecoin PoW Limit
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // Machinecoin 3.5 days
        consensus.nPowTargetSpacing = 2.5 * 60; // Machinecoin 2.5 minutes
				consensus.nPowTargetTimespanV2 = 2.5 * 60; // Machinecoin 2.5 minutes
				consensus.nPowTargetSpacingV2 = 2.5 * 60; // Machinecoin 2.5 minutes
				// consensus.nPowTargetTimespanV3
				// consensus.nPowTargetSpacingV3
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1; // Machinecoin Testnet 1% of 2
        consensus.nMinerConfirmationWindow = 2; // Machinecoin nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1494604259; // May 12, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1494604959; // May 12, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1494604259; // May 12, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1494604959; // May 12, 2017

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x72059c481cc49a2941cc36bd0f070abfe1ccc6e329534602dbdef555547e895f"); //0

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xdb;
        nDefaultPort = 50333;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1473357600, 5653466, 0x1e0ffff0, 1, 50 * COIN); // Machinecoin Testnet 1473357600, 5653466
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x72059c481cc49a2941cc36bd0f070abfe1ccc6e329534602dbdef555547e895f"));
        assert(genesis.hashMerkleRoot == uint256S("0x36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("dnsseed1.machinecoin.org", "dnsseed1.machinecoin.org", true));
        vSeeds.push_back(CDNSSeedData("dnsseed2.machinecoin.org", "dnsseed2.machinecoin.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,53); // Machinecoin N
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5); // Machinecoin 3
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178); // Machinecoin 2
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
          boost::assign::map_list_of
          ( 0, uint256S("0x72059c481cc49a2941cc36bd0f070abfe1ccc6e329534602dbdef555547e895f"))  // Machinecoin Testnet: checkpoint at 0
          ( 209, uint256S("0x3c63f32416111dca75775eb0361b110be82f4603a83bb3f2e8d88326a5ccf2f3"))  // Machinecoin Testnet: checkpoint at 209
        };

        chainTxData = ChainTxData{
            // Data as of block 3c63f32416111dca75775eb0361b110be82f4603a83bb3f2e8d88326a5ccf2f3 (height 209).
            1494605858, // * UNIX timestamp of last known number of transactions
            211,  // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500.0         // * estimated number of transactions per second after that timestamp
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
        consensus.nSubsidyHalvingInterval = 150; // Machinecoin number of blocks until halving on 1st Nov. 2016 09:17:52 PM
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256(); // Machinecoin BIP34 Hash
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Machinecoin PoW Limit
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // Machinecoin 3.5 days
        consensus.nPowTargetSpacing = 2.5 * 60; // Machinecoin 2.5 minutes
        consensus.nPowTargetTimespanV2 = 2.5 * 60; // Machinecoin 2.5 minutes
        consensus.nPowTargetSpacingV2 = 2.5 * 60; // Machinecoin 2.5 minutes
        // consensus.nPowTargetTimespanV3
        // consensus.nPowTargetSpacingV3
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108;
        consensus.nMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); //0

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xc6;
        pchMessageStart[3] = 0xab;
        nDefaultPort = 60333;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 0, 0x207fffff, 1, 50 * COIN); // Machinecoin Regtest 1296688602, 0
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x62214f698865bf3308a4cddec2a18c1e9a98f272189f696e0f99d5d1b137912b"));
        assert(genesis.hashMerkleRoot == uint256S("0x36a9e41063f3e71466299d0ed9e8193c1c802a88b286016fa4a4d0c3bc384a5c"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
          boost::assign::map_list_of
          ( 0, uint256S("0x62214f698865bf3308a4cddec2a18c1e9a98f272189f696e0f99d5d1b137912b"))  // Machinecoin Regtest: checkpoint at 0
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,55); // Machinecoin N
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5); // Machinecoin 3
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178); // Machinecoin 2
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
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

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
