#!/bin/bash

echo "// Generating a DSA key pair for the authority"
./icftool -e authority.priv.icf authority.pub.icf
echo ""

echo "// Creating a self-signed certificate for the authority"
./icftool -c authority.cert.icf authority.pub.icf authority.priv.icf -d365 -l1
echo ""

echo "// Generating DH static key pairs for the responder"
./icftool -x responder.priv.icf responder.pub.icf
echo ""

echo "// Creating a certificate for the responder signed by authority private key"
./icftool -c responder.cert.icf responder.pub.icf authority.priv.icf -d100 -l0
echo ""

echo "// Generating DH static key pairs for the initiator"
./icftool -x initiator.priv.icf initiator.pub.icf
echo ""

echo "// Creating a certificate for the initiator signed by authority private key"
./icftool -c initiator.cert.icf initiator.pub.icf authority.priv.icf -d100 -l0
echo ""

