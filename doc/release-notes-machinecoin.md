Machinecoin Core version 0.10.2.2 is now available from:

  <https://github.com/machinecoin-project/machinecoin-core/archive/master-0.10.zip>

This is a new major version release, bringing bug fixes and translation 
updates.

Please report bugs using the issue tracker at github:

  <https://github.com/machinecoin-project/machinecoin-core/issues>

Upgrading and downgrading
=========================

How to Upgrade
--------------

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes for older versions), then run the
installer (on Windows) or just copy over /Applications/Machinecoin-Qt (on Mac) or
machinecoind/machinecoin-qt (on Linux).

Downgrade warning
------------------

Because release 0.10+ and later makes use of headers-first synchronization and
parallel block download (see further), the block files and databases are not
backwards-compatible with pre-0.10 versions of Machinecoin Core or other software:

* Blocks will be stored on disk out of order (in the order they are
received, really), which makes it incompatible with some tools or
other programs. Reindexing using earlier versions will also not work
anymore as a result of this.

* The block index database will now hold headers for which no block is
stored on disk, which earlier versions won't support.

If you want to be able to downgrade smoothly, make a backup of your entire data
directory. Without this your node will need start syncing (or importing from
bootstrap.dat) anew afterwards. It is possible that the data from a completely
synchronised 0.10 node may be usable in older versions as-is, but this is not
supported and may break as soon as the older version attempts to reindex.

This does not affect wallet forward or backward compatibility.


Machinecoin 0.10.2.2 Change log
============================
This release is based upon Bitcoin Core v0.10.2.  Their upstream changelog applies to us and
is included in as separate release-notes.  This section describes the Machinecoin-specific differences.

Protocol:
- Scrypt Proof-of-Work instead of sha256d, however block hashes are sha256d for performance reasons.
- Machinecoin TCP port 40333 (instead of 8333)
- RPC TCP port 40332 (instead of 8332)
- Testnet TCP port 50333 (instead of 18333)
- Testnet RPC TCP port 50332 (instead of 18332)
- 84 million coin limit  (instead of 21 million)
- Magic 0xfbc0b6db       (instead of 0xf9beb4d9)
- Target Block Time 2.5 minutes (instead of 10 minutes)
- Target Timespan 2.5 minutes      (instead of two weeks)
- bnProofOfWorkLimit = >> 20    (instead of >> 32)
- zeitgeist2 protection: Slightly different way to calculate difficulty changes.
- Machinecoin Core v0.10.2.2 is protocol version 70003 (instead of 70002)

Relay:
- Machinecoin Core rounds transaction size up to the nearest 1000 bytes before calculating fees.  This size rounding behavior is to mimic fee calculation of Machinecoin v0.6 and v0.8.
- Bitcoin's IsDust() is disabled in favor of Machinecoin's fee-based dust penalty.
- Fee-based Dust Penalty: For each transaction output smaller than DUST_THRESHOLD (currently 0.001 MAC) the default relay/mining policy will expect an additional 1000 bytes of fee.  Otherwise the transaction will be rejected from relay/mining.  Such transactions are also disqualified from the free/high-priority transaction rule.
- Miners and relays can adjust the expected fee per-KB with the -minrelaytxfee parameter.

Wallet:
- Coins smaller than 0.00001 MAC are by default ignored by the wallet.  Use the -mininput parameter if you want to see smaller coins.

Notable changes since Machinecoin v0.8.7.4
===================================

- The Block data and indexes of v0.10 are incompatible with v0.8.7.4 clients.  You can upgrade from v0.8.7.4 but you downgrading is not possible.  For this reason you may want to make a backup copy of your Data Directory.
- machinecoind no longer sends RPC commands.  You must use the separate machinecoin-cli command line utility.
- Watch-Only addresses are now possible.


0.8.7.4 changes
=============

- Upgrade OpenSSL to 1.0.1k based on several bugs in prior versions of OpenSSL.
  Machinecoin patched to work with OpenSSL CVE-2014-8275 which broke 
  compatibility with Machinecoin.

- Added several new checkpoints

