#!/bin/sh

# wrapper around dpkg-buildpackage to generate correct changelog
# use "debian/daily-build.sh -b" to create binary packages
# and "debian/daily-build.sh -S" to create a source package only

LC_ALL=C git log|head -n6|grep "Date:"|cut -d' ' -f 5,6,8
version=$(LC_ALL=C git log|head -n6|grep "Date:"|cut -d' ' -f 5,6,8 \
    | sed -r '
s/Jan/01/; tz
s/Feb/02/; tz
s/Mar/03/; tz
s/Apr/04/; tz
s/May/05/; tz
s/Jun/06/; tz
s/Jul/07/; tz
s/Aug/08/; tz
s/Sep/09/; tz
s/Oct/10/; tz
s/Nov/11/; tz
s/Dec/12/; tz
:z
s/^(.+)\b([1-9])\b(.+)$/\10\2\3/
s/^([0-9]+)[ ]+([0-9]+)[ ]+([0-9]+)$/\3\1\2/
')
echo $version

# ensure correct directory
test -r debian/control || exit 1

rm debian/changelog
echo $version > snapshot_version
dch --create --empty --package libutvideo -v 2:12.0~git${version} "Daily build"

dpkg-buildpackage -us -uc -i -I.git "$@"
rm -f snapshot_version
