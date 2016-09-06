#!/usr/bin/env python3
# Copyright (c) 2016 The Machinecoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

#
# Helper script to create the cache
# (see MachinecoinTestFramework.setup_chain)
#

from test_framework.test_framework import MachinecoinTestFramework

class CreateCache(MachinecoinTestFramework):

    def setup_network(self):
        # Don't setup any test nodes
        self.options.noshutdown = True

    def run_test(self):
        pass

if __name__ == '__main__':
    CreateCache().main()
