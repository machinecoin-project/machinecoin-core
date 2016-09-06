Shared Libraries
================

## machinecoinconsensus

The purpose of this library is to make the verification functionality that is critical to Machinecoin's consensus available to other applications, e.g. to language bindings.

### API

The interface is defined in the C header `machinecoinconsensus.h` located in  `src/script/machinecoinconsensus.h`.

#### Version

`machinecoinconsensus_version` returns an `unsigned int` with the API version *(currently at an experimental `0`)*.

#### Script Validation

`machinecoinconsensus_verify_script` returns an `int` with the status of the verification. It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters
- `const unsigned char *scriptPubKey` - The previous output script that encumbers spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `machinecoinconsensus_error* err` - Will have the error/success code for the operation *(see below)*.

##### Script Flags
- `machinecoinconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `machinecoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH ([MIP16](https://github.com/machinecoin-project/mips/blob/master/mip-0016.mediawiki)) subscripts
- `machinecoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER ([MIP66](https://github.com/machinecoin-project/mips/blob/master/mip-0066.mediawiki)) compliance

##### Errors
- `machinecoinconsensus_ERR_OK` - No errors with input parameters *(see the return value of `machinecoinconsensus_verify_script` for the verification status)*
- `machinecoinconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `machinecoinconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size of `txTo`
- `machinecoinconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`

### Example Implementations
- [NMachinecoin](https://github.com/NicolasDorier/NMachinecoin/blob/master/NMachinecoin/Script.cs#L814) (.NET Bindings)
- [node-libmachinecoinconsensus](https://github.com/bitpay/node-libmachinecoinconsensus) (Node.js Bindings)
- [java-libmachinecoinconsensus](https://github.com/dexX7/java-libmachinecoinconsensus) (Java Bindings)
- [machinecoinconsensus-php](https://github.com/Bit-Wasp/machinecoinconsensus-php) (PHP Bindings)
