//
//  BRCryptoFeeBasis.c
//  Core
//
//  Created by Ed Gamble on 5/15/19.
//  Copyright © 2019 Breadwinner AG. All rights reserved.
//
//  See the LICENSE file at the project root for license information.
//  See the CONTRIBUTORS file at the project root for a list of contributors.
//
#include <math.h>

#include "BRCryptoFeeBasis.h"
#include "ethereum/ewm/BREthereumBase.h"
#include "generic/BRGeneric.h"
#include "BRCryptoPrivate.h"

static void
cryptoFeeBasisRelease (BRCryptoFeeBasis feeBasis);

struct BRCryptoFeeBasisRecord {
    BRCryptoBlockChainType type;
    union {
        struct {
            uint32_t feePerKB;
            uint32_t sizeInByte;
        } btc;
        BREthereumFeeBasis eth;
        struct {
            BRGenericWalletManager gwm;
            BRGenericFeeBasis bid;
        } gen;
    } u;
    BRCryptoUnit unit;
    BRCryptoRef ref;
};

IMPLEMENT_CRYPTO_GIVE_TAKE (BRCryptoFeeBasis, cryptoFeeBasis)

static BRCryptoFeeBasis
cryptoFeeBasisCreateInternal (BRCryptoBlockChainType type,
                              BRCryptoUnit unit) {
    BRCryptoFeeBasis feeBasis = malloc (sizeof (struct BRCryptoFeeBasisRecord));

    feeBasis->type = type;
    feeBasis->unit = cryptoUnitTake (unit);
    feeBasis->ref  = CRYPTO_REF_ASSIGN (cryptoFeeBasisRelease);

    return feeBasis;
}

private_extern BRCryptoFeeBasis
cryptoFeeBasisCreateAsBTC (BRCryptoUnit unit,
                           uint32_t feePerKB,
                           uint32_t sizeInByte) {
    BRCryptoFeeBasis feeBasis = cryptoFeeBasisCreateInternal (BLOCK_CHAIN_TYPE_BTC, unit);
    feeBasis->u.btc.feePerKB   = feePerKB;
    feeBasis->u.btc.sizeInByte = sizeInByte;

    return feeBasis;
}

private_extern BRCryptoFeeBasis
cryptoFeeBasisCreateAsETH (BRCryptoUnit unit,
                           BREthereumGas gas,
                           BREthereumGasPrice gasPrice) {
    BRCryptoFeeBasis feeBasis = cryptoFeeBasisCreateInternal (BLOCK_CHAIN_TYPE_ETH, unit);
    feeBasis->u.eth = (BREthereumFeeBasis) {
        FEE_BASIS_GAS,
        { .gas = { gas, gasPrice }}
    };

    return feeBasis;
}

private_extern BRCryptoFeeBasis
cryptoFeeBasisCreateAsGEN (BRCryptoUnit unit,
                           BRGenericWalletManager gwm,
                           OwnershipGiven BRGenericFeeBasis bid) {
    BRCryptoFeeBasis feeBasis = cryptoFeeBasisCreateInternal (BLOCK_CHAIN_TYPE_GEN, unit);
    feeBasis->u.gen.gwm = gwm;
    feeBasis->u.gen.bid = bid;

    return feeBasis;
}

static void
cryptoFeeBasisRelease (BRCryptoFeeBasis feeBasis) {
    cryptoUnitGive (feeBasis->unit);
    switch (feeBasis->type) {
        case BLOCK_CHAIN_TYPE_BTC: break;
        case BLOCK_CHAIN_TYPE_ETH: break;

        case BLOCK_CHAIN_TYPE_GEN:
            // TODO: Release BRGenericFeeBasis
            break;
    }
    free (feeBasis);
}

extern BRCryptoBlockChainType
cryptoFeeBasisGetType (BRCryptoFeeBasis feeBasis) {
    return feeBasis->type;
}

static UInt256
cryptoFeeBasisGetPricePerCostFactorAsUInt256 (BRCryptoFeeBasis feeBasis) {
    switch (feeBasis->type) {
        case BLOCK_CHAIN_TYPE_BTC:
            return createUInt256(feeBasis->u.btc.feePerKB);

        case BLOCK_CHAIN_TYPE_ETH:
            return feeBasis->u.eth.u.gas.price.etherPerGas.valueInWEI;

        case BLOCK_CHAIN_TYPE_GEN:
            assert (0);
            return UINT256_ZERO;
    }
}

extern BRCryptoAmount
cryptoFeeBasisGetPricePerCostFactor (BRCryptoFeeBasis feeBasis) {
    return cryptoAmountCreateInternal (feeBasis->unit,
                                       CRYPTO_FALSE,
                                       cryptoFeeBasisGetPricePerCostFactorAsUInt256 (feeBasis),
                                       1);
}

extern BRCryptoUnit
cryptoFeeBasisGetPricePerCostFactorUnit (BRCryptoFeeBasis feeBasis) {
    return cryptoUnitTake (feeBasis->unit);
}

extern double
cryptoFeeBasisGetCostFactor (BRCryptoFeeBasis feeBasis) {
    switch (feeBasis->type) {
        case BLOCK_CHAIN_TYPE_BTC:
            return ((double) feeBasis->u.btc.sizeInByte) / 1000.0;

        case BLOCK_CHAIN_TYPE_ETH:
            return feeBasis->u.eth.u.gas.limit.amountOfGas;

        case BLOCK_CHAIN_TYPE_GEN:
            assert (0);
            return 0;
    }
}

extern BRCryptoAmount
cryptoFeeBasisGetFee (BRCryptoFeeBasis feeBasis) {
    switch (feeBasis->type) {
        case BLOCK_CHAIN_TYPE_BTC: {
            double fee = (((double) feeBasis->u.btc.feePerKB) * feeBasis->u.btc.sizeInByte) / 1000.0;
            return cryptoAmountCreateInternal (feeBasis->unit,
                                               CRYPTO_FALSE,
                                               createUInt256 (round (fee)),
                                               1);
        }
            
        case BLOCK_CHAIN_TYPE_ETH:
        case BLOCK_CHAIN_TYPE_GEN: {
            UInt256 pricePerCostFactor = cryptoFeeBasisGetPricePerCostFactorAsUInt256 (feeBasis);
            double  costFactor = cryptoFeeBasisGetCostFactor (feeBasis);

            int overflow = 0, negative = 0;
            double rem;

            UInt256 value = mulUInt256_Double (pricePerCostFactor, costFactor, &overflow, &negative, &rem);

            return (overflow
                    ? NULL
                    : cryptoAmountCreateInternal (feeBasis->unit,
                                                  CRYPTO_FALSE,
                                                  value,
                                                  1));
        }
    }
}

private_extern uint64_t // SAT-per-KB
cryptoFeeBasisAsBTC (BRCryptoFeeBasis feeBasis) {
    assert (BLOCK_CHAIN_TYPE_BTC == feeBasis->type);
    return feeBasis->u.btc.feePerKB;
}

private_extern BREthereumFeeBasis
cryptoFeeBasisAsETH (BRCryptoFeeBasis feeBasis) {
    assert (BLOCK_CHAIN_TYPE_ETH == feeBasis->type);
    return feeBasis->u.eth;
}

private_extern BRGenericFeeBasis
cryptoFeeBasisAsGEN (BRCryptoFeeBasis feeBasis) {
    assert (BLOCK_CHAIN_TYPE_GEN == feeBasis->type);
    return feeBasis->u.gen.bid;
}
