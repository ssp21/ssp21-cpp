#!/bin/bash

echo "Generating DH static key pairs for the initiator and the responder"
./icftool -x responder.priv.icf responder.pub.icf
./icftool -x initiator.priv.icf responder.pub.icf

