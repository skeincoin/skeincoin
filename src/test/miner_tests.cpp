// Copyright (c) 2011-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "coins.h"
#include "consensus/consensus.h"
#include "consensus/merkle.h"
#include "consensus/validation.h"
#include "main.h"
#include "miner.h"
#include "pubkey.h"
#include "script/standard.h"
#include "txmempool.h"
#include "uint256.h"
#include "util.h"
#include "utilstrencodings.h"

#include "test/test_bitcoin.h"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(miner_tests, TestingSetup)

static
struct {
    unsigned char extranonce;
    unsigned int nonce;
} blockinfo[] = {
    {0, 0x000b4f2d}, {0, 0x000fd62c}, {0, 0x000a7551}, {0, 0x0002cdf7},
    {0, 0x00054bc6}, {0, 0x002eeb18}, {0, 0x00067278}, {0, 0x00001fc9},
    {0, 0x00017b3a}, {0, 0x000678ea}, {0, 0x001c3985}, {0, 0x00197491},
    {0, 0x0019f4c0}, {0, 0x0009d156}, {0, 0x00241a54}, {0, 0x00084f32},
    {0, 0x00048c42}, {0, 0x0004323d}, {0, 0x00091bd5}, {0, 0x00160d46},
    {0, 0x00123804}, {0, 0x001e8925}, {0, 0x001e1c1e}, {0, 0x0002f322},
    {0, 0x0008ea53}, {0, 0x0004f2d5}, {0, 0x0028ccef}, {0, 0x00026ab9},
    {0, 0x000f7010}, {0, 0x000ebf01}, {0, 0x001620ec}, {0, 0x000060f5},
    {0, 0x001260e7}, {0, 0x0049ca75}, {0, 0x00179dac}, {0, 0x001712c4},
    {0, 0x000cb531}, {0, 0x000bf801}, {0, 0x000b509d}, {0, 0x0018b854},
    {0, 0x00344585}, {0, 0x0007376b}, {0, 0x00223d67}, {0, 0x00099ec0},
    {0, 0x0007f00d}, {0, 0x0027af08}, {0, 0x00296eee}, {0, 0x00012270},
    {0, 0x0007491c}, {0, 0x0001dd6d}, {0, 0x0028eaf1}, {0, 0x00004ced},
    {0, 0x00011ae8}, {0, 0x000e5bcb}, {0, 0x000035f7}, {0, 0x00376ac9},
    {0, 0x0040efec}, {0, 0x0004acea}, {0, 0x0007b5c2}, {0, 0x000f9d78},
    {0, 0x001730d7}, {0, 0x001483d3}, {0, 0x002a04da}, {0, 0x0001a651},
    {0, 0x0010374f}, {0, 0x000256c7}, {0, 0x00007168}, {0, 0x003be3e2},
    {0, 0x00042823}, {0, 0x002357ff}, {0, 0x00265d9c}, {0, 0x0014cb83},
    {0, 0x0002a2d9}, {0, 0x000b5c3c}, {0, 0x00038ca2}, {0, 0x001a8cd6},
    {0, 0x000b6ecd}, {0, 0x001adfc8}, {0, 0x001147b9}, {0, 0x0005ca53},
    {0, 0x0002e59f}, {0, 0x0001ef45}, {0, 0x001dac6b}, {0, 0x000e943c},
    {0, 0x0012daa0}, {0, 0x00017306}, {0, 0x0012d1a9}, {0, 0x00090ab7},
    {0, 0x000a68c5}, {0, 0x00711c77}, {0, 0x000aa872}, {0, 0x000686c9},
    {0, 0x00358512}, {0, 0x0011b6e1}, {0, 0x00065a86}, {0, 0x000b5ce0},
    {0, 0x000a3c99}, {0, 0x0000b456}, {0, 0x000373bf}, {0, 0x0005256c},
    {0, 0x0006662b}, {0, 0x00504f36}, {0, 0x001afed5}, {0, 0x000328ae},
    {0, 0x000e7f5c}, {0, 0x0004bda8}, {0, 0x0013672d}, {0, 0x0004ff86},
    {0, 0x001c4843}, {0, 0x00035105}, {0, 0x0005ec71}, {0, 0x000d03a7},
    {0, 0x005290db}, {0, 0x0004658e}, {0, 0x0024c967}, {0, 0x0006a03c},
    {0, 0x000a5f8d}, {0, 0x0001a9c4}, {0, 0x0004019a}, {0, 0x00061f7c},
    {0, 0x0046923f}, {0, 0x0016cdd7}, {0, 0x009af25e}, {0, 0x0005d053},
    {0, 0x001c354c}, {0, 0x00036673}, {0, 0x00025e1f}, {0, 0x001c8b60},
    {0, 0x004d058c}, {0, 0x00243058}, {0, 0x00074362}, {0, 0x0008b019},
    {0, 0x001ee541}, {0, 0x002eb5ce}, {0, 0x000e9a5a}, {0, 0x0000b75e},
    {0, 0x00200411}, {0, 0x000082ed}, {0, 0x000ffb3f}, {0, 0x0000d6af},
    {0, 0x00112e30}, {0, 0x000fc81d}, {0, 0x000f148b}, {0, 0x00150eb4},
    {0, 0x0019e30c}, {0, 0x0006ed9d}, {0, 0x0023f461}, {0, 0x001b3915},
    {0, 0x00528f64}, {0, 0x00435bd1}, {0, 0x001a55bb}, {0, 0x0007ec4b},
    {0, 0x00092f4e}, {0, 0x00054be2}, {0, 0x0009986b}, {0, 0x002241e7},
    {0, 0x000691cc}, {0, 0x000c79f7}, {0, 0x001fe1ed}, {0, 0x0000de74},
    {0, 0x0003fd28}, {0, 0x003b55f1}, {0, 0x002d8d91}, {0, 0x0016c550},
    {0, 0x003e543f}, {0, 0x0012d212}, {0, 0x0003f1b1}, {0, 0x0025f0b4},
    {0, 0x00146921}, {0, 0x0015b1f6}, {0, 0x003c0b5b}, {0, 0x003471ca},
    {0, 0x001dfb14}, {0, 0x001bf3ef}, {0, 0x00209c26}, {0, 0x001039da},
    {0, 0x000a79dd}, {0, 0x002a18e9}, {0, 0x0011c1f2}, {0, 0x000a3db3},
    {0, 0x0000bcbd}, {0, 0x0015ce9e}, {0, 0x0024f8f2}, {0, 0x000f53ca},
    {0, 0x0000f778}, {0, 0x00150322}, {0, 0x00006b07}, {0, 0x000aee60},
    {0, 0x0001cfed}, {0, 0x003735a8}, {0, 0x001e7275}, {0, 0x000b4283},
    {0, 0x00022386}, {0, 0x0005ae3f}, {0, 0x0007adde}, {0, 0x0001730a},
    {0, 0x002032dd}, {0, 0x0004017b}, {0, 0x00536632}, {0, 0x00073482},
    {0, 0x0014cae8}, {0, 0x0020ada5}, {0, 0x000ca48d}, {0, 0x000dac16},
    {0, 0x000692d6}, {0, 0x0022582f}, {0, 0x0037243a}, {0, 0x0015ddf8},
    {0, 0x0025b9ad}, {0, 0x001c865b}, {0, 0x00043221}, {0, 0x003577ea},
    {0, 0x0005d148}, {0, 0x000e977b}, {0, 0x0030c51e}, {0, 0x00351fd9},
    {0, 0x008c460a}, {0, 0x0060c3fb}, {0, 0x0049d665}, {0, 0x00016ceb},
    {0, 0x0015b368}, {0, 0x00091225}, {0, 0x0010af8a}, {0, 0x002521e1},
    {0, 0x0012338c}, {0, 0x001b99b5}, {0, 0x004a25a8}, {0, 0x0002410e},
    {0, 0x000f3a4c}, {0, 0x00775ace}, {0, 0x0000cca7}, {0, 0x001bcdd5},
    {0, 0x0056625a}, {0, 0x0032b12c}, {0, 0x000fba4f}, {0, 0x0003bc5b},
    {0, 0x003e8bdf}, {0, 0x0024318c}, {0, 0x000e9cfb}, {0, 0x005ea34d},
    {0, 0x0007a67e}, {0, 0x0007677b}, {0, 0x004989a4}, {0, 0x0017d272},
    {0, 0x002425d9}, {0, 0x0001216d}, {0, 0x0001564f}, {0, 0x000db5f1},
    {0, 0x0041c7c0}, {0, 0x0009fdd2}, {0, 0x00035b1b}, {0, 0x0013005d},
    {0, 0x00318818}, {0, 0x0010dd68}, {0, 0x0008aac2}, {0, 0x007f88c4},
    {0, 0x0017dfb5}, {0, 0x000e1d97}, {0, 0x001a2b77}, {0, 0x00258198},
    {0, 0x000a92f3},
};

