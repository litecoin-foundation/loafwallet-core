//
//  BRChainParams.h
//
//  Created by Aaron Voisine on 1/10/18.
//  Copyright (c) 2019 breadwallet LLC
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

#ifndef BRChainParams_h
#define BRChainParams_h

#include "BRMerkleBlock.h"
#include <assert.h>

typedef struct {
    uint32_t height;
    UInt256 hash;
    uint32_t timestamp;
    uint32_t target;
} BRCheckPoint;

typedef struct {
    const char * const *dnsSeeds; // NULL terminated array of dns seeds
    uint16_t standardPort;
    uint32_t magicNumber;
    uint64_t services;
    int (*verifyDifficulty)(const BRMerkleBlock *block, const BRMerkleBlock *previous, uint32_t transitionTime);
    const BRCheckPoint *checkpoints;
    size_t checkpointsCount;
} BRChainParams;

static const char *BRMainNetDNSSeeds[] = {
    "dnsseed.litecointools.com.", "dnsseed.litecoinpool.org.", "seed-a.litecoin.loshan.co.uk.",
    "dnsseed.thrasher.io.", "dnsseed.koin-project.com.", NULL
};

static const char *BRTestNetDNSSeeds[] = {
    "testnet-seed.ltc.xurious.com.", "seed-b.litecoin.loshan.co.uk.", "dnsseed-testnet.thrasher.io.", NULL
};

// blockchain checkpoints - these are also used as starting points for partial chain downloads, so they must be at
// difficulty transition boundaries in order to verify the block difficulty at the immediately following transition
static const BRCheckPoint BRMainNetCheckpoints[] = {
    {       0, uint256("12a765e31ffd4059bada1e25190f6e98c99d9714d334efa41a195a7e7e04bfe2"), 1317972665, 0x1e0ffff0 },
    {   20160, uint256("633036c8df655531c2449b2d09b264cc0b49d945a89be23fd3c1a97361ca198c"), 1319798300, 0x1d055262 },
    {   40320, uint256("d148cdd2cf44069cef4b63f0feaf30a8d291ca9ea9ba7e83f226b9738c1d5e9c"), 1322522019, 0x1d018053 },
    {   60480, uint256("3250f0a560d55f039c34bfaee1b71297aa5104ac6641778f9a87d73232d12c6c"), 1325540574, 0x1d00e848 },
    {   80640, uint256("bedc0a090b740b1902d870aeb6caa89040a24e7d670d46f8ef035fd9d2e9ce80"), 1328779944, 0x1d00ab92 },
    {  100800, uint256("7b0b620d15f781faaaa73b43607a49d5becb2b803ef19b4010014646cc177a61"), 1331873688, 0x1d00ae9f },
    {  120960, uint256("dbd6249f30e5690890bc03dabcc0a526c46adcde572be06af4075b6ea28aa251"), 1334881566, 0x1d009e48 },
    {  161280, uint256("f595c754d0abcfe3616573bfabee01b230ec0ea6b2f2894c40214ea23d772b6c"), 1340918301, 0x1d008881 },
    {  201600, uint256("d481df8e8ce144fca9ae6b3157cc706e903c6ea161a13d2c421270354a02d6d0"), 1346567025, 0x1c1c89e8 },
    {  221760, uint256("88cf3446129161a633050244f112e3041a2d53152ee9293984b20f468fbadb8a"), 1349481542, 0x1c135d42 },
    {  262080, uint256("13a5d47f01fe3ab17ebf2b15b605efa41efe06b02bb685bc2ad4cec22af0b478"), 1355560195, 0x1c0a01e5 },
    {  302400, uint256("e798d897a837bf4989d329266128754ec1cbeff1eb0c0afd67f71d2b7c44bdaa"), 1361913149, 0x1c102ea7 },
    {  342720, uint256("33f62e026a202be550e8a9df37d638d38991553544e279cb264123378bf46042"), 1367113967, 0x1c0095a5 },
    {  383040, uint256("5c0a443361c1356796a7db472c69433b6ce6108d61e4403fd9a9d91e01009ce3"), 1372971948, 0x1b481262 },
    {  403200, uint256("ef78aa1925cc51ff8dc3a1e59f389c89845fb8b9e566348222e663e963e67640"), 1376014028, 0x1b4b858d },
    {  443520, uint256("37d668803ed1efc24ffab4a2a90da9ac92679acf68370d7570f042c2bd6d651b"), 1382034998, 0x1b3f864f },
    {  483840, uint256("759de6c4e6161fc8c996cf0d5e012ee0afc52a037e657dd54e85da9a9f803633"), 1387792541, 0x1b167254 },
    {  504000, uint256("97db0624d3d5137bc085f0d731607314972bb4124b85b73420ef9aa5fc10d640"), 1390892377, 0x1b1aa868 },
    {  544320, uint256("95ae252971d1ec9deeed1ed19fe9537e04348a82839a9e2bf8856faaa03e324e"), 1396719779, 0x1b0a9622 },
    {  564480, uint256("c876276bf12754c2b265787d9e7ab83d429e59761dc63057f728529018db7834"), 1399724592, 0x1b099dce },
    {  584640, uint256("df5454af79491c392fe740b5efd47afbe1cb53cd8d86be3ab9c97fdd2786d237"), 1402630524, 0x1b065b94 },
    {  604800, uint256("43c1a80b8abaf57817e5daea9cfdde99ea5f324705779045792ccad52d54f3d4"), 1405459509, 0x1b033d34 },
    {  645120, uint256("9b7ddc3753c5138fc471accd15f9730020e828bc69058f2e382549c7c0ffba0f"), 1411376787, 0x1b020a10 },
    {  685440, uint256("29d2328990dda4c4870846d4e3d573785452bed68e6013930a83fc8d5fe89b09"), 1417289378, 0x1b01473b },
    {  705600, uint256("e350118d9047c1ca5f047a1b1ee400562fb0cfb8b3c8032b56b8545b456a03ab"), 1420305710, 0x1b01399e },
    {  745920, uint256("04809a35ff6e5054e21d14582072605b812b7d4ae11d3450e7c03a7237e1d35d"), 1426441593, 0x1b019b8c },
    {  786240, uint256("d1b9fa6999f7a09d1dc52511750e47d263aaa7ea4a262762fff8665890d631a5"), 1432507384, 0x1b01a8ec },
    {  806400, uint256("e2363e8b3e8f237b9b1bfc1c72ede80fef2c7bd1aabcd78afed82065a194b960"), 1435516150, 0x1b019268 },
    {  846720, uint256("6f5d94d7cfd01f1dbf4aa631b987f8e2ec9d0c57720604787b816bafe34192a8"), 1441561050, 0x1b0187a3 },
    {  901152, uint256("cfccdf8e3830ae4879e910051ac3dc583b4fb45b83be3a38019e5d9326dfa223"), 1449698771, 0x1b015b0e },
    {  941472, uint256("4fddb941d414f071c29f100da2a160cf527397fc9a7a9c9d0a849b6f67799042"), 1455719547, 0x1b0133ec },
    {  953568, uint256("e46e01cf1239cffa69408ac162d517bac5a4899972e0328fd0ba4d93e8ad3764"), 1457542869, 0x1b013c91 },
    {  961632, uint256("bfc01091cb21ea81dd079fcee6cf7910087281bfdbcb1ad9e5dbc226b5f45a86"), 1458730622, 0x1b012535 },
    {  993888, uint256("1d80e7793bd9e16e0ce84d93b105d6732ed63e1a6fe491c1b7ea310e75eb504e"), 1463613744, 0x1b014cbd },
    { 1001952, uint256("eccbede26ac99ea996377972d5bd05b9306bcc6ac1f4071f1587e3094a704dff"), 1464900396, 0x1b01a29e },
    { 1058400, uint256("76ce37c66d449a4ffbfc35674cf932da701066a001dc223754f9250dd2bdbc62"), 1473296285, 0x1b013ca7 },
    { 1260000, uint256("85a22b528d805bf7a641d1d7c6d96ef5054beda3dcab6be7b83f2e3df24b33a8"), 1502976600, 0x1a25a0d3 }
};

