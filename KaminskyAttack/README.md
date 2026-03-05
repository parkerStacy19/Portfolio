# Kaminsky Attack Implementation

## Overview

This project simulates the Kaminsky Attack, a critical vulnerability in DNS query resolution that allows attackers to spoof DNS responses and redirect traffic to arbitrary destinations. The Python library Scapy was utilized to create templates for spoofed packets, and a C program rapidly edits and sends them to the target.

A detailed report of this project, its context, and the problem-solving process can be found in ```Report.pdf```.

This was a collaborative project with Ashley Valasquez.

## Project Files

- **attack.c** - Main attack engine written in C that sends raw packets and performs the timing-based attack
- **construct_request.py** - Generates DNS request packet templates using Scapy
- **spoof_reply.py** - Generates spoofed DNS response packets with arbitrary DNS records

## Key Techniques

- Raw socket manipulation for precise packet control
- DNS transaction ID prediction and race condition exploitation
- Authority section injection for DNS cache poisoning

**Disclaimer:** *This project was developed for educational purposes to demonstrate DNS security vulnerabilities and the importance of DNSSEC deployment.*
