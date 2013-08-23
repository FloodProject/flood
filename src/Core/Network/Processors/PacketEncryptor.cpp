/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/Processors/PacketEncryptor.h"
#include "Core/Network/Peer.h"
#include "Core/Network/Packet.h"
#include "Core/Network/Network.h"
#include "Core/Log.h"
#include "Core/Utilities.h"

#include <tropicssl/aes.h>
#include <tropicssl/sha1.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

PacketEncryptor::PacketEncryptor(Array<uint8> secret)
{
    aes = Allocate(AllocatorGetNetwork(), aes_context);
    this->secret = secret;
    //sha1(secret.data(), secret.size(), iv);
    memset(iv,0,16);
}

PacketEncryptor::~PacketEncryptor()
{
    Deallocate(aes);
}

bool PacketEncryptor::processInPacket(Peer* peer, Packet* packet, int channelId)
{
    if(!GetBitFlag(packet->getFlags(), PacketFlags::Encrypted))
        return true;

    auto stream = packet->getMemoryStream();
	int size = stream->getPosition();
    uint8* data = stream->data.data();

    // Decrypt
    aes_setkey_dec(aes, secret.data(), secret.size());
    aes_crypt_cbc(aes, AES_DECRYPT, size, iv, data, data);

    // Remove AES padding
    uint8 p = *(data+size-1);
	stream->setPosition(-p-1,  StreamSeekMode::Relative);

    return true;
}

bool PacketEncryptor::processOutPacket(Peer* peer, Packet* packet, int channelId)
{
    if(!GetBitFlag(packet->getFlags(), PacketFlags::Encrypted))
        return true;

    auto stream = packet->getMemoryStream();

    // Add AES padding
	uint8 p = 16 - (stream->getPosition()+1)%16;
	stream->setPosition(p, StreamSeekMode::Relative);
	stream->write(&p, 1);

    // Encrypt
	int size = stream->getPosition();
    uint8* data = stream->data.data();
    aes_setkey_enc(aes, secret.data(), secret.size());
    aes_crypt_cbc(aes, AES_ENCRYPT, size, iv, data, data);

    return true;
}

//-----------------------------------//

NAMESPACE_CORE_END