#!/bin/bash

echo "Generating a DSA key pair for the authority"
./icftool -e authority.priv.icf authority.pub.icf

echo "Creating a self-signed certificate for the authority"
./icftool -c authority.cert.icf authority.pub.icf authority.priv.icf -d365 -l0

echo "Generating DH static key pairs for the initiator and the responder"
./icftool -x responder.priv.icf responder.pub.icf
./icftool -x initiator.priv.icf responder.pub.icf


