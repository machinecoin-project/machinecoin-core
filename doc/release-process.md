Release Process
====================

###update (commit) version in sources

	contrib/verifysfbinaries/verify.sh
	doc/README*
	share/setup.nsi
	src/clientversion.h (change CLIENT_VERSION_IS_RELEASE to true)

###tag version in git

	git tag -s v(new version, e.g. 0.8.0)

###write release notes. git shortlog helps a lot, for example:

	git shortlog --no-merges v(current version, e.g. 0.7.2)..v(new version, e.g. 0.8.0)

* * *

###update gitian

 In order to take advantage of the new caching features in gitian, be sure to update to a recent version (e9741525c or higher is recommended)

###perform gitian builds

 From a directory containing the machinecoin source, gitian-builder and gitian.sigs.mac
  
	export SIGNER=(your gitian key, ie gitju etc)
	export VERSION=(new version, e.g. 0.8.0)
	pushd ./machinecoin
	git checkout v${VERSION}
	popd
	pushd ./gitian-builder

###fetch and build inputs: (first time, or when dependency versions change)
 
	mkdir -p inputs

 Register and download the Apple SDK: (see OSX Readme for details)
 
 https://developer.apple.com/downloads/download.action?path=Developer_Tools/xcode_4.6.3/xcode4630916281a.dmg
 
 Using a Mac, create a tarball for the 10.7 SDK and copy it to the inputs directory:
 
	tar -C /Volumes/Xcode/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/ -czf MacOSX10.7.sdk.tar.gz MacOSX10.7.sdk

###Optional: Seed the Gitian sources cache

  By default, gitian will fetch source files as needed. For offline builds, they can be fetched ahead of time:

	make -C ../machinecoin/depends download SOURCES_PATH=`pwd`/cache/common

  Only missing files will be fetched, so this is safe to re-run for each build.

###Build Machinecoin Core for Linux, Windows, and OS X:
  
	./bin/gbuild --commit machinecoin=v${VERSION} ../machinecoin/contrib/gitian-descriptors/gitian-linux.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-linux --destination ../gitian.sigs.mac/ ../machinecoin/contrib/gitian-descriptors/gitian-linux.yml
	mv build/out/machinecoin-*.tar.gz build/out/src/machinecoin-*.tar.gz ../
	./bin/gbuild --commit machinecoin=v${VERSION} ../machinecoin/contrib/gitian-descriptors/gitian-win.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-win --destination ../gitian.sigs.mac/ ../machinecoin/contrib/gitian-descriptors/gitian-win.yml
	mv build/out/machinecoin-*.zip build/out/machinecoin-*.exe ../
	./bin/gbuild --commit machinecoin=v${VERSION} ../machinecoin/contrib/gitian-descriptors/gitian-osx.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-osx-unsigned --destination ../gitian.sigs.mac/ ../machinecoin/contrib/gitian-descriptors/gitian-osx.yml
	mv build/out/machinecoin-*-unsigned.tar.gz inputs/machinecoin-osx-unsigned.tar.gz
	mv build/out/machinecoin-*.tar.gz build/out/machinecoin-*.dmg ../
	popd
  Build output expected:

  1. source tarball (machinecoin-${VERSION}.tar.gz)
  2. linux 32-bit and 64-bit binaries dist tarballs (machinecoin-${VERSION}-linux[32|64].tar.gz)
  3. windows 32-bit and 64-bit installers and dist zips (machinecoin-${VERSION}-win[32|64]-setup.exe, machinecoin-${VERSION}-win[32|64].zip)
  4. OSX unsigned installer (machinecoin-${VERSION}-osx-unsigned.dmg)
  5. Gitian signatures (in gitian.sigs/${VERSION}-<linux|win|osx-unsigned>/(your gitian key)/

###Next steps:

Commit your signature to gitian.sigs:

	pushd gitian.sigs
	git add ${VERSION}-linux/${SIGNER}
	git add ${VERSION}-win/${SIGNER}
	git add ${VERSION}-osx-unsigned/${SIGNER}
	git commit -a
	git push  # Assuming you can push to the gitian.sigs tree
	popd

  Wait for OSX detached signature:
	Once the OSX build has 3 matching signatures, Gitju will sign it with the apple App-Store key.
	He will then upload a detached signature to be combined with the unsigned app to create a signed binary.

  Create the signed OSX binary:

	pushd ./gitian-builder
	# Fetch the signature as instructed by Gitju
	cp signature.tar.gz inputs/
	./bin/gbuild -i ../machinecoin/contrib/gitian-descriptors/gitian-osx-signer.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-osx-signed --destination ../gitian.sigs/ ../machinecoin/contrib/gitian-descriptors/gitian-osx-signer.yml
	mv build/out/machinecoin-osx-signed.dmg ../machinecoin-${VERSION}-osx.dmg
	popd

Commit your signature for the signed OSX binary:

	pushd gitian.sigs
	git add ${VERSION}-osx-signed/${SIGNER}
	git commit -a
	git push  # Assuming you can push to the gitian.sigs tree
	popd

-------------------------------------------------------------------------

### After 3 or more people have gitian-built and their results match:

- Perform code-signing.

    - Code-sign Windows -setup.exe (in a Windows virtual machine using signtool)

  Note: only Gitju has the code-signing keys currently.

- Create `SHA256SUMS.asc` for the builds, and GPG-sign it:
```bash
sha256sum * > SHA256SUMS
gpg --digest-algo sha256 --clearsign SHA256SUMS # outputs SHA256SUMS.asc
rm SHA256SUMS
```
(the digest algorithm is forced to sha256 to avoid confusion of the `Hash:` header that GPG adds with the SHA256 used for the files)

- Update machinecoin.org version

- Announce the release:

  - Release sticky on machinecointalk: https://machinecointalk.org/index.php?board=1.0

  - machinecoin-development mailing list

  - Update title of #machinecoin on Freenode IRC

  - Optionally reddit /r/machinecoin, ... but this will usually sort out itself

- Add release notes for the new version to the directory `doc/release-notes` in git master

- Celebrate 
