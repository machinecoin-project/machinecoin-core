// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "uritests.h"

#include "guiutil.h"
#include "walletmodel.h"

#include <QUrl>

void URITests::uriTests()
{
    SendCoinsRecipient rv;
    QUrl uri;
    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?dontexist="));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?label=Gitju Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw"));
    QVERIFY(rv.label == QString("Gitju Example Address"));
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?amount=0.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000);

    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?amount=1.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100100000);

    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?amount=100&label=Gitju Example"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("Gitju Example"));

    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?message=Gitju Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw"));
    QVERIFY(rv.label == QString());

    QVERIFY(GUIUtil::parseBitcoinURI("machinecoin://MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?message=Gitju Example Address", &rv));
    QVERIFY(rv.address == QString("MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?req-message=Gitju Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?amount=1,000&label=Gitju Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("machinecoin:MDSQf1PSdrpTBE8GGV4ydhAqq9z5AbMDAw?amount=1,000.0&label=Gitju Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));
}
