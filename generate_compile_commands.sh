#!/bin/sh

if ! command -v bear > /dev/null; then
    echo "Please install bear!"
    echo "  sudo pacman -S bear (Arch)"
    echo "  sudo apt install bear (Debian/Ubuntu)"
    echo "  sudo dnf install bear (Fedora)"
    echo "  brew install bear (macOS)"
    exit 1
fi

bear -- make -B
