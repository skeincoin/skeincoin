Skeincoin Core integration/staging tree
=======================================

https://www.skeincoin.co

What is Skeincoin?
------------------

Skeincoin is an experimental new digital currency that enables instant payments to
anyone, anywhere in the world. Skeincoin uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Skeincoin Core is the name of open source
software which enables the use of this currency.

For more information, as well as an immediately useable, binary version of
the Skeincoin Core software, see https://www.skeincoin.co/

License
-------

Skeincoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see http://opensource.org/licenses/MIT.

Development Process
-------------------

Developers work in their own trees, then submit pull requests when they think
their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the Skeincoin
development team members simply pulls it.

If it is a *more complicated or potentially controversial* change, then the patch
submitter will be asked to start a discussion (if they haven't already) on the
[mailing list](https://lists.linuxfoundation.org/mailman/listinfo/bitcoin-dev)

The developer [mailing list](https://lists.linuxfoundation.org/mailman/listinfo/bitcoin-dev)
should be used to discuss complicated or controversial changes before working
on a patch set.

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/skeincoin/skeincoin/tags) are created
regularly to indicate new official, stable release versions of Skeincoin.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](/doc/unit-tests.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`

There are also [regression and integration tests](/qa) of the RPC interface, written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/qa) are installed) with: `qa/pull-tester/rpc-tests.py`