- Minor fixes:
  - Fix request of external ip
  - Fix compiler warnings
  - Update coin icon sets

- Thanks to all the contributors and sponsors that made this update possible:
  - Dirk
  - Ria
  - Nico
  - Julia
  - Michael
  - Andrea
  - Anonymous

0.8.7.3 changes
=============

- Digishield Retarget

0.8.7.2 changes
=============

- Mac and Windows Official Gitian Builds: upgrade to openssl-1.0.1h for CVE-2014-0224
                   Linux Gitian build uses Lucid 0.9.8k-7ubuntu8.18

0.8.7.1 changes
=============

- Mac and Windows Official Gitian Builds: upgrade to openssl-1.0.1g for CVE-2014-0160
                   Linux was not vulnerable with Lucid openssl-0.9.8k
                   Older versions were only vulnerable with rarely used RPC SSL
- If you build from source, be sure that your openssl is patched for CVE-2014-0160.
- Upgrade openssl, qt, miniupnpc, zlib, libpng, qrencode
- Many bug fixes from Bitcoin 0.8.7rc stable branch
    including transaction malleability mitigation backports from 0.9
- Add testnet checkpoints
- Add new testnet seed

0.8.6.2 changes
=============

- Windows only: Fixes issue where network connectivity can fail.

- Cleanup of SSE2 scrypt detection.

- Minor fixes:
  - s/Bitcoin/Machinecoin/ in the Coin Control example
  - Fix custom build on MacOS X 10.9
  - Fix QT5 custom build
  - Update Debian build instructions
  - Update Homebrew build 

0.8.6.1 changes
=============

- Coin Control - experts only GUI selection of inputs before you send a transaction

- Disable Wallet - reduces memory requirements, helpful for miner or relay nodes

- 20x reduction in default mintxfee.

- Up to 50% faster PoW validation, faster sync and reindexing.

- Peers older than protocol version 70002 are disconnected.  0.8.3.7 is the oldest compatible client.

- Internal miner added back to Machinecoin.  setgenerate now works, although it is generally a bad idea as it is significantly slower than external CPU miners.

- New RPC commands: getbestblockhash and verifychain

- Improve fairness of the high priority transaction space per block

- OSX block chain database corruption fixes
  - Update leveldb to 1.13
  - Use fcntl with `F_FULLSYNC` instead of fsync on OSX
  - Use native Darwin memory barriers
  - Replace use of mmap in leveldb for improved reliability (only on OSX)

- Fix nodes forwarding transactions with empty vins and getting banned

- Network code performance and robustness improvements

- Additional debug.log logging for diagnosis of network problems, log timestamps by default

- Fix rare GUI crash on send

0.8.5.1 changes
===============

Workaround negative version numbers serialization bug.

Fix out-of-bounds check (Machinecoin currently does not use this codepath, but we apply this
patch just to match Bitcoin 0.8.5.)

0.8.4.1 changes
===============

CVE-2013-5700 Bloom: filter crash issue - Machinecoin 0.8.3.7 disabled bloom by default so was 
unaffected by this issue, but we include their patches anyway just in case folks want to 
enable bloomfilter=1.

CVE-2013-4165: RPC password timing guess vulnerability

CVE-2013-4627: Better fix for the fill-memory-with-orphaned-tx attack

Fix multi-block reorg transaction resurrection.

Fix non-standard disconnected transactions causing mempool orphans.  This bug could cause 
nodes running with the -debug flag to crash, although it was lot less likely on Machinecoin 
as we disabled IsDust() in 0.8.3.x.

Mac OSX: use 'FD_FULLSYNC' with LevelDB, which will (hopefully!) prevent the database 
corruption issues have experienced on OSX.

Add height parameter to getnetworkhashps.

Fix Norwegian and Swedish translations.

Minor efficiency improvement in block peer request handling.


0.8.3.7 changes
===============

Fix CVE-2013-4627 denial of service, a memory exhaustion attack that could crash low-memory nodes.

Fix a regression that caused excessive writing of the peers.dat file.

Add option for bloom filtering.

Fix Hebrew translation.
