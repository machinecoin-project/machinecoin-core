// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{
    uint256 p23021 = uint256("0x0268f4e816aac0874c911c83e263353289854c94a21cf97675652419893e7d8f");
    uint256 p53600 = uint256("0x327ec569aa2439c16542ed9402884f5ce691d08f49168d672f19f817ace7a06b");
    BOOST_CHECK(Checkpoints::CheckBlock(23021, p23021));
    BOOST_CHECK(Checkpoints::CheckBlock(53600, p53600));

    
    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(23021, p53600));
    BOOST_CHECK(!Checkpoints::CheckBlock(53600, p23021));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(23021+1, p53600));
    BOOST_CHECK(Checkpoints::CheckBlock(53600+1, p23021));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 53600);
}    

BOOST_AUTO_TEST_SUITE_END()