static const BRCheckPoint BRTestNetCheckpoints[] = {
    {       0, uint256("4966625a4b2851d9fdee139e56211a0d88575f59ed816ff5e6a63deb4e3e29a0"), 1486949366, 0x1e0ffff0 }
};

static int BRTestNetVerifyDifficulty(const BRMerkleBlock *block, const BRMerkleBlock *previous, uint32_t transitionTime)
{
    int r = 1;

    assert(block != NULL);
    assert(previous != NULL);

    if (! previous || !UInt256Eq(block->prevBlock, previous->blockHash) || block->height != previous->height + 1) r = 0;
    if (r && (block->height % BLOCK_DIFFICULTY_INTERVAL) == 0 && transitionTime == 0) r = 0;
    return r;
}

static const BRChainParams BRMainNetParams = {
    BRMainNetDNSSeeds,
    9333,       // standardPort
    0xdbb6c0fb, // magicNumber
    0,          // services
    BRMerkleBlockVerifyDifficulty,
    BRMainNetCheckpoints,
    sizeof(BRMainNetCheckpoints)/sizeof(*BRMainNetCheckpoints)
};

static const BRChainParams BRTestNetParams = {
    BRTestNetDNSSeeds,
    19335,      // standardPort
    0xf1c8d2fd, // magicNumber
    0,          // services
    BRTestNetVerifyDifficulty,
    BRTestNetCheckpoints,
    sizeof(BRTestNetCheckpoints)/sizeof(*BRTestNetCheckpoints)
};

#endif // BRChainParams_h
