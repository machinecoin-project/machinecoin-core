#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

MACHINECOIND=${MACHINECOIND:-$SRCDIR/machinecoind}
MACHINECOINCLI=${MACHINECOINCLI:-$SRCDIR/machinecoin-cli}
MACHINECOINTX=${MACHINECOINTX:-$SRCDIR/machinecoin-tx}
MACHINECOINQT=${MACHINECOINQT:-$SRCDIR/qt/machinecoin-qt}

[ ! -x $MACHINECOIND ] && echo "$MACHINECOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
MACVER=($($MACHINECOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for machinecoind if --version-string is not set,
# but has different outcomes for machinecoin-qt and machinecoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$MACHINECOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $MACHINECOIND $MACHINECOINCLI $MACHINECOINTX $MACHINECOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${MACVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${MACVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
