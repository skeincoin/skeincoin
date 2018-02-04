// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
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
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Guardian - 1 Nov 2013 - RBS places troublesome assets worth £38bn in internal 'bad bank'";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
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
        consensus.nSubsidyHalvingInterval = 262800;
        consensus.nMajorityEnforceBlockUpgrade = 7500;
        consensus.nMajorityRejectBlockOutdated = 9500;
        consensus.nMajorityWindow = 10000;
        consensus.BIP34Height = 227931;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4 * 120; // 8 minutes
        consensus.nPowTargetSpacing = 120; // 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf7;
        pchMessageStart[1] = 0x26;
        pchMessageStart[2] = 0xa1;
        pchMessageStart[3] = 0xbf;
        vAlertPubKey = ParseHex("04e41db2a8b8dc3981f819d46060875ce483bf303613b108e673d7bb636f7786bd0458e2ced6e8b337be32d024562f3e69776412b55a7210396ad7a9944812b445");
        nDefaultPort = 11230;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1383313611, 2094010698, 0x1e0fffff, 1, COIN / 10000);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(genesis.hashMerkleRoot == uint256S("0xa4b385e3bc4907593d15be30d69cb28439684893f4dc2e637503cf3156b149a3"));
        assert(consensus.hashGenesisBlock == uint256S("0x0000046cebed69de151ada93a60cb8a5f9490a196399abe714bb83ad5b20f985"));

        vSeeds.push_back(CDNSSeedData("seed-a.skeincoin.net", "seed-a.skeincoin.net"));
        vSeeds.push_back(CDNSSeedData("seed-b.skeincoin.net", "seed-b.skeincoin.net"));
        vSeeds.push_back(CDNSSeedData("seed-c.skeincoin.net", "seed-c.skeincoin.net"));
        vSeeds.push_back(CDNSSeedData("seed-d.skeincoin.net", "seed-d.skeincoin.net"));
        vSeeds.push_back(CDNSSeedData("seed-e.skeincoin.net", "seed-e.skeincoin.net"));
        vSeeds.push_back(CDNSSeedData("seed-f.skeincoin.net", "seed-f.skeincoin.net"));
        vSeeds.push_back(CDNSSeedData("seed-g.skeincoin.net", "seed-g.skeincoin.net"));
        vSeeds.push_back(CDNSSeedData("seed-h.skeincoin.net", "seed-h.skeincoin.net"));
        vSeeds.push_back(CDNSSeedData("skein1.ignorelist.com", "skein1.ignorelist.com"));
        vSeeds.push_back(CDNSSeedData("skein2.ignorelist.com", "skein2.ignorelist.com"));
        vSeeds.push_back(CDNSSeedData("skein3.ignorelist.com", "skein3.ignorelist.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,12);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,226);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (     0,  uint256S("0x0000046cebed69de151ada93a60cb8a5f9490a196399abe714bb83ad5b20f985"))
            (  8001,  uint256S("0x0000000012ce3340428fdd525c7ae8ab62cbdc104409ccd9cc03042c5c2fa100"))
            ( 52721,  uint256S("0x00000000004781ede745a02508e2680343cb12d53ca466b6066d8d3b323a1b46"))
            ( 84202,  uint256S("0x00000000032f902c2da525c75217e9b4219eb3b1aaf56f2f5ca6fb65f9de4ab4"))
            ( 95197,  uint256S("0x000000000237a7236374b307d88f07be7fe61c974499c31bf547e68fe5d2f6c7"))
            (112814,  uint256S("0x0000000004fb21b45b3b30eb4f1a8db0899e7d8893c35fce0af5fd1ef7b1ec79"))
            (117682,  uint256S("0x00000000015918d209c230db8141fcf177c9431fcf153ddf563ff3e83fb6d847"))
            (139538,  uint256S("0x00000000017ef670edcab8e120efdd5a4d44972095aa538a1a505bb23424d72f"))
            (144721,  uint256S("0x0000000000f96c1c5e88bd5ee1819a42469ab0cbb463de66b36cdba75b90a538"))
            (220450,  uint256S("0x0000000003fe832596716f971f3acf2975b198ec2536c6ba23e3f3746ce6c666"))
            (304170,  uint256S("0x000000002122d751e98911571800e0c8f2143fc7b93cccd6d661e1398d3bbaa0"))
            (1269933, uint256S("0x000000000066173e74b2b76ff23b3a5de0828f21437b87d5fc6808e7b3c83f41")),
            1516072669, // * UNIX timestamp of last checkpoint block
            1335748,    // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            720         // * estimated number of transactions per day after checkpoint
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
        consensus.nSubsidyHalvingInterval = 262800;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 21111;
        consensus.BIP34Hash = uint256S("0x0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8");
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4 * 120; // 8 minutes
        consensus.nPowTargetSpacing = 120; // 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        pchMessageStart[0] = 0x07;
        pchMessageStart[1] = 0xa0;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0x03;
        vAlertPubKey = ParseHex("0490ce131d0e51eacddb074906fc0be1990214ec9880c2da9bfc15d649236f211e3950670fee0c45f8ea00d425d84da61425778617ba73d64e1880abc4607de965");
        nDefaultPort = 27711;
        nMaxTipAge = 0x7fffffff;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1382385267, 416003859, 0x1e0fffff, 1, COIN / 10000);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(genesis.hashMerkleRoot == uint256S("0xa4b385e3bc4907593d15be30d69cb28439684893f4dc2e637503cf3156b149a3"));
        assert(consensus.hashGenesisBlock == uint256S("0x00000015f9fb4c1c9cc55ad08b6ec47fcce2b00bc482a2c48914ab6506daf439"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,56);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,88);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,237);
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
            ( 0, uint256S("0x00000015f9fb4c1c9cc55ad08b6ec47fcce2b00bc482a2c48914ab6506daf439")),
            1382385267,
            0,
            720
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
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4 * 120; // 8 minutes
        consensus.nPowTargetSpacing = 120; // 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0x0f;
        pchMessageStart[2] = 0xa5;
        pchMessageStart[3] = 0x5a;
        nMaxTipAge = 24 * 60 * 60;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 4, 0x207fffff, 1, COIN / 10000);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(genesis.hashMerkleRoot == uint256S("0xa4b385e3bc4907593d15be30d69cb28439684893f4dc2e637503cf3156b149a3"));
        assert(consensus.hashGenesisBlock == uint256S("0x6620ff0f4b001bb6c6999d85a61f406a3b213a4ed0364d1b10a46596fcb09785"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x6620ff0f4b001bb6c6999d85a61f406a3b213a4ed0364d1b10a46596fcb09785")),
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
