#!/bin/bash
IP=$(ifconfig | grep 'inet ' | grep -v '127.0.0.1' | awk '{print $2}')
echo "Votre adresse IP est: $IP"