// NOTE: These tests rely on CreateNewBlock doing its own self-validation!
BOOST_AUTO_TEST_CASE(CreateNewBlock_validity)
{
    const CChainParams& chainparams = Params(CBaseChainParams::MAIN);
    CScript scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    CBlockTemplate *pblocktemplate;
    CMutableTransaction tx,tx2;
    CScript script;
    uint256 hash;
    TestMemPoolEntryHelper entry;
    entry.nFee = 11;
    entry.dPriority = 111.0;
    entry.nHeight = 11;

    LOCK(cs_main);
    fCheckpointsEnabled = false;

    // We can't make transactions until we have inputs
    // Therefore, load 100 blocks :)
    std::vector<CTransaction*>txFirst;
    for (unsigned int i = 0; i < sizeof(blockinfo)/sizeof(*blockinfo); ++i)
    {
        // Simple block creation, nothing special yet:
        BOOST_CHECK(pblocktemplate = CreateNewBlock(chainparams, scriptPubKey));

        CBlock *pblock = &pblocktemplate->block; // pointer for convenience
        pblock->nVersion = 1;
        pblock->nTime = chainActive.Tip()->GetMedianTimePast()+1;
        CMutableTransaction txCoinbase(pblock->vtx[0]);
        txCoinbase.nVersion = 1;
        txCoinbase.vin[0].scriptSig = CScript();
        txCoinbase.vin[0].scriptSig.push_back(blockinfo[i].extranonce);
        txCoinbase.vin[0].scriptSig.push_back(chainActive.Height());
        txCoinbase.vout[0].scriptPubKey = CScript();
        pblock->vtx[0] = CTransaction(txCoinbase);
        if (i >= 100 && txFirst.size() < 2)
            txFirst.push_back(new CTransaction(pblock->vtx[0]));
        pblock->hashMerkleRoot = BlockMerkleRoot(*pblock);
        pblock->nNonce = blockinfo[i].nonce;
        CValidationState state;
        BOOST_CHECK(ProcessNewBlock(state, chainparams, NULL, pblock, true, NULL));
        BOOST_CHECK(state.IsValid());
        pblock->hashPrevBlock = pblock->GetHash();

        delete pblocktemplate;
    }

    // Just to make sure we can still make simple blocks
    BOOST_CHECK(pblocktemplate = CreateNewBlock(chainparams, scriptPubKey));
    delete pblocktemplate;

    // block sigops > limit: 1000 CHECKMULTISIG + 1
    tx.vin.resize(1);
    // NOTE: OP_NOP is used to force 20 SigOps for the CHECKMULTISIG
    tx.vin[0].scriptSig = CScript() << OP_0 << OP_0 << OP_0 << OP_NOP << OP_CHECKMULTISIG << OP_1;
    tx.vin[0].prevout.hash = txFirst[0]->GetHash();
    tx.vin[0].prevout.n = 0;
    tx.vout.resize(1);
    tx.vout[0].nValue = 3200000000LL;
    for (unsigned int i = 0; i < 1001; ++i)
    {
        tx.vout[0].nValue -= 1000000;
        hash = tx.GetHash();
        bool spendsCoinbase = (i == 0) ? true : false; // only first tx spends coinbase
        // If we don't set the # of sig ops in the CTxMemPoolEntry, template creation fails
        mempool.addUnchecked(hash, entry.Fee(1000000).Time(GetTime()).SpendsCoinbase(spendsCoinbase).FromTx(tx));
        tx.vin[0].prevout.hash = hash;
    }
    BOOST_CHECK_THROW(CreateNewBlock(chainparams, scriptPubKey), std::runtime_error);
    mempool.clear();

    tx.vin[0].prevout.hash = txFirst[0]->GetHash();
    tx.vout[0].nValue = 3200000000LL;
    for (unsigned int i = 0; i < 1001; ++i)
    {
        tx.vout[0].nValue -= 1000000;
        hash = tx.GetHash();
        bool spendsCoinbase = (i == 0) ? true : false; // only first tx spends coinbase
        // If we do set the # of sig ops in the CTxMemPoolEntry, template creation passes
        mempool.addUnchecked(hash, entry.Fee(1000000).Time(GetTime()).SpendsCoinbase(spendsCoinbase).SigOps(20).FromTx(tx));
        tx.vin[0].prevout.hash = hash;
    }
    BOOST_CHECK(pblocktemplate = CreateNewBlock(chainparams, scriptPubKey));
    delete pblocktemplate;
    mempool.clear();

    // block size > limit
    tx.vin[0].scriptSig = CScript();
    // 18 * (520char + DROP) + OP_1 = 9433 bytes
    std::vector<unsigned char> vchData(520);
    for (unsigned int i = 0; i < 18; ++i)
        tx.vin[0].scriptSig << vchData << OP_DROP;
    tx.vin[0].scriptSig << OP_1;
    tx.vin[0].prevout.hash = txFirst[0]->GetHash();
    tx.vout[0].nValue = 3200000000LL;
    for (unsigned int i = 0; i < 128; ++i)
    {
        tx.vout[0].nValue -= 10000000;
        hash = tx.GetHash();
        bool spendsCoinbase = (i == 0) ? true : false; // only first tx spends coinbase
        mempool.addUnchecked(hash, entry.Fee(1000000).Time(GetTime()).SpendsCoinbase(spendsCoinbase).FromTx(tx));
        tx.vin[0].prevout.hash = hash;
    }
    BOOST_CHECK(pblocktemplate = CreateNewBlock(chainparams, scriptPubKey));
    delete pblocktemplate;
    mempool.clear();

    // orphan in mempool, template creation fails
    hash = tx.GetHash();
    mempool.addUnchecked(hash, entry.Fee(1000000).Time(GetTime()).FromTx(tx));
    BOOST_CHECK_THROW(CreateNewBlock(chainparams, scriptPubKey), std::runtime_error);
    mempool.clear();

    // child with higher priority than parent
    tx.vin[0].scriptSig = CScript() << OP_1;
    tx.vin[0].prevout.hash = txFirst[1]->GetHash();
    tx.vout[0].nValue = 3100000000LL;
    hash = tx.GetHash();
    mempool.addUnchecked(hash, entry.Fee(100000000LL).Time(GetTime()).SpendsCoinbase(true).FromTx(tx));
    tx.vin[0].prevout.hash = hash;
    tx.vin.resize(2);
    tx.vin[1].scriptSig = CScript() << OP_1;
    tx.vin[1].prevout.hash = txFirst[0]->GetHash();
    tx.vin[1].prevout.n = 0;
    tx.vout[0].nValue = 4100000000LL;
    hash = tx.GetHash();
    mempool.addUnchecked(hash, entry.Fee(400000000LL).Time(GetTime()).SpendsCoinbase(true).FromTx(tx));
    BOOST_CHECK(pblocktemplate = CreateNewBlock(chainparams, scriptPubKey));
    delete pblocktemplate;
    mempool.clear();

    // coinbase in mempool, template creation fails
    tx.vin.resize(1);
    tx.vin[0].prevout.SetNull();
    tx.vin[0].scriptSig = CScript() << OP_0 << OP_1;
    tx.vout[0].nValue = 0;
    hash = tx.GetHash();
    // give it a fee so it'll get mined
    mempool.addUnchecked(hash, entry.Fee(100000).Time(GetTime()).SpendsCoinbase(false).FromTx(tx));
    BOOST_CHECK_THROW(CreateNewBlock(chainparams, scriptPubKey), std::runtime_error);
    mempool.clear();

    // invalid (pre-p2sh) txn in mempool, template creation fails
    tx.vin[0].prevout.hash = txFirst[0]->GetHash();
    tx.vin[0].prevout.n = 0;
    tx.vin[0].scriptSig = CScript() << OP_1;
    tx.vout[0].nValue = 3100000000LL;
    script = CScript() << OP_0;
    tx.vout[0].scriptPubKey = GetScriptForDestination(CScriptID(script));
    hash = tx.GetHash();
    mempool.addUnchecked(hash, entry.Fee(10000000L).Time(GetTime()).SpendsCoinbase(true).FromTx(tx));
    tx.vin[0].prevout.hash = hash;
    tx.vin[0].scriptSig = CScript() << std::vector<unsigned char>(script.begin(), script.end());
    tx.vout[0].nValue -= 1000000;
    hash = tx.GetHash();
    mempool.addUnchecked(hash, entry.Fee(1000000).Time(GetTime()).SpendsCoinbase(false).FromTx(tx));
    BOOST_CHECK_THROW(CreateNewBlock(chainparams, scriptPubKey), std::runtime_error);
    mempool.clear();

    // double spend txn pair in mempool, template creation fails
    tx.vin[0].prevout.hash = txFirst[0]->GetHash();
    tx.vin[0].scriptSig = CScript() << OP_1;
    tx.vout[0].nValue = 3100000000LL;
    tx.vout[0].scriptPubKey = CScript() << OP_1;
    hash = tx.GetHash();
    mempool.addUnchecked(hash, entry.Fee(100000000L).Time(GetTime()).SpendsCoinbase(true).FromTx(tx));
    tx.vout[0].scriptPubKey = CScript() << OP_2;
    hash = tx.GetHash();
    mempool.addUnchecked(hash, entry.Fee(100000000L).Time(GetTime()).SpendsCoinbase(true).FromTx(tx));
    BOOST_CHECK_THROW(CreateNewBlock(chainparams, scriptPubKey), std::runtime_error);
    mempool.clear();

    // subsidy changing
    int nHeight = chainActive.Height();
    chainActive.Tip()->nHeight = 209999;
    BOOST_CHECK(pblocktemplate = CreateNewBlock(chainparams, scriptPubKey));
    delete pblocktemplate;
    chainActive.Tip()->nHeight = 210000;
    BOOST_CHECK(pblocktemplate = CreateNewBlock(chainparams, scriptPubKey));
    delete pblocktemplate;
    chainActive.Tip()->nHeight = nHeight;

    // non-final txs in mempool
    SetMockTime(chainActive.Tip()->GetMedianTimePast()+1);

    // height locked
    tx.vin[0].prevout.hash = txFirst[0]->GetHash();
    tx.vin[0].scriptSig = CScript() << OP_1;
    tx.vin[0].nSequence = 0;
    tx.vout[0].nValue = 3100000000LL;
    tx.vout[0].scriptPubKey = CScript() << OP_1;
    tx.nLockTime = chainActive.Tip()->nHeight+1;
    hash = tx.GetHash();
    mempool.addUnchecked(hash, entry.Fee(100000000L).Time(GetTime()).SpendsCoinbase(true).FromTx(tx));
    BOOST_CHECK(!CheckFinalTx(tx, LOCKTIME_MEDIAN_TIME_PAST));

    // time locked
    tx2.vin.resize(1);
    tx2.vin[0].prevout.hash = txFirst[1]->GetHash();
    tx2.vin[0].prevout.n = 0;
    tx2.vin[0].scriptSig = CScript() << OP_1;
    tx2.vin[0].nSequence = 0;
    tx2.vout.resize(1);
    tx2.vout[0].nValue = 3100000000LL;
    tx2.vout[0].scriptPubKey = CScript() << OP_1;
    tx2.nLockTime = chainActive.Tip()->GetMedianTimePast()+1;
    hash = tx2.GetHash();
    mempool.addUnchecked(hash, entry.Fee(100000000L).Time(GetTime()).SpendsCoinbase(true).FromTx(tx2));
    BOOST_CHECK(!CheckFinalTx(tx2, LOCKTIME_MEDIAN_TIME_PAST));

    BOOST_CHECK(pblocktemplate = CreateNewBlock(chainparams, scriptPubKey));

    // Neither tx should have make it into the template.
    BOOST_CHECK_EQUAL(pblocktemplate->block.vtx.size(), 1);
    delete pblocktemplate;

    // However if we advance height and time by one, both will.
    chainActive.Tip()->nHeight++;
    SetMockTime(chainActive.Tip()->GetMedianTimePast()+2);

    // FIXME: we should *actually* create a new block so the following test
    //        works; CheckFinalTx() isn't fooled by monkey-patching nHeight.
    //BOOST_CHECK(CheckFinalTx(tx));
    //BOOST_CHECK(CheckFinalTx(tx2));

    BOOST_CHECK(pblocktemplate = CreateNewBlock(chainparams, scriptPubKey));
    BOOST_CHECK_EQUAL(pblocktemplate->block.vtx.size(), 2);
    delete pblocktemplate;

    chainActive.Tip()->nHeight--;
    SetMockTime(0);
    mempool.clear();

    BOOST_FOREACH(CTransaction *tx, txFirst)
        delete tx;

    fCheckpointsEnabled = true;
}

BOOST_AUTO_TEST_SUITE_END()
