//
//  BRWallet.h
//
//  Created by Aaron Voisine on 9/1/15.
//  Copyright (c) 2015 breadwallet LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef BRWallet_h
#define BRWallet_h

#include "BRTypes.h"
#include "BRTransaction.h"
#include "BRBIP32Sequence.h"
#include <string.h>

typedef struct {
    UInt256 hash;
    uint32_t n;
} BRUTXO;

inline static int BRUTXOEq(const void *a, const void *b)
{
    return (UInt256Eq(((const BRUTXO *)a)->hash, ((const BRUTXO *)b)->hash) &&
            ((const BRUTXO *)a)->n == ((const BRUTXO *)b)->n);
}

inline static size_t BRUTXOHash(const void *utxo)
{
    return (((const BRUTXO *)utxo)->hash.u32[0] ^ ((const BRUTXO *)utxo)->n)*0x01000193; // (hash xor n)*FNV_PRIME
}

typedef struct _BRWallet BRWallet;

// allocate and populate a wallet
BRWallet *BRWalletNew(BRTransaction *transactions[], size_t txCount, BRMasterPubKey mpk,
                      void *(*seed)(const char *, uint64_t, size_t *));

void BRWalletSetCallbacks(BRWallet *wallet,
                          void (*balanceChanged)(BRWallet *wallet, uint64_t balance, void *info),
                          void (*txAdded)(BRWallet *wallet, BRTransaction *tx, void *info),
                          void (*txUpdated)(BRWallet *wallet, UInt256 txHash, uint32_t blockHeight, uint32_t timestamp,
                                            void *info),
                          void (*txDeleted)(BRWallet *wallet, UInt256 txHash, void *info),
                          void *info);

// current wallet balance, not including transactions known to be invalid
uint64_t BRWalletBalance(BRWallet *wallet);

// list of all unspent outputs
BRUTXO *BRWalletUTXOs(BRWallet *wallet, size_t *count);

// all transactions registered in the wallet, sorted by date, oldest first
BRTransaction **BRWalletTransactions(BRWallet *wallet, size_t *count);

// total amount spent from the wallet (exluding change)
uint64_t BRWalletTotalSent(BRWallet *wallet);

// total amount received by the wallet (exluding change)
uint64_t BRWalletTotalReceived(BRWallet *wallet);

// fee-per-kb of transaction size to use when creating a transaction
void BRWalletSetFeePerKb(BRWallet *wallet, uint64_t feePerKb);

// returns the first unused external address
const char *BRWalletReceiveAddress(BRWallet *wallet);

// returns the first unused internal address
const char *BRWalletChangeAddress(BRWallet *wallet);

// true if the given txHash is registered in the wallet
int BRWalletContainsTxHash(BRWallet *wallet, UInt256 txHash);

// true if the address is controlled by the wallet
int BRWalletContainsAddress(BRWallet *wallet, const char *addr);

// true if the address was previously used as an input or output in any wallet transaction
int BRWalletAddressIsUsed(BRWallet *wallet, const char *addr);

// returns an unsigned transaction that sends the specified amount from the wallet to the given address, result must be
// freed using BRTransactionFree()
BRTransaction *BRWalletCreateTransaction(BRWallet *wallet, uint64_t amount, const char *addr);

// sign any inputs in the given transaction that can be signed using private keys from the wallet
int BRWalletSignTransaction(BRWallet *wallet, BRTransaction *tx, const char *authPrompt);

// true if the given transaction is associated with the wallet (even if it hasn't been registered)
int BRWalletContainsTransaction(BRWallet *wallet, BRTransaction *tx);

// adds a transaction to the wallet, or returns false if it isn't associated with the wallet
int BRWalletRegisterTransaction(BRWallet *wallet, BRTransaction *tx);

// removes a transaction from the wallet along with any transactions that depend on its outputs
void BRWalletRemoveTransaction(BRWallet *wallet, UInt256 txHash);

// returns the transaction with the given hash if it's been registered in the wallet
const BRTransaction *BRWalletTransactionForHash(BRWallet *wallet, UInt256 txHash);

// true if no previous wallet transaction spends any of the given transaction's inputs, and no input tx is invalid
int BRWalletTransactionIsValid(BRWallet *wallet, BRTransaction *tx);

// returns true if transaction won't be valid by blockHeight + 1 or within the next 10 minutes
int BRWalletTransactionIsPostdated(BRWallet *wallet, BRTransaction *tx, uint32_t blockHeight);

// set the block height and timestamp for the given transaction
void BRWalletUpdateTransaction(BRWallet *wallet, UInt256 txHash, uint32_t blockHeight, uint32_t timestamp);

// returns the amount received by the wallet from the transaction (total outputs to change and/or receive addresses)
uint64_t BRWalletAmountReceivedFromTx(BRWallet *wallet, BRTransaction *tx);

// retuns the amount sent from the wallet by the trasaction (total wallet outputs consumed, change and fee included)
uint64_t BRWalletAmountSentByTx(BRWallet *wallet, BRTransaction *tx);

// returns the fee for the given transaction if all its inputs are from wallet transactions, ULLONG_MAX otherwise
uint64_t BRWalletFeeForTx(BRWallet *wallet, BRTransaction *tx);

// historical wallet balance after the given transaction, or current balance if transaction is not registered in wallet
uint64_t BRWalletBalanceAfterTx(BRWallet *wallet, BRTransaction *tx);

// fee that will be added for a transaction of the given size in bytes
uint64_t BRWalletFeeForTxSize(BRWallet *wallet, size_t size);

// frees memory allocated for wallet, also calls BRTransactionFree() for all registered transactions
void BRWalletFree(BRWallet *wallet);

// returns the given amount in local currency units, price is local currency units per bitcoin
uint64_t BRLocalAmount(uint64_t amount, double price);

// returns the given local currency amount in satoshis, price is local currency units per bitcoin
uint64_t BRBitcoinAmount(uint64_t localAmount, double price);

#endif // BRWallet_h