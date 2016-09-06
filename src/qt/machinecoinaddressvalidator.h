// Copyright (c) 2011-2014 The Machinecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MACHINECOIN_QT_MACHINECOINADDRESSVALIDATOR_H
#define MACHINECOIN_QT_MACHINECOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class MachinecoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit MachinecoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Machinecoin address widget validator, checks for a valid machinecoin address.
 */
class MachinecoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit MachinecoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // MACHINECOIN_QT_MACHINECOINADDRESSVALIDATOR_H
