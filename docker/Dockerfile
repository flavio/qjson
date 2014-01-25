FROM mmckeen/opensuse-13-1

MAINTAINER Flavio Castelli "fcastelli@suse.com"

# make sure the package repository is up to date
RUN zypper --non-interactive --gpg-auto-import-keys ref

RUN zypper in -y cmake \
                 gcc-c++ \
                 libqt5-qtbase-devel
